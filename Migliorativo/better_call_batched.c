
/* Progetto PMCSN Valerio Crecco, Ludovico De Santis, Pierpaolo Spaziani */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Risorse/rngs.c"
#include "../Risorse/rvms.h"
#include "../Risorse/rvms.c"
#include "../utils.h"

#ifdef V1
#include "Configurations/Infinite/config_v1(1).h"
#endif
#ifdef V2
#include "Configurations/Infinite/config_v1(2).h"
#endif
#ifdef V3
#include "Configurations/Infinite/config_v1(3).h"
#endif
#ifdef V4
#include "Configurations/Infinite/config_v2(1).h"
#endif
#ifdef V5
#include "Configurations/Infinite/config_v2(2).h"
#endif
#ifdef V6
#include "Configurations/Infinite/config_v2(3).h"
#endif

t_center node[5];
int ne_node_index;                  /* indice del nodo che avrà in next event */
int ne_event_index;                 /* indice next event nel nodo */
int g_numb = 0;                     /* global counter of all jobs in the system   */
int max_size[5];                    /* max number of active servers for each node */
int total_arrival;
long int arrivals[5];
long int exits[5];
long int dropped[5];

t_center      batch_node[5];
t_center      prev_batch_node[5];
t_center      curr_batch_node[5];
long double   prev_area[5];
long double   *prev_service[5];
long double   prev_last_center_event[7];
bool          batch_state = false;
int           num_batch = 0;
t_batch_stats batch_stats[BATCH_K];
int           batch_ne_node_index;
int           batch_ne_event_index;
int           batch_g_numb = 0;
int           batch_total_arrival;
long int      batch_arrivals[5];
long int      batch_dropped[5];
long int      batch_exits[5];

long double mean_est[7][6]  = {{0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0}};

long double stdev_est[7][6] = {{0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0}};

long double w_est[7][6]     = {{0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0},
                               {0.0,0.0,0.0,0.0,0.0,0.0}};

long double mean_est_resp     = 0.0;
long double stdev_est_resp    = 0.0;
long double w_est_resp        = 0.0;
long double batch_means_resp  = 0.0;
long double interval_est_resp = 0.0;

FILE *batch_file;
FILE *delay_file;
FILE *bypass;
FILE *phase_file_0;
FILE *phase_file_1;
FILE *phase_file_2;
FILE *phase_file_3;
FILE *phase_file_4;
FILE *phase_file_5;
FILE *phase_file_6;

/* permette di calcolare l'intervallo di confidenza del 95% */
void interval_est_func(long n, int r, int s){
  long double u,t;
  if (n > 1) {
    u = 1.0 - 0.5 * (1.0 - 0.95);
    t = idfStudent(n - 1, u);
    w_est[r][s] = t * stdev_est[r][s] / sqrt(n - 1);
  }else{
    printf("ERROR - insufficient data\n");
  }
}

void interval_est_func_resp(long n){
  long double u,t;
  if (n > 1) {
    u = 1.0 - 0.5 * (1.0 - 0.95);
    t = idfStudent(n - 1, u);
    w_est_resp = t * stdev_est_resp / sqrt(n - 1);
  }else{
    printf("ERROR - insufficient data\n");
  }
}

/* ritorna l'ultimo evento accaduto in un centro */
long double get_max_last_event(int center_index, bool is_dis){
  long double last_center_event = 0.0;
  if (is_dis == true){
    for (int j = 0; j < max_size[center_index]+1; j++){
      if (last_center_event < node[center_index].event[j].t_last_event_d){
        last_center_event = node[center_index].event[j].t_last_event_d;
      }
    }
  } else {
    for (int j = 0; j < max_size[center_index]+1; j++){
      if (last_center_event < node[center_index].event[j].t_last_event){
        last_center_event = node[center_index].event[j].t_last_event;
      }
    }
  }
  return last_center_event;
}

void open_files(){
  remove("CSV/resp_time.csv");
  remove("CSV/delay_time.csv");
  remove("CSV/bypass.csv");
  remove("CSV/phase_file_0.csv");
  remove("CSV/phase_file_1.csv");
  remove("CSV/phase_file_2.csv");
  remove("CSV/phase_file_3.csv");
  remove("CSV/phase_file_4.csv");
  remove("CSV/phase_file_5.csv");
  remove("CSV/phase_file_6.csv");

  if((batch_file = fopen("CSV/resp_time.csv", "a")) == NULL){
    printf("\n fopen error");
    exit(-1);
  }
  if((delay_file = fopen("CSV/delay_time.csv", "a")) == NULL){
    printf("\n fopen error");
    exit(-1);
  }
  if((bypass = fopen("CSV/bypass.csv", "a")) == NULL){
    printf("\n fopen error");
    exit(-1);
  }
  if((phase_file_0 = fopen("CSV/phase_file_0.csv", "a")) == NULL){
    printf("\n fopen error");
    exit(-1);
  }
  if((phase_file_1 = fopen("CSV/phase_file_1.csv", "a")) == NULL){
    printf("\n fopen error");
    exit(-1);
  }
  if((phase_file_2 = fopen("CSV/phase_file_2.csv", "a")) == NULL){
    printf("\n fopen error");
    exit(-1);
  }
  if((phase_file_3 = fopen("CSV/phase_file_3.csv", "a")) == NULL){
    printf("\n fopen error");
    exit(-1);
  }
  if((phase_file_4 = fopen("CSV/phase_file_4.csv", "a")) == NULL){
    printf("\n fopen error");
    exit(-1);
  }
  if((phase_file_5 = fopen("CSV/phase_file_5.csv", "a")) == NULL){
    printf("\n fopen error");
    exit(-1);
  }
  if((phase_file_6 = fopen("CSV/phase_file_6.csv", "a")) == NULL){
    printf("\n fopen error");
    exit(-1);
  }
}

void close_files(){
  if(fclose(batch_file) != 0){
    printf("\n fclose error");
    exit(-1);
  }
  if(fclose(delay_file) != 0){
    printf("\n fclose error");
    exit(-1);
  }
  if(fclose(bypass) != 0){
    printf("\n fclose error");
    exit(-1);
  }
  if(fclose(phase_file_0) != 0){
    printf("\n fclose error");
    exit(-1);
  }
  if(fclose(phase_file_1) != 0){
    printf("\n fclose error");
    exit(-1);
  }
  if(fclose(phase_file_2) != 0){
    printf("\n fclose error");
    exit(-1);
  }
  if(fclose(phase_file_3) != 0){
    printf("\n fclose error");
    exit(-1);
  }
  if(fclose(phase_file_4) != 0){
    printf("\n fclose error");
    exit(-1);
  }
  if(fclose(phase_file_5) != 0){
    printf("\n fclose error");
    exit(-1);
  }
  if(fclose(phase_file_6) != 0){
    printf("\n fclose error");
    exit(-1);
  }
}

