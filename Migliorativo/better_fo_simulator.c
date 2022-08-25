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

long double Uniform(long double a, long double b){
  return (a + (b - a) * Random());
}

/* lancia F_O_REPS volte il programma main_fo.c, dopo *
 * aver preso in input il seed, la fase e rimosso i   *
 * file precendentemente creati relativi a tempi di   *
 * risposta e utilizzazioni, etc..										*/
int main(int argc, char const *argv[]){
	char buffer[1024];

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
	return 0;
}