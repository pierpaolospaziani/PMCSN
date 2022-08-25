
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
#include "Configurations/Finite/config_v1.h"
#endif
#ifdef V2
#include "Configurations/Finite/config_v2.h"
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
bool          batch_state = false;
int           num_batch = 0;
t_batch_stats batch_stats[(int)STOP/F_O_TIME];
int           batch_ne_node_index;
int           batch_ne_event_index;
int           batch_g_numb = 0;
int           batch_phase  = 0;
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

/* permette di salvare le statistiche relative all'esecuzione */
void batch_print(){
  node[3].event[0].t = node[3].event[0].t_last_event;
  if (node[4].event[0].t_last_event > node[4].event[0].t_last_event_d){
    node[4].event[0].t = node[4].event[0].t_last_event;
  } else {
    node[4].event[0].t = node[4].event[0].t_last_event_d;
  }
  long double wait_sum = 0.0;
  long double delay_sum = 0.0;
  for(int i = 0; i < 5; i++){
    if (i == 2){
      long double last_center_event = get_max_last_event(i,true);
      long double utilization_sum = 0.0;

      batch_stats[num_batch].avg_intarr[5]     = node[2].event[0].t / node[2].index_d;
      batch_stats[num_batch].avg_wait[5]       = node[2].area_d     / node[2].index_d;
      batch_stats[num_batch].avg_population[5] = node[2].area_d     / last_center_event;
      if (node[i].index_d != 0){
        wait_sum += (node[i].area_d / arrivals[2]);
      }

      long double service_time = 0.0;
      for (int s = 0; s < max_size[2]; s++){
        node[i].area_d  -= node[2].stat_d[s].service;
        service_time    += node[2].stat_d[s].service;
        utilization_sum += node[2].stat_d[s].service / last_center_event;
      }

      batch_stats[num_batch].avg_service[5] = service_time   / node[2].index_d;
      batch_stats[num_batch].avg_delay[5]   = node[2].area_d / node[2].index_d;
      batch_stats[num_batch].avg_queue[5]   = node[2].area_d / last_center_event;
          
      if (node[i].index_d != 0){
        delay_sum += (node[i].area_d / arrivals[2]);
      }
      
      long double val = (long double) utilization_sum/node[i].num_servers;
      add_utilization_file(5,val);
    } else if (i == 4){
      long double last_center_event = get_max_last_event(i,true);
      long double utilization_sum = 0.0;

      batch_stats[num_batch].avg_intarr[6]     = node[4].event[0].t / node[4].index_d;
      batch_stats[num_batch].avg_wait[6]       = node[4].area_d     / node[4].index_d;
      batch_stats[num_batch].avg_population[6] = node[4].area_d     / last_center_event;
      if (node[i].index_d != 0){
        wait_sum += (node[i].area_d / arrivals[4]);
      }

      long double service_time = 0.0;
      for (int s = 0; s < max_size[4]; s++){
        node[i].area_d  -= node[4].stat_d[s].service;
        service_time    += node[4].stat_d[s].service;
        utilization_sum += node[4].stat_d[s].service / last_center_event;
      }  

      batch_stats[num_batch].avg_service[6] = service_time   / node[4].index_d;
      batch_stats[num_batch].avg_delay[6]   = node[4].area_d / node[4].index_d;
      batch_stats[num_batch].avg_queue[6]   = node[4].area_d / last_center_event;
          
      if (node[i].index_d != 0){
        delay_sum += (node[i].area_d / arrivals[4]);
      }
      
      long double val = (long double) utilization_sum/node[i].num_servers;
      add_utilization_file(6,val);
    }

    long double last_center_event = get_max_last_event(i,false);
    long double utilization_sum = 0.0;

    batch_stats[num_batch].avg_intarr[i]     = node[i].event[0].t / node[i].index;
    batch_stats[num_batch].avg_wait[i]       = node[i].area       / node[i].index;
    batch_stats[num_batch].avg_population[i] = node[i].area       / last_center_event;
    if (node[i].index != 0){
      wait_sum += (node[i].area / arrivals[i]);
    }

    long double service_time = 0.0;
    for (int s = 0; s < max_size[i]; s++){
      node[i].area            -= node[i].stat[s].service;
      service_time            += node[i].stat[s].service;
      utilization_sum         += node[i].stat[s].service / last_center_event;
    }

    batch_stats[num_batch].avg_service[i] = service_time / node[i].index;
    batch_stats[num_batch].avg_delay[i]   = node[i].area / node[i].index;
    batch_stats[num_batch].avg_queue[i]   = node[i].area / last_center_event;
    
    if (node[i].index != 0){
      delay_sum += (node[i].area / arrivals[i]);
    }
    
    long double val = (long double) utilization_sum/node[i].num_servers;
    add_utilization_file(i,val);
  }
  long double val = (long double) arrivals[3]/arrivals[4];
  add_stats_files(wait_sum,delay_sum,val);
}