/* scrive su file l'utilizzazione del relativo centro passato in input */
void add_utilization_file(int i, long double val){
  FILE *utilization_file;
  if (i == 0){
    utilization_file = phase_file_0;
  } else if (i == 1){
    utilization_file = phase_file_1;
  } else if (i == 2){
    utilization_file = phase_file_2;
  } else if (i == 3){
    utilization_file = phase_file_3;
  } else if (i == 4){
    utilization_file = phase_file_4;
  } else if (i == 5){
    utilization_file = phase_file_5;
  } else if (i == 6){
    utilization_file = phase_file_6;
  }
  if(val < 1.0 && val > 0.00000){
    if(fprintf(utilization_file, "%Lf\n", val) < 0){
      perror("\nfwite error");
      exit(-1);
    }
    fflush(utilization_file);
  } else{
    if(fprintf(utilization_file, "%Lf\n", (long double) 1.0) < 0){
      perror("\nfwite error");
      exit(-1);
    }
    fflush(utilization_file);
  }
}

/* scrive su file delay, wait e prob. di bypass del centro della polizia */
void add_stats_files(long double wait_sum, long double delay_sum, long double val){
  if(fprintf(batch_file, "%Lf\n", wait_sum) < 0){
    perror("\nfwite error");
    exit(-1);
  }
  fflush(batch_file);
  if(fprintf(delay_file, "%Lf\n", delay_sum) < 0){
    perror("\nfwite error");
    exit(-1);
  }
  fflush(delay_file);
  if(val < 1.0 && val > 0.00000){
    if(fprintf(bypass, "%Lf\n", val) < 0){
      perror("\nfwite error");
      exit(-1);
    }
    fflush(bypass);
  } else{
    if(fprintf(bypass, "%Lf\n", (long double) 0.0) < 0){
      perror("\nfwite error");
      exit(-1);
    }
    fflush(bypass);
  }
}

/* permette di salvare le statistiche relative ad un singolo batch eseguito */
void batch_print(){

  node[3].event[0].t = node[3].event[0].t_last_event;
  if (node[4].event[0].t_last_event > node[4].event[0].t_last_event_d){
    node[4].event[0].t = node[4].event[0].t_last_event;
  } else {
    node[4].event[0].t = node[4].event[0].t_last_event_d;
  }

  long double wait_sum  = 0.0;
  long double delay_sum = 0.0;

  for(int i = 0; i < 5; i++){

    node[i].area       -= prev_batch_node[i].area;
    node[i].area_d     -= prev_batch_node[i].area_d;
    node[i].index      -= prev_batch_node[i].index;
    node[i].index_d    -= prev_batch_node[i].index_d;
    node[i].event[0].t -= prev_batch_node[i].event[0].t;
    
    if (i == 3){
      node[3].event[0].t -= prev_batch_node[3].event[0].t_last_event;
    } else if (i == 4 && node[4].event[0].t_last_event > node[4].event[0].t_last_event_d){
      node[4].event[0].t -= prev_batch_node[4].event[0].t_last_event;
    } else if (i == 4) {
      node[4].event[0].t -= prev_batch_node[4].event[0].t_last_event_d;
    }

    if (i == 2){
      long double last_center_event = get_max_last_event(i,true);
      long double utilization_sum = 0.0;

      batch_stats[num_batch].avg_intarr[5]     = node[2].event[0].t / node[2].index_d;
      batch_stats[num_batch].avg_wait[5]       = node[2].area_d     / node[2].index_d;
      batch_stats[num_batch].avg_population[5] = node[2].area_d     / (last_center_event - prev_last_center_event[5]);
      if (node[i].index_d != 0){
        wait_sum += (node[i].area_d / (node[2].index_d + node[2].index));
      }

      long double service_time = 0.0;
      for (int s = 0; s < max_size[2]; s++){
        node[i].stat_d[s].service -= prev_batch_node[2].stat_d[s].service;
        node[i].area_d            -= node[2].stat_d[s].service;
        service_time              += node[2].stat_d[s].service;
        utilization_sum           += node[2].stat_d[s].service / (last_center_event - prev_last_center_event[5]);
      }
      
      batch_stats[num_batch].avg_service[5] = service_time   / node[2].index_d;
      batch_stats[num_batch].avg_delay[5]   = node[2].area_d / node[2].index_d;
      batch_stats[num_batch].avg_queue[5]   = node[2].area_d / (last_center_event - prev_last_center_event[5]);
      
      if (node[i].index_d != 0){
        delay_sum += (node[i].area_d / node[i].index_d);
      }

      prev_last_center_event[5] = last_center_event;
      
      long double val = (long double) utilization_sum/node[i].num_servers;
      add_utilization_file(5,val);

    } else if (i == 4){
      long double last_center_event = get_max_last_event(i,true);
      long double utilization_sum = 0.0;

      batch_stats[num_batch].avg_intarr[6]     = node[4].event[0].t / node[4].index_d;
      batch_stats[num_batch].avg_wait[6]       = node[4].area_d     / node[4].index_d;
      batch_stats[num_batch].avg_population[6] = node[4].area_d     / (last_center_event - prev_last_center_event[6]);
      
      if (node[i].index_d != 0){
        wait_sum += (node[i].area_d / (node[4].index_d + node[4].index));
      }

      long double service_time = 0.0;
      for (int s = 0; s < max_size[4]; s++){
        node[i].stat_d[s].service -= prev_batch_node[i].stat_d[s].service;
        node[i].area_d    -= node[4].stat_d[s].service;
        service_time      += node[4].stat_d[s].service;
        utilization_sum   += node[4].stat_d[s].service / (last_center_event - prev_last_center_event[6]);
      }

      batch_stats[num_batch].avg_service[6] = service_time   / node[4].index_d;
      batch_stats[num_batch].avg_delay[6]   = node[4].area_d / node[4].index_d;
      batch_stats[num_batch].avg_queue[6]   = node[4].area_d / (last_center_event - prev_last_center_event[6]);
      
      if (node[i].index_d != 0){
        delay_sum += (node[i].area_d / node[i].index_d);
      }

      prev_last_center_event[6] = last_center_event;
      
      long double val = (long double) utilization_sum/node[i].num_servers;
      add_utilization_file(6,val);
    }
    long double last_center_event = get_max_last_event(i,false);
    long double utilization_sum = 0.0;

    batch_stats[num_batch].avg_intarr[i]     = node[i].event[0].t / node[i].index;
    batch_stats[num_batch].avg_wait[i]       = node[i].area       / node[i].index;
    batch_stats[num_batch].avg_population[i] = node[i].area       / (last_center_event - prev_last_center_event[i]);

    if (node[i].index != 0){
      wait_sum += (node[i].area / (node[i].index_d + node[i].index));
    }

    long double service_time = 0.0;
    for (int s = 0; s < max_size[i]; s++){
      node[i].stat[s].service -= prev_batch_node[i].stat[s].service;
      node[i].area            -= node[i].stat[s].service;
      service_time            += node[i].stat[s].service;
      utilization_sum         += node[i].stat[s].service / (last_center_event - prev_last_center_event[i]);
    }

    batch_stats[num_batch].avg_service[i] = service_time / node[i].index;
    batch_stats[num_batch].avg_delay[i]   = node[i].area / node[i].index;
    batch_stats[num_batch].avg_queue[i]   = node[i].area / (last_center_event - prev_last_center_event[i]);
    
    if (node[i].index != 0){
      delay_sum += (node[i].area / node[i].index);
    }

    prev_last_center_event[i] = last_center_event;
    
    long double val = (long double) utilization_sum/node[i].num_servers;
    add_utilization_file(i,val);
  }
  long double val = (long double) arrivals[3]/arrivals[4];
  add_stats_files(wait_sum,delay_sum,val);
}

