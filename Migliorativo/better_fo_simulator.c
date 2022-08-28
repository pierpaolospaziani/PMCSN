#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Risorse/rngs.c"
#include "../Risorse/rvms.h"
#include "../Risorse/rvms.c"
#include "../utils.h"

#ifdef V1
#include "Configurations/Finite/config_v1.h"
#endif
#ifdef V2
#include "Configurations/Finite/config_v2.h"
#endif

long double mean_est[3]			= {0.0,0.0,0.0};
long double stdev_est[3]		= {0.0,0.0,0.0};
long double w_est[3]				= {0.0,0.0,0.0};
long double batch_means[3]	= {0.0,0.0,0.0};
long double interval_est[3] = {0.0,0.0,0.0};
long double medie_f1[256];
long double medie_f2[256];
long double medie_f3[256];

/* permette di calcolare l'intervallo di confidenza del 95% */
void interval_est_func(long n, int r){
  long double u,t;
  if (n > 1) {
    u = 1.0 - 0.5 * (1.0 - 0.95);
    t = idfStudent(n - 1, u);
    w_est[r] = t * stdev_est[r] / sqrt(n - 1);
  }else{
    printf("ERROR - insufficient data\n");
  }
}

long double Uniform(long double a, long double b){
  return (a + (b - a) * Random());
}

/* lancia F_O_REPS volte il programma main_fo.c, dopo *
 * aver preso in input il seed, la fase e rimosso i   *
 * file precendentemente creati relativi a tempi di   *
 * risposta e utilizzazioni, etc..										*/
int main(int argc, char const *argv[]){
	char buffer[1024];
	char buff[1024];
	long double val;
	char *str;
	FILE *file;
	int j = 0;
	int u = 0;
	int num_rip = (STOP/F_O_TIME)*F_O_REPS;

  if(argc < 2){
    printf("\nUsage: program <seed> <phase>");
    exit(-1);
  }

  long seed = atol(argv[1]);
  PlantSeeds(seed);
  SelectStream(8);

	remove("CSV/resp_time.csv");
	remove("CSV/bypass.csv");
	remove("CSV/phase_file_0.csv");
	remove("CSV/phase_file_1.csv");
	remove("CSV/phase_file_2.csv");
	remove("CSV/phase_file_3.csv");
	remove("CSV/phase_file_4.csv");
	remove("CSV/phase_file_5.csv");
	remove("CSV/phase_file_6.csv");
	
  printf("\n");
  fflush(stdout);
	for(int i = 1; i <= F_O_REPS; i++){
    printf("\r Executing %3d° of %d simulations ...", i, F_O_REPS);
		fflush(stdout);
		memset(buffer, 0, 1024);
		snprintf(buffer, sizeof(buffer), "./fo_better %Lf", Uniform(1,9999999));
		system(buffer);
	}
	printf("\r Executed %d simulations successfully!\n\n", F_O_REPS);
	fflush(stdout);

	if((file = fopen("CSV/resp_time.csv", "r")) == NULL){
  	printf("\n fopen error");
  	exit(-1);
  }

  if(fseek(file, 0L, SEEK_SET) != 0){
  	printf("\n fseek error");
  	exit(1);
  }

  long double *rip_buff = (long double *)malloc((STOP/F_O_TIME*F_O_REPS)*sizeof(long double));
	if(rip_buff == NULL){
		printf("\n malloc error");
		exit(1);
	}

  while (fgets(buff, 1024, file)){
    char *token = strtok(buff, ",");
    while (token) { 
      val = strtold(token, &str);
      rip_buff[u] = val;
    	u++;
      token = strtok(NULL, ",");
    }
  }

  if(fclose(file) != 0){
      printf("fclose error");
      exit(1);
  }

  for (int i = 0; i < 256; i++){
  	medie_f1[i] = 0.0;
  	medie_f2[i] = 0.0;
  	medie_f3[i] = 0.0;
  }

  long n = 0;
	long double diff;
	int  off_set = 0;

	for(int r = 0; r < 3; r++){
		int off_set = 0;
		n = 0;
		for(int f = 0; f < F_O_REPS; f++){
			if(f == 0){
				off_set += ((int)((STOP/F_O_TIME)*r)/3) + ((STOP/F_O_TIME)*f);
			}else{
				off_set += (STOP/F_O_TIME);
			}
			for (int i = off_set; i < (off_set+((int) ((STOP/F_O_TIME)*1)/3)); i++){
				if (r == 0){
					medie_f1[f] += rip_buff[i];
				} else if(r == 1){
					medie_f2[f] += rip_buff[i];
				} else {
					medie_f3[f] += rip_buff[i];
				}
			}
			if (r == 0){
				medie_f1[f] = medie_f1[f]/((int) ((STOP/F_O_TIME)*1)/3);
				batch_means[r] += medie_f1[f];
				n++;
				diff = medie_f1[f] - mean_est[r];
				interval_est[r] += diff*diff*(n - 1.0)/n;
				mean_est[r] += diff / n;
			} else if(r == 1){
				medie_f2[f] = medie_f2[f]/((int) ((STOP/F_O_TIME)*1)/3);
				batch_means[r] += medie_f2[f];
				n++;
				diff = medie_f2[f] - mean_est[r];
				interval_est[r] += diff*diff*(n - 1.0)/n;
				mean_est[r] += diff / n;
			} else {
				medie_f3[f] = medie_f3[f]/((int) ((STOP/F_O_TIME)*1)/3);
				batch_means[r] += medie_f3[f];
				n++;
				diff = medie_f3[f] - mean_est[r];
				interval_est[r] += diff*diff*(n - 1.0)/n;
				mean_est[r] += diff / n;
			}
		}
		stdev_est[r] = sqrt(interval_est[r] / n);  
		interval_est_func(n,r);
	}
	
	for(int r = 0; r < 3; r++){
		batch_means[r] = batch_means[r]/((int) ((STOP/F_O_TIME)*1)/3);
	}

	printf("\n ****** The Response Time are:\n");
	fflush(stdout);
	printf(" End Phase 1: [%Lf]+/-[%Lf]\n", mean_est[0], w_est[0]);
	fflush(stdout);
	printf(" End Phase 2: [%Lf]+/-[%Lf]\n", mean_est[1], w_est[1]);
	fflush(stdout);
	printf(" End Phase 3: [%Lf]+/-[%Lf]\n\n\n", mean_est[2], w_est[2]);
	fflush(stdout);
	return 0;
}