/* permette di salvare lo stato del sistema */
void save_state(t_center *center){
  batch_ne_node_index  = ne_node_index;
  batch_ne_event_index = ne_event_index;
  batch_g_numb         = g_numb;
  batch_total_arrival  = total_arrival;
  for (int i=0; i<5; i++){
    center[i].num_servers   = node[i].num_servers;
    center[i].index         = node[i].index;
    center[i].area          = node[i].area;
    center[i].index_d       = node[i].index_d;
    center[i].area_d        = node[i].area_d;
    center[i].number        = node[i].number;
    center[i].number_d      = node[i].number_d;
    for(int j = 0; j < max_size[i]+1; j++){
      center[i].event[j].status         = node[i].event[j].status;
      center[i].event[j].is_active      = node[i].event[j].is_active;
      center[i].event[j].t              = node[i].event[j].t;
      center[i].event[j].t_last_event   = node[i].event[j].t_last_event;
      center[i].event[j].t_last_event_d = node[i].event[j].t_last_event_d;
      center[i].event[j].is_ghost       = node[i].event[j].is_ghost;
      center[i].event[j].flag_d         = node[i].event[j].flag_d;
    }
    for(int j = 0; j < max_size[i]; j++){
      center[i].stat[j].service   = node[i].stat[j].service;
      center[i].stat[j].served    = node[i].stat[j].served;
      center[i].stat_d[j].service = node[i].stat_d[j].service;
      center[i].stat_d[j].served  = node[i].stat_d[j].served;
    }
    batch_arrivals[i] = arrivals[i];
    batch_dropped[i]  = dropped[i];
    batch_exits[i]    = exits[i];
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
    center[i].index    = 0;
    center[i].area     = 0.0;
    center[i].index_d  = 0;
    center[i].area_d   = 0.0;
    center[i].number   = 0;
    center[i].number_d = 0;
    arrivals[i]        = 0;
    exits[i]           = 0;
    dropped[i]         = 0;
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
    for(int j = 0; j < max_size[i]; j++){
      center[i].stat[j].service   = 0.0;
      center[i].stat[j].served    = 0;
      center[i].stat_d[j].service   = 0.0;
      center[i].stat_d[j].served    = 0;
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
      new_center_index = old_num_servers + 1;
      while(new_center_index <= node[i].num_servers){
        node[i].event[new_center_index].is_active = ACTIVE;
        if (phase == 2){
          node[i].event[new_center_index].t = FHOUR;
        } else {
          node[i].event[new_center_index].t = SHOUR;
        }
        new_center_index++;
      }
      new_center_index = old_num_servers + 1;
      while(old_queue > 0 && new_center_index <= node[i].num_servers){
        node[i].event[new_center_index].status = BUSY;
        node[i].event[new_center_index].is_ghost = true;
        old_queue--;
        new_center_index++;
      }
    } else if (diff < 0){
      new_center_index = node[i].num_servers + 1;
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
    if (batch_state == false){
      node[dest].event[0].status = BUSY;
    }

    if (node[dest].event[0].t > STOP){
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

/* conta il numero di persone disabili presenti nel sistema*/
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
  long double t_min = (1000*STOP);
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
    
    PlantSeeds(seed);
    t.current = START;
    dest = initial_routing_func();

    node[dest].event[0].t = GetArrival(phase);
    node[dest].event[0].status = BUSY;
    g_numb++;
    int k=0;
    int kesimo_batch = 0;
  label:
    while (control()) {
      k++;
      NextEventNodes();

      if (node[ne_node_index].event[ne_event_index].t > kesimo_batch && kesimo_batch < STOP){
        save_state(batch_node);
        batch_print();
        reset_state();
        kesimo_batch += F_O_TIME;
      }
      
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
  close_files();
  return 0;
}