/* permette di salvare lo stato del sistema */
void save_state(t_center *from_center, t_center *center, bool is_batch){
  if (is_batch){
    batch_ne_node_index  = ne_node_index;
    batch_ne_event_index = ne_event_index;
    batch_g_numb         = g_numb;
    batch_total_arrival  = total_arrival;
  }
  for (int i=0; i<5; i++){
    center[i].num_servers   = from_center[i].num_servers;
    center[i].index         = from_center[i].index;
    center[i].area          = from_center[i].area;
    center[i].index_d       = from_center[i].index_d;
    center[i].area_d        = from_center[i].area_d;
    center[i].number        = from_center[i].number;
    center[i].number_d      = from_center[i].number_d;
    for(int j = 0; j < max_size[i]+1; j++){
      center[i].event[j].status         = from_center[i].event[j].status;
      center[i].event[j].is_active      = from_center[i].event[j].is_active;
      center[i].event[j].t              = from_center[i].event[j].t;
      center[i].event[j].t_last_event   = from_center[i].event[j].t_last_event;
      center[i].event[j].t_last_event_d = from_center[i].event[j].t_last_event_d;
      center[i].event[j].is_ghost       = from_center[i].event[j].is_ghost;
      center[i].event[j].flag_d         = from_center[i].event[j].flag_d;
    }
    for(int j = 0; j < max_size[i]; j++){
      center[i].stat[j].service   = from_center[i].stat[j].service;
      center[i].stat[j].served    = from_center[i].stat[j].served;
      center[i].stat_d[j].service = from_center[i].stat_d[j].service;
      center[i].stat_d[j].served  = from_center[i].stat_d[j].served;
    }
    if (is_batch){
      batch_arrivals[i] = arrivals[i];
      batch_dropped[i]  = dropped[i];
      batch_exits[i]    = exits[i];
    }
  }
}

/* permettere di reimpostare lo stato dei centri del sistema */
void reset_state(){
  ne_node_index  = batch_ne_node_index;
  ne_event_index = batch_ne_event_index;
  g_numb         = batch_g_numb;
  total_arrival  = batch_total_arrival;
  for (int i=0; i<5; i++){
    node[i].num_servers = batch_node[i].num_servers;
    node[i].index       = batch_node[i].index;
    node[i].area        = batch_node[i].area;
    node[i].index_d     = batch_node[i].index_d;
    node[i].area_d      = batch_node[i].area_d;
    node[i].number      = batch_node[i].number;
    node[i].number_d    = batch_node[i].number_d;
    for(int j = 0; j < max_size[i]+1; j++){
      node[i].event[j].status         = batch_node[i].event[j].status;
      node[i].event[j].is_active      = batch_node[i].event[j].is_active;
      node[i].event[j].t              = batch_node[i].event[j].t;
      node[i].event[j].t_last_event   = batch_node[i].event[j].t_last_event;
      node[i].event[j].t_last_event_d = batch_node[i].event[j].t_last_event_d;
      node[i].event[j].is_ghost       = batch_node[i].event[j].is_ghost;
      node[i].event[j].flag_d         = batch_node[i].event[j].flag_d;
    }
    for(int j = 0; j < max_size[i]; j++){
      node[i].stat[j].service   = batch_node[i].stat[j].service;
      node[i].stat[j].served    = batch_node[i].stat[j].served;
      node[i].stat_d[j].service = batch_node[i].stat_d[j].service;
      node[i].stat_d[j].served  = batch_node[i].stat_d[j].served;
    }
    arrivals[i] = batch_arrivals[i];
    dropped[i]  = batch_dropped[i];
    exits[i]    = batch_exits[i];
  }
}

void max_num_centers(){

  max_size[0] = NUM_SERVER_C0_1;
  max_size[1] = NUM_SERVER_C1_1;
  max_size[2] = NUM_SERVER_C2_1;
  max_size[3] = NUM_SERVER_C3_1;
  max_size[4] = NUM_SERVER_C4_1;

  if (max_size[0] < NUM_SERVER_C0_2){
    max_size[0] = NUM_SERVER_C0_2;
  }
  if (max_size[1] < NUM_SERVER_C1_2){
    max_size[1] = NUM_SERVER_C1_2;
  }
  if (max_size[2] < NUM_SERVER_C2_2){
    max_size[2] = NUM_SERVER_C2_2;
  }
  if (max_size[3] < NUM_SERVER_C3_2){
    max_size[3] = NUM_SERVER_C3_2;
  }
  if (max_size[4] < NUM_SERVER_C4_2){
    max_size[4] = NUM_SERVER_C4_2;
  }
  if (max_size[0] < NUM_SERVER_C0_3){
    max_size[0] = NUM_SERVER_C0_3;
  }
  if (max_size[1] < NUM_SERVER_C1_3){
    max_size[1] = NUM_SERVER_C1_3;
  }
  if (max_size[2] < NUM_SERVER_C2_3){
    max_size[2] = NUM_SERVER_C2_3;
  }
  if (max_size[3] < NUM_SERVER_C3_3){
    max_size[3] = NUM_SERVER_C3_3;
  }
  if (max_size[4] < NUM_SERVER_C4_3){
    max_size[4] = NUM_SERVER_C4_3;
  }
}

void initialize_centers(t_center *center){
  node[0].num_servers = NUM_SERVER_C0_1;
  node[1].num_servers = NUM_SERVER_C1_1;
  node[2].num_servers = NUM_SERVER_C2_1;
  node[3].num_servers = NUM_SERVER_C3_1;
  node[4].num_servers = NUM_SERVER_C4_1;

  max_num_centers();
  
  for(int i = 0;i < 5; i++){
    center[i].index           = 0;
    center[i].area            = 0.0;
    center[i].index_d         = 0;
    center[i].area_d          = 0.0;
    center[i].number          = 0;
    center[i].number_d        = 0;
    arrivals[i]               = 0;
    exits[i]                  = 0;
    dropped[i]                = 0;
    prev_area[i]              = 0.0;
    prev_last_center_event[i] = 0.0;
    center[i].stat = malloc(sizeof(sum)*max_size[i]);
    if (center[i].stat == NULL){
      printf("malloc error!\n");
      fflush(stdout);
      exit(1);
    }
    center[i].stat_d = malloc(sizeof(sum)*max_size[i]);
    if (center[i].stat_d == NULL){
      printf("malloc error!\n");
      fflush(stdout);
      exit(1);
    }
    prev_service[i] = malloc(sizeof(long double)*max_size[i]);
    if (prev_service[i] == NULL){
      printf("malloc error!\n");
      fflush(stdout);
      exit(1);
    }
    for(int j = 0; j < max_size[i]; j++){
      center[i].stat[j].service   = 0.0;
      center[i].stat[j].served    = 0;
      center[i].stat_d[j].service   = 0.0;
      center[i].stat_d[j].served    = 0;
      prev_service[i][j]          = 0.0;
    }
    center[i].event = malloc(sizeof(event_list)*(max_size[i]+1));
    if (center[i].event == NULL){
      printf("malloc error!\n");
      fflush(stdout);
      exit(1);
    }
    for(int j = 0; j < max_size[i]+1; j++){
      center[i].event[j].status         = IDLE;
      center[i].event[j].is_active      = NOT_ACTIVE;
      center[i].event[j].t              = START;
      center[i].event[j].t_last_event   = 0.0;
      center[i].event[j].t_last_event_d = 0.0;
      center[i].event[j].is_ghost       = false;
      center[i].event[j].flag_d         = false;
    }

    for(int j = 0; j < center[i].num_servers+1; j++){
      center[i].event[j].is_active    = ACTIVE;
    }
  }
}

long double Exponential(long double m){
  return (-m * log(1.0 - Random()));
}

long double Uniform(long double a, long double b){
  return (a + (b - a) * Random());
}

/* seleziona una diversa configurazione di serventi in *
 * base alla fascia oraria                             */
int caso(int phase, int center_index){
  if (phase == 2){
    switch(center_index){
      case 0:
        return NUM_SERVER_C0_2;
      case 1:
        return NUM_SERVER_C1_2;
      case 2:
        return NUM_SERVER_C2_2;
      case 3:
        return NUM_SERVER_C3_2;
      default:
        return NUM_SERVER_C4_2;
    }
  } else {
    switch(center_index){
      case 0:
        return NUM_SERVER_C0_3;
      case 1:
        return NUM_SERVER_C1_3;
      case 2:
        return NUM_SERVER_C2_3;
      case 3:
        return NUM_SERVER_C3_3;
      default:
        return NUM_SERVER_C4_3;
    }
  }
  return 0;
}

/* gestisce lo stato dei serventi che vengono allocati/deallocati *
 * a seconda del cambio di fascia oraria.                         */
void switch_phase(int phase){
  int old_num_servers;
  int old_queue;
  int diff;
  int new_center_index;
  for (int i = 0; i < 5; i++){
    old_num_servers = node[i].num_servers;
    old_queue = node[i].number + node[i].number_d - old_num_servers;
    node[i].num_servers = caso(phase,i);
    diff = node[i].num_servers - old_num_servers;

    if (diff > 0){
      new_center_index = old_num_servers + 1;             /* ... attivo i nuovi server ... */
      while(new_center_index <= node[i].num_servers){
        node[i].event[new_center_index].is_active = ACTIVE;
        if (phase == 2){
          node[i].event[new_center_index].t = FHOUR;
        } else {
          node[i].event[new_center_index].t = SHOUR;
        }
        new_center_index++;
      }
      new_center_index = old_num_servers + 1;             /* ... e li segno come BUSY così li prende la NextEvent */
      while(old_queue > 0 && new_center_index <= node[i].num_servers){
        node[i].event[new_center_index].status = BUSY;
        node[i].event[new_center_index].is_ghost = true;
        old_queue--;
        new_center_index++;
      }
    } else if (diff < 0){                                 /* se ho tolto server ... */
      new_center_index = node[i].num_servers + 1;         /* ... disattivo i server vuoti */
      while(new_center_index <= old_num_servers){
        node[i].event[new_center_index].is_active = NOT_ACTIVE;
        new_center_index++;
      }
    }
  }
}

long double GetArrival(int phase){     
  static long double arrival = START;
  SelectStream(0);
  switch(phase){
    case 1:
      arrival += Exponential(1/LAMBDA1);
      break;
    case 2:
      arrival += Exponential(1/LAMBDA2);
      break;
    case 3:
      arrival += Exponential(1/LAMBDA3);
      break;
  }
  return (arrival);
}

long double GetService(int index){
  SelectStream(index);
  switch(index){
    case PARK_ENTRY:
      return (Exponential(AVG_SERVICE_C0));
    case TICKET_BUY:
      return (Exponential(AVG_SERVICE_C1));
    case DOC_PAT:
      return (Exponential(AVG_SERVICE_C2));
    case POLICE_CONTROL:
      return (Exponential(AVG_SERVICE_C3));
    default:
      return (Exponential(AVG_SERVICE_C4));
  }
}

/* effettua il routing degli arrivi esterni */
int initial_routing_func() {
  long double random = Uniform(0, 1);
  if (random <= P_C0){
    return PARK_ENTRY;
  }else if (random <= (P_C0 + P_C1)){
    return TICKET_BUY;
  }else{
    return DOC_PAT;
  }
}

/* trova un servente IDLE applicando la politica Equity, al quale assegnare il job */
int FindOne(event_list event[], int num_servers){
  int s;
  int i = 1;
  while (event[i].status == BUSY)
    i++;
  s = i;
  while (i < num_servers) {
    i++;
    if ((event[i].status == IDLE) && (event[i].t < event[s].t))
      s = i;
  }
  return (s);
}

/* gestione di un nuovo arrivo in un centro. Se un job non trova coda viene mandato *
 * in servizio invocando la FindOne e successivamente vengono aggiornate le         *
 * statistiche del centro. Se il job, invece, trova coda viene messo in coda        */
void NewArrivalManage(long double current, int phase, bool is_dis){
  int dest;
  int s;
  if(ne_node_index == PARK_ENTRY || ne_node_index == TICKET_BUY || ne_node_index == DOC_PAT){
    arrivals[ne_node_index]++;
    int busy_not_active_servers = 0;
    for (int i=1; i<max_size[ne_node_index]+1; i++){
      if (node[ne_node_index].event[i].is_active == NOT_ACTIVE && node[ne_node_index].event[i].status == BUSY){
        busy_not_active_servers++;
      }
    }
    total_arrival++;
    batch_state = false;
    if ((node[ne_node_index].number + node[ne_node_index].number_d) <= (node[ne_node_index].num_servers + busy_not_active_servers)) {
      long double service = GetService(ne_node_index);
      
      s = FindOne(node[ne_node_index].event,node[ne_node_index].num_servers);
      
      if(is_dis == true){
        node[ne_node_index].event[s].flag_d = true;
        node[ne_node_index].stat_d[s-1].service += service;
        node[ne_node_index].stat_d[s-1].served++;
        node[ne_node_index].event[s].t_last_event_d = node[ne_node_index].event[ne_event_index].t;
      }else{
        node[ne_node_index].event[s].flag_d = false;
        node[ne_node_index].stat[s-1].service += service;
        node[ne_node_index].stat[s-1].served++;
        node[ne_node_index].event[s].t_last_event = node[ne_node_index].event[ne_event_index].t;
      }
      node[ne_node_index].event[s].t = current + service;
      node[ne_node_index].event[s].status = BUSY;
    }
    if(is_dis){
      node[ne_node_index].event[0].t_last_event_d = node[ne_node_index].event[ne_event_index].t;
    }else{
      node[ne_node_index].event[0].t_last_event = node[ne_node_index].event[ne_event_index].t;
    }
    dest = initial_routing_func();
    node[dest].event[0].t = GetArrival(phase);
    node[ne_node_index].event[0].status = IDLE;
    node[dest].event[0].status = BUSY;

    if (total_arrival == (BATCH_K * BATCH_B)){
      node[dest].event[0].status = IDLE;
      g_numb--;
    }
  }
}

/* restituisce l'evento più recente accaduto in un nodo */
int NextEventSingleNode(event_list event[], int attual_node){
  int e;                                      
  int i = 0;
  while (event[i].status == IDLE && i <= max_size[attual_node]){
    i++;
  }
  if (i > max_size[attual_node]){
    return -1;
  }
  e = i;
  while (i < max_size[attual_node]) {
    i++;
    if ((event[i].status == BUSY) && (event[i].t < event[e].t)){
      e = i;
    }
  }
  return (e);
}

/* conta il numero di persone disabili presenti nel sistema */
int count_d_running(){
  int count = 0;
  for(int i = 0; i < max_size[ne_node_index] + 1; i++){
    if((node[ne_node_index].event[i].flag_d == true) && (node[ne_node_index].event[i].status == BUSY)){
      count++;
    }
  }
  return count;
}

/* trova il next event del sistema */
void NextEventNodes(){
  int current;
  long double t_min = (1000000*STOP);
  for(int i = 0; i < 5; i++){
    current = NextEventSingleNode(node[i].event,i);
    if (current == -1){
      continue;
    }
    if(t_min > node[i].event[current].t ){
      t_min = node[i].event[current].t;
      ne_node_index = i;
      ne_event_index = current;
    }
  }
}

/* permette la gestione di un job che arriva in un nuovo nodo del sistema */
void entry_next_node(int center){
  if(node[ne_node_index].event[ne_event_index].flag_d == true){
    node[center].area_d += (node[ne_node_index].event[ne_event_index].t - get_max_last_event(center,true)) * node[center].number_d;
    node[center].number_d++;
    node[center].event[0].t_last_event_d = node[ne_node_index].event[ne_event_index].t;
  }else{
    node[center].area += (node[ne_node_index].event[ne_event_index].t - get_max_last_event(center,false)) * node[center].number;
    node[center].number++;
    node[center].event[0].t_last_event = node[ne_node_index].event[ne_event_index].t;
  }
  int busy_not_active_servers = 0;
  for (int i=1; i<max_size[center]+1; i++){
    if (node[center].event[i].is_active == NOT_ACTIVE && node[center].event[i].status == BUSY){
      busy_not_active_servers++;
    }
  }
  if((node[center].number + node[center].number_d) <= (node[center].num_servers + busy_not_active_servers)){
    long double service = GetService(center);
    int s = FindOne(node[center].event,node[center].num_servers);
    if(node[ne_node_index].event[ne_event_index].flag_d == true){
      node[center].event[s].flag_d = true;
      node[center].event[s].t_last_event_d = node[ne_node_index].event[ne_event_index].t;
      node[center].stat_d[s-1].service += service;
      node[center].stat_d[s-1].served++;
    }else{
      node[center].event[s].flag_d = false;
      node[center].event[s].t_last_event = node[ne_node_index].event[ne_event_index].t;
      node[center].stat[s-1].service += service;
      node[center].stat[s-1].served++;
    }
    node[center].event[s].t = node[ne_node_index].event[ne_event_index].t + service;
    node[center].event[s].status = BUSY;
  }
}

/* gestisce le uscite dal centro POLICE CONTROL */
void entry_next_node_police(){
  if(node[POLICE_CONTROL].number >= node[POLICE_CONTROL].num_servers){
    arrivals[4]++;
    entry_next_node(TURNSTILES);
  }else{
    arrivals[3]++;
    entry_next_node(POLICE_CONTROL);
  }
}

/* gestisce le uscite dal centro PARK ENTRY */
void routing_park_entry(){
  node[0].index++;
  long double random = Uniform(0, 1);
  if (random <= P_SB0){
    g_numb--;
    dropped[0]++;
  } else {
    exits[0]++;
    random = Uniform(0, 1);
    if(random <= P_PD0){
      arrivals[4]++;
      node[ne_node_index].event[ne_event_index].flag_d = true;
      entry_next_node(TURNSTILES);
      node[ne_node_index].event[ne_event_index].flag_d = false;
    }else{
      entry_next_node_police();
    }
  }
}

/* gestisce le uscite dal centro TICKET BUY */
void routing_ticket_buy(){
  node[1].index++;
  long double random = Uniform(0, 1);
  if (random <= P_SB1){
    g_numb--;
    dropped[1]++;
  } else {
    exits[1]++;
    arrivals[2]++;
    random = Uniform(0, 1);
    if (random <= P_CD){
      node[ne_node_index].event[ne_event_index].flag_d = true;
    }
    entry_next_node(DOC_PAT);
    node[ne_node_index].event[ne_event_index].flag_d = false;
  }
}

/* gestisce le uscite dal centro DOC & PAT */
void routing_doc_pat(){
  if (node[2].event[ne_event_index].flag_d == true){
    node[2].index_d++;
  } else {
    node[2].index++;
  }
  long double random = Uniform(0, 1);
  if (random <= P_SB2){
    g_numb--;
    dropped[2]++;
  }else{
    exits[2]++;
    if(node[ne_node_index].event[ne_event_index].flag_d == true){
      arrivals[4]++;
      entry_next_node(TURNSTILES);
    }else{
      entry_next_node_police();
    }
  }
}

/* gestisce le uscite dal centro POLICE CONTROL */
void routing_police_control(){
  node[3].index++;
  long double random = Uniform(0, 1);
  if (random <= P_CP3){
    g_numb--;
    dropped[3]++;
  }else {
    exits[3]++;
    arrivals[4]++;
    entry_next_node(TURNSTILES);
  }
}

/* gestisce le uscite dal centro TURNSTILES */
void routing_turnstiles(){
  long double random = Uniform(0, 1);
  g_numb--;
  if (node[4].event[ne_event_index].flag_d == true){
    node[4].index_d++;
  } else {
    node[4].index++;
  }
  if (random <= P_BS4){   
    dropped[4]++;
  }else{
    exits[4]++;
  }
}

/* gestisce i completamenti di un centro. Se ho un completamento e ho coda *
 * processorò un altro job, altrimenti il server verrà messo IDLE          */
void NewCompletionManage(long double current){
  if (!(node[ne_node_index].event[ne_event_index].is_ghost)){
    switch(ne_node_index){
      case PARK_ENTRY:
        routing_park_entry();
        break;
      case TICKET_BUY:
        routing_ticket_buy();
        break;
      case DOC_PAT:
        routing_doc_pat();
        break;
      case POLICE_CONTROL:
        routing_police_control();
        break;
      default:
        routing_turnstiles();
        break;
    }
    int busy_not_active_servers = 0;
    for (int i = 1; i < max_size[ne_node_index] + 1; i++){
      if (node[ne_node_index].event[i].is_active == NOT_ACTIVE && node[ne_node_index].event[i].status == BUSY){
        busy_not_active_servers++;
      }
    }
    if((node[ne_node_index].number + node[ne_node_index].number_d) >= (node[ne_node_index].num_servers + busy_not_active_servers) && node[ne_node_index].event[ne_event_index].is_active == ACTIVE){
      long double service = GetService(ne_node_index);
      if(node[ne_node_index].number_d > count_d_running()){
        node[ne_node_index].event[ne_event_index].flag_d = true;
        node[ne_node_index].stat_d[ne_event_index-1].service += service;
        node[ne_node_index].stat_d[ne_event_index-1].served++;
      } else {
        node[ne_node_index].event[ne_event_index].flag_d = false;
        node[ne_node_index].stat[ne_event_index-1].service += service;
        node[ne_node_index].stat[ne_event_index-1].served++;
      }
      node[ne_node_index].event[ne_event_index].t = current + service;
    } else {
      node[ne_node_index].event[ne_event_index].status = IDLE;
      node[ne_node_index].event[ne_event_index].flag_d = false;
    }
  }else{
    long double service = GetService(ne_node_index);
    if(node[ne_node_index].number_d > count_d_running()){
      node[ne_node_index].event[ne_event_index].flag_d = true;
      node[ne_node_index].stat_d[ne_event_index-1].service += service;
      node[ne_node_index].stat_d[ne_event_index-1].served++;
    } else {
      node[ne_node_index].event[ne_event_index].flag_d = false;
      node[ne_node_index].stat[ne_event_index-1].service += service;
      node[ne_node_index].stat[ne_event_index-1].served++;
    }
    node[ne_node_index].event[ne_event_index].is_ghost = false;
    node[ne_node_index].event[ne_event_index].t = current + service;
  }
}

/* controlla se vi sono eventi da processare */
bool control(){
  if(g_numb > 0)
    return true;
  for(int i = 0;i < 5;i++){
    if (node[i].event[0].status != 0)
      return true;
  }
  return false;
}

int main(int argc, char const *argv[]){

    if(argc < 2){
      printf("\nUsage: program <seed>");
      exit(-1);
    }

    open_files();

    long seed = atol(argv[1]);

    struct {
      long double current;
      long double next;
    } t;
  
    int phase = 1;
    int s;
    int dest;

    long double avg_intarr[5];
    long double avg_intarr_d[5];
    long double avg_wait[5];
    long double avg_wait_d[5];
    long double avg_population[5];
    long double avg_population_d[5];
    long double avg_service[5];
    long double avg_delay[5];
    long double avg_queue[5];
    long double avg_service_d[5];
    long double avg_delay_d[5];
    long double avg_queue_d[5];

    initialize_centers(node);
    initialize_centers(batch_node);
    initialize_centers(prev_batch_node);
    initialize_centers(curr_batch_node);
    
    PlantSeeds(seed);
    t.current = START;
    dest = initial_routing_func();

    node[dest].event[0].t = GetArrival(phase);
    node[dest].event[0].status = BUSY;
    g_numb++;
    int k=0;
    int prev_arr = 0;
  label:
    while (control()) {
      if (total_arrival%BATCH_B == 0 && batch_state == false && total_arrival != prev_arr){
        printf("\r Executing %3d° of %d batches ...", num_batch+1, BATCH_K);
        fflush(stdout);
        prev_arr = total_arrival;
        batch_state = true;
        save_state(node,batch_node,true);
        for(int s = 0; s < 5; s++){
          node[s].event[0].status = IDLE;
        }
      }
      
      k++;
      NextEventNodes();
      
      bool is_dis;
      long double random = Uniform(0, 1);
      if(ne_node_index == DOC_PAT && ne_event_index == 0 && random <= P_CD){
        is_dis = true;
      }else{
        is_dis = false;
      }
      
      t.next = node[ne_node_index].event[ne_event_index].t;

      if (node[ne_node_index].event[ne_event_index].flag_d == true){
        node[ne_node_index].area_d += (t.next - get_max_last_event(ne_node_index,true)) * node[ne_node_index].number_d;
      } else {
        if (is_dis){
          node[ne_node_index].area_d += (t.next - get_max_last_event(ne_node_index,true)) * node[ne_node_index].number_d;
        } else {
          node[ne_node_index].area   += (t.next - get_max_last_event(ne_node_index,false)) * node[ne_node_index].number;
        }
      }

      t.current = t.next;

      if(t.current < FHOUR){
        phase = 1;
      } else if(t.current <= SHOUR){
        phase = 2;
        switch_phase(2);
      } else {
        phase = 3;
        switch_phase(3);
      }

      bool was_d = false;
      if (ne_event_index == 0) {
        g_numb++;
        if(is_dis){
          node[ne_node_index].number_d++;
          NewArrivalManage(t.current, phase, true);
        }else{
          node[ne_node_index].number++;
          NewArrivalManage(t.current, phase, false);
        }
      }else{
        if (!(node[ne_node_index].event[ne_event_index].is_ghost)){
          if(node[ne_node_index].event[ne_event_index].flag_d == true){
            was_d = true;
            node[ne_node_index].number_d--;
          }else{
            was_d = false;
            node[ne_node_index].number--;
          }
        }
        NewCompletionManage(t.current);
      }
      if (was_d){
        node[ne_node_index].event[ne_event_index].t_last_event_d = t.current;
      } else {
        node[ne_node_index].event[ne_event_index].t_last_event = t.current;
      }
    }

    if (total_arrival != (BATCH_B*BATCH_K)){
      save_state(node,curr_batch_node,false);
      batch_print();
      save_state(curr_batch_node,prev_batch_node,false);
      num_batch++;
      reset_state();
      goto label;
    }
    save_state(node,batch_node,true);
    batch_print();
    reset_state();

    close_files();

    node[3].event[0].t = node[3].event[0].t_last_event;
    
    if (node[4].event[0].t_last_event > node[4].event[0].t_last_event_d){
      node[4].event[0].t = node[4].event[0].t_last_event;
    } else {
      node[4].event[0].t = node[4].event[0].t_last_event_d;
    }

    for(int i = 0; i < 5; i++){

      long double last_center_event   = get_max_last_event(i,false);
      long double last_center_event_d = get_max_last_event(i,true);
      if (last_center_event < last_center_event_d){
        last_center_event = last_center_event_d;
      }

      printf("\nCENTER %d:", i);
      fflush(stdout);

      avg_intarr[i]       = node[i].event[0].t / node[i].index;
      avg_intarr_d[i]     = node[i].event[0].t / node[i].index_d;
      avg_wait[i]         = node[i].area       / node[i].index;
      avg_wait_d[i]       = node[i].area_d     / node[i].index_d;
      avg_population[i]   = node[i].area       / last_center_event;
      avg_population_d[i] = node[i].area_d     / last_center_event;

      long int sum_served = 0;
      long int sum_served_d = 0;
      for (s = 0; s < max_size[i]; s++){
        sum_served   += node[i].stat[s].served;
        sum_served_d += node[i].stat_d[s].served;
      }

      printf("\nfor %ld jobs the service node statistics are:\n\n", node[i].index - dropped[i] + node[i].index_d);
      fflush(stdout);
      printf("  # arrivals ........... = %ld\n", arrivals[i]);
      fflush(stdout);
      printf("  # dropped ............ = %ld\n", dropped[i]);
      fflush(stdout);
      printf("  # exits .............. = %ld\n\n", exits[i]);
      fflush(stdout);
      
      long double service_time      = 0.0;
      long double service_time_d    = 0.0;
      long double utilization_sum   = 0.0;
      long double utilization_sum_d = 0.0;

      for (s = 0; s < max_size[i]; s++){
        node[i].area -= node[i].stat[s].service;
        service_time += node[i].stat[s].service;
        utilization_sum += node[i].stat[s].service / last_center_event;
        node[i].area_d -= node[i].stat_d[s].service;
        service_time_d += node[i].stat_d[s].service;
        utilization_sum_d += node[i].stat_d[s].service / last_center_event_d;
      }

      avg_service[i]   = service_time   / node[i].index;
      avg_delay[i]     = node[i].area   / node[i].index;
      avg_queue[i]     = node[i].area   / last_center_event;
      avg_service_d[i] = service_time_d / node[i].index_d;
      avg_delay_d[i]   = node[i].area_d / node[i].index_d;
      avg_queue_d[i]   = node[i].area_d / last_center_event_d;

      if (i == 2 || i == 4){
        printf(" PRIO QUEUE:\n");
        printf("  d_jobs processed ..... = %ld\n", node[i].index_d);
        fflush(stdout);
        printf("  avg interarrivals_d .. = %6.5Lf\n", avg_intarr_d[i]);
        fflush(stdout);
        printf("  avg #_d in node ...... = %6.5Lf\n", avg_population_d[i]);
        fflush(stdout);
        printf("  avg wait_d ........... = %6.5Lf\n", avg_wait_d[i]);
        fflush(stdout);
        printf("  avg service_d ........ = %6.5Lf\n", avg_service_d[i]);
        fflush(stdout);
        printf("  avg delay_d .......... = %6.5Lf\n", avg_delay_d[i]);
        fflush(stdout);
        printf("  avg # in queue_d ..... = %6.5Lf\n", avg_queue_d[i]);
        fflush(stdout);
        printf("  avg utilization_d .... = %6.5Lf\n", utilization_sum_d/max_size[i]);
        fflush(stdout);
        printf("\n NO-PRIO QUEUE:\n");
      }
      printf("  jobs processed ....... = %ld\n", node[i].index);
      fflush(stdout);
      printf("  avg interarrivals .... = %6.5Lf\n", avg_intarr[i]);
      fflush(stdout);
      printf("  avg # in node ........ = %6.5Lf\n", avg_population[i]);
      fflush(stdout);
      printf("  avg wait ............. = %6.5Lf\n", avg_wait[i]);
      fflush(stdout);
      printf("  avg service .......... = %6.5Lf\n", avg_service[i]);
      fflush(stdout);
      printf("  avg delay ............ = %6.5Lf\n", avg_delay[i]);
      fflush(stdout);
      printf("  avg # in queue ....... = %6.5Lf\n", avg_queue[i]);
      fflush(stdout);
      printf("  avg utilization ...... = %6.5Lf\n", utilization_sum/max_size[i]);
      fflush(stdout);

      printf("\n****** the server statistics are:\n\n");

      long double sum_share_d = 0.0;
      if (i == 2 || i == 4){
        printf(" PRIO QUEUE:\n");
        printf("   server_d    utilization_d   avg service_d    share_d     served_d\n");
        fflush(stdout);

        for (s = 0; s < max_size[i]; s++){
          sum_share_d += (long double)node[i].stat_d[s].served / node[i].index_d;
          printf("%8d %14.5Lf %15.5Lf %15.5Lf %10ld\n",
            s+1,
            node[i].stat_d[s].service / last_center_event_d,
            node[i].stat_d[s].service / node[i].stat_d[s].served,
            (long double)node[i].stat_d[s].served / node[i].index_d,
            node[i].stat_d[s].served);
          fflush(stdout);
        }
        printf("\n NO-PRIO QUEUE:\n");
      }
      printf("    server     utilization     avg service      share       served\n");
      fflush(stdout);

      long double sum_share   = 0.0;
      for (s = 0; s < max_size[i]; s++){
        sum_share   += (long double)node[i].stat[s].served   / node[i].index;
        printf("%8d %14.5Lf %15.5Lf %15.5Lf %10ld\n",
          s+1,
          node[i].stat[s].service / last_center_event,
          node[i].stat[s].service / node[i].stat[s].served,
          (long double)node[i].stat[s].served / node[i].index,
          node[i].stat[s].served);
        fflush(stdout);
      }

      printf("\n");
      fflush(stdout);
    }
  
  long double batch_avg_intarr[7]     = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  long double batch_avg_wait[7]       = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  long double batch_avg_population[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  long double batch_avg_service[7]    = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  long double batch_avg_delay[7]      = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  long double batch_avg_queue[7]      = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  for (int i=0; i<BATCH_K; i++){
    for (int j=0; j<7; j++){
      batch_avg_intarr[j%7]     += batch_stats[i].avg_intarr[j];
      batch_avg_wait[j%7]       += batch_stats[i].avg_wait[j];
      batch_avg_population[j%7] += batch_stats[i].avg_population[j];
      batch_avg_service[j%7]    += batch_stats[i].avg_service[j];
      batch_avg_delay[j%7]      += batch_stats[i].avg_delay[j];
      batch_avg_queue[j%7]      += batch_stats[i].avg_queue[j];
    }
  }
  printf("\n\n");
  fflush(stdout);

  long double **rip_buff = (long double **)malloc((BATCH_K*7)*sizeof(long double *));
  if(rip_buff == NULL){
    printf("\n malloc error");
    exit(1);
  }
  for(int k = 0; k < (BATCH_K*7); k++){
    rip_buff[k] = (long double *)malloc(6*sizeof(long double));
    if(rip_buff[k] == NULL){
      printf("\n malloc error");
      exit(1);
    }  
  }

  long double batch_means[7][6]  = {{0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0}};

  long double interval_est[7][6] = {{0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0},
                                    {0.0,0.0,0.0,0.0,0.0,0.0}};

  for (int i=0; i<BATCH_K; i++){
    for (int j=0; j<7; j++){
      rip_buff[(i*7)+j][0] = batch_stats[i].avg_intarr[j];
      rip_buff[(i*7)+j][1] = batch_stats[i].avg_wait[j];
      rip_buff[(i*7)+j][2] = batch_stats[i].avg_population[j];
      rip_buff[(i*7)+j][3] = batch_stats[i].avg_service[j];
      rip_buff[(i*7)+j][4] = batch_stats[i].avg_delay[j];
      rip_buff[(i*7)+j][5] = batch_stats[i].avg_queue[j];
    }
  }

  long n = 0;
  long double diff;
  for(int r = 0; r < 7; r++){
    for(int s = 0;s < 6; s++){
      int  off_set = 0;
      n = 0;
      for(int f = 0; f < BATCH_K; f++){
        if(f == 0){
          off_set += r+(7*f);
        }else{
          off_set += 7;
        }
        batch_means[r][s] += rip_buff[off_set][s];
        n++;
        diff = rip_buff[off_set][s] - mean_est[r][s];
        interval_est[r][s] += diff*diff*(n - 1.0)/n;
        mean_est[r][s] += diff / n;
      }
      stdev_est[r][s] = sqrt(interval_est[r][s] / n);  
      interval_est_func(n,r,s);
    }
  }

  for(int r_index = 0; r_index < 7; r_index++){
    for(int c_index = 0; c_index < 6; c_index++){
      batch_means[r_index][c_index] = batch_means[r_index][c_index]/BATCH_K;
    }
  }

  for(int r_index = 0; r_index < 5; r_index++){
    printf("\n ****** the center %d Global statistics are:\n\n", r_index);
  back:
    if (r_index == 2){
      r_index = 5;
      printf(" PRIO QUEUE ...........................\n");
    } else if (r_index == 5){
      r_index = 2;
      printf("\n NO-PRIO QUEUE ........................\n");
    } else if (r_index == 4){
      r_index = 6;
      printf(" PRIO QUEUE ...........................\n");
    } else if (r_index == 6){
      r_index = 4;
      printf("\n NO-PRIO QUEUE ........................\n");
    }
    for(int c_index = 0; c_index < 6; c_index++){
      switch(c_index){
        case 0:
          printf(" avg intarr");
          fflush(stdout);
          break;
        case 1:
          printf(" avg wait");
          fflush(stdout);
          break;
        case 2:
          printf(" avg population");
          fflush(stdout);
          break;
        case 3:
          printf(" avg service");
          fflush(stdout);
          break;
        case 4:
          printf(" avg delay");
          fflush(stdout);
          break;
        case 5:
          printf(" avg queue");
          fflush(stdout);
          break;
      }
      printf("\t[%.5Lf]+/-[%.5Lf]\n",
        mean_est[r_index][c_index],
        w_est[r_index][c_index]);
      fflush(stdout);
    }
    if (r_index == 5 || r_index == 6){
      goto back;
    }
  }

  char buff[1024];
  long double val_resp;
  char *str;
  FILE *file;
  int giro = 0;

back_to:
  mean_est_resp     = 0.0;
  stdev_est_resp    = 0.0;
  w_est_resp        = 0.0;
  batch_means_resp  = 0.0;
  interval_est_resp = 0.0;
  if (giro == 0){
    if((file = fopen("CSV/resp_time.csv", "r")) == NULL){
      printf("\n fopen error");
      exit(-1);
    }
  } else {
    if((file = fopen("CSV/bypass.csv", "r")) == NULL){
      printf("\n fopen error");
      exit(-1);
    }
  }

  if(fseek(file, 0L, SEEK_SET) != 0){
    printf("\n fseek error");
    exit(1);
  }

  long double *rip_buff_rest = (long double *)malloc(BATCH_K*sizeof(long double));
  if(rip_buff_rest == NULL){
    printf("\n malloc error");
    exit(1);
  }

  int u = 0;
  while (fgets(buff, 1024, file)){
    char *token = strtok(buff, ",");
    while (token) { 
      val_resp = strtold(token, &str);
      rip_buff_rest[u] = val_resp;
      u++;
      token = strtok(NULL, ",");
    }
  }

  if(fclose(file) != 0){
      printf("fclose error");
      exit(1);
  }

  n = 0;
  for(int f = 0; f < BATCH_K; f++){
    batch_means_resp += rip_buff_rest[f];
    n++;
    diff = rip_buff_rest[f] - mean_est_resp;
    interval_est_resp += diff*diff*(n - 1.0)/n;
    mean_est_resp += diff / n;
  }
  stdev_est_resp = sqrt(interval_est_resp / n);  
  interval_est_func_resp(n);

  batch_means_resp = batch_means_resp/BATCH_K;

  if (giro == 0){
    printf("\n\n The Response Time is: [%Lf]+/-[%Lf]", mean_est_resp, w_est_resp);
    fflush(stdout);
    giro = 1;
    goto back_to;
  }
  printf("\n\n The Police Control Percentage is: [%Lf]+/-[%Lf]\n\n\n", mean_est_resp, w_est_resp);
  fflush(stdout);
  return 0;
}