
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
#include "Configurations/Infinite/config_under(1).h"
#endif
#ifdef V2
#include "Configurations/Infinite/config_ott(1).h"
#endif
#ifdef V3
#include "Configurations/Infinite/config_no_qos(2).h"
#endif
#ifdef V4
#include "Configurations/Infinite/config_ott(2).h"
#endif
#ifdef V5
#include "Configurations/Infinite/config_no_qos(3).h"
#endif
#ifdef V6
#include "Configurations/Infinite/config_ott(3).h"
#endif

t_center node[7];
int ne_node_index;
int ne_event_index;
int g_numb = 0;
int max_size[7];
int total_arrival;
long int arrivals[7];
long int exits[7];
long int dropped[7];

t_center      batch_node[7];
t_center      prev_batch_node[7];
t_center      curr_batch_node[7];
long double   prev_area[7];
long double   *prev_service[7];
long double   prev_last_center_event[7];
bool          batch_state = false;
int           num_batch = 0;
t_batch_stats batch_stats[BATCH_K];
int           batch_ne_node_index;
int           batch_ne_event_index;
int           batch_g_numb = 0;
int           batch_total_arrival;
long int      batch_arrivals[7];
long int      batch_dropped[7];
long int      batch_exits[7];

long double mean_est[7][6]  = {{0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0}};

long double stdev_est[7][6] = {{0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0}};

long double w_est[7][6]     = {{0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0},
                               {0,0,0,0,0,0}};

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
long double get_max_last_event(int center_index){
  long double last_center_event = 0.0;
  for (int j = 0; j < max_size[center_index]+1; j++){
    if (last_center_event < node[center_index].event[j].t_last_event){
      last_center_event = node[center_index].event[j].t_last_event;
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
  for(int ind = 4; ind < 7; ind++){
    node[ind].event[0].t = node[ind].event[0].t_last_event;
  }
  long double wait_sum  = 0.0;
  long double delay_sum = 0.0;
  for(int i = 0; i < 7; i++){

    long double last_center_event = get_max_last_event(i);
    long double utilization_sum = 0.0;

    node[i].area       -= prev_batch_node[i].area;
    node[i].index      -= prev_batch_node[i].index;
    node[i].event[0].t -= prev_batch_node[i].event[0].t;
    if(i == 4 || i == 5 || i == 6){
      node[i].event[0].t -= prev_batch_node[i].event[0].t_last_event;
    }

    batch_stats[num_batch].avg_intarr[i]     = node[i].event[0].t / node[i].index;
    batch_stats[num_batch].avg_wait[i]       = node[i].area       / node[i].index;
    batch_stats[num_batch].avg_population[i] = node[i].area       / (last_center_event - prev_last_center_event[i]);

    if (node[i].index != 0){
      wait_sum += (node[i].area / node[i].index);
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
  long double val = (long double) arrivals[4]/arrivals[5];
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
  for (int i=0; i<7; i++){
    center[i].num_servers = from_center[i].num_servers;
    center[i].index       = from_center[i].index;
    center[i].area        = from_center[i].area;
    center[i].number      = from_center[i].number;
    center[i].number_d    = from_center[i].number_d;
    for(int j = 0; j < max_size[i]+1; j++){
      center[i].event[j].status       = from_center[i].event[j].status;
      center[i].event[j].is_active    = from_center[i].event[j].is_active;
      center[i].event[j].t            = from_center[i].event[j].t;
      center[i].event[j].t_last_event = from_center[i].event[j].t_last_event;
      center[i].event[j].is_ghost     = from_center[i].event[j].is_ghost;
      center[i].event[j].flag_d       = from_center[i].event[j].flag_d;
    }
    for(int j = 0; j < max_size[i]; j++){
      center[i].stat[j].service = from_center[i].stat[j].service;
      center[i].stat[j].served  = from_center[i].stat[j].served;
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
  for (int i=0; i<7; i++){
    node[i].num_servers = batch_node[i].num_servers;
    node[i].index       = batch_node[i].index;
    node[i].area        = batch_node[i].area;
    node[i].number      = batch_node[i].number;
    node[i].number_d    = batch_node[i].number_d;
    for(int j = 0; j < max_size[i]+1; j++){
      node[i].event[j].status       = batch_node[i].event[j].status;
      node[i].event[j].is_active    = batch_node[i].event[j].is_active;
      node[i].event[j].t            = batch_node[i].event[j].t;
      node[i].event[j].t_last_event = batch_node[i].event[j].t_last_event;
      node[i].event[j].is_ghost     = batch_node[i].event[j].is_ghost;
      node[i].event[j].flag_d       = batch_node[i].event[j].flag_d;
    }
    for(int j = 0; j < max_size[i]; j++){
      node[i].stat[j].service = batch_node[i].stat[j].service;
      node[i].stat[j].served  = batch_node[i].stat[j].served;
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
  max_size[5] = NUM_SERVER_C5_1;
  max_size[6] = NUM_SERVER_C6_1;

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
  if (max_size[5] < NUM_SERVER_C5_2){
    max_size[5] = NUM_SERVER_C5_2;
  }
  if (max_size[6] < NUM_SERVER_C6_2){
    max_size[6] = NUM_SERVER_C6_2;
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
  if (max_size[5] < NUM_SERVER_C5_3){
    max_size[5] = NUM_SERVER_C5_3;
  }
  if (max_size[6] < NUM_SERVER_C6_3){
    max_size[6] = NUM_SERVER_C6_3;
  }
}

void initialize_centers(t_center *center){
  node[0].num_servers = NUM_SERVER_C0_1;
  node[1].num_servers = NUM_SERVER_C1_1;
  node[2].num_servers = NUM_SERVER_C2_1;
  node[3].num_servers = NUM_SERVER_C3_1;
  node[4].num_servers = NUM_SERVER_C4_1;
  node[5].num_servers = NUM_SERVER_C5_1;
  node[6].num_servers = NUM_SERVER_C6_1;

  max_num_centers();

  for(int i = 0;i < 7; i++){
    center[i].index           = 0;
    center[i].area            = 0.0;
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
    prev_service[i] = malloc(sizeof(long double)*max_size[i]);
    if (prev_service[i] == NULL){
      printf("malloc error!\n");
      fflush(stdout);
      exit(1);
    }
    for(int j = 0; j < max_size[i]; j++){
      center[i].stat[j].service   = 0.0;
      center[i].stat[j].served    = 0;
      prev_service[i][j]          = 0.0;
    }
    center[i].event = malloc(sizeof(event_list)*(max_size[i]+1));
    if (center[i].event == NULL){
      printf("malloc error!\n");
      fflush(stdout);
      exit(1);
    }
    for(int j = 0; j < max_size[i]+1; j++){
      center[i].event[j].status       = IDLE;
      center[i].event[j].is_active    = NOT_ACTIVE;
      center[i].event[j].t            = START;
      center[i].event[j].t_last_event = 0.0;
      center[i].event[j].is_ghost     = false;
      center[i].event[j].flag_d       = false;
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
    case DOC_PAT_D:
      return (Exponential(AVG_SERVICE_C3));
    case POLICE_CONTROL:
      return (Exponential(AVG_SERVICE_C4));
    case TURNSTILES:
      return (Exponential(AVG_SERVICE_C5));
    default:
      return (Exponential(AVG_SERVICE_C6));
  }
}

/* effettua il routing degli arrivi esterni */
int initial_routing_func() {
  long double random = Uniform(0, 1);
  if (random <= P_C0){
    return PARK_ENTRY;
  }else if (random <= P_C0 + P_C1){
    return TICKET_BUY;
  }else{
    random = Uniform(0, 1);
    if(random <= P_C2){
      return DOC_PAT;
    }else{
      return DOC_PAT_D;
    }
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
    if ((event[i].status == IDLE) && (event[i].t < event[s].t)){
      s = i;
    }
  }
  return (s);
}

/* gestione di un nuovo arrivo in un centro. Se un job non trova coda viene mandato *
 * in servizio invocando la FindOne e successivamente vengono aggiornate le         *
 * statistiche del centro. Se il job, invece, trova coda viene messo in coda        */
void NewArrivalManage(long double current, int phase){
  int dest;
  int s;

  if(ne_node_index == PARK_ENTRY || ne_node_index == TICKET_BUY || ne_node_index == DOC_PAT || ne_node_index == DOC_PAT_D){
    arrivals[ne_node_index]++;
    int busy_not_active_servers = 0;
    for (int i=1; i<max_size[ne_node_index]+1; i++){
      if (node[ne_node_index].event[i].is_active == NOT_ACTIVE && node[ne_node_index].event[i].status == BUSY){
        busy_not_active_servers++;
      }
    }
    total_arrival++;
    batch_state = false;
    if (node[ne_node_index].number <= (node[ne_node_index].num_servers + busy_not_active_servers)) {
      long double service  = GetService(ne_node_index);

      s = FindOne(node[ne_node_index].event,node[ne_node_index].num_servers);

      node[ne_node_index].event[s].t_last_event = node[ne_node_index].event[ne_event_index].t;
      node[ne_node_index].stat[s-1].service += service;
      node[ne_node_index].stat[s-1].served++;
      node[ne_node_index].event[s].t = current + service;
      node[ne_node_index].event[s].status = BUSY;
    }

    node[ne_node_index].event[0].t_last_event = node[ne_node_index].event[ne_event_index].t;

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
    if ((event[i].status == BUSY) && (event[i].t < event[e].t))
      e = i;
  }
  return (e);
}

/* trova il next event del sistema */
void NextEventNodes(){
  int current;
  long double t_min = 100000*STOP;
  for(int i = 0; i < 7; i++){
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
  node[center].area += (node[ne_node_index].event[ne_event_index].t - get_max_last_event(center)) * node[center].number;
  node[center].number++;
  node[center].event[0].t_last_event = node[ne_node_index].event[ne_event_index].t;
  int busy_not_active_servers = 0;
  for (int i=1; i<max_size[center]+1; i++){
    if (node[center].event[i].is_active == NOT_ACTIVE && node[center].event[i].status == BUSY){
      busy_not_active_servers++;
    }
  }
  if(node[center].number <= (node[center].num_servers + busy_not_active_servers)){
    long double service = GetService(center);
    int s = FindOne(node[center].event,node[center].num_servers);
    node[center].event[s].t_last_event = node[ne_node_index].event[ne_event_index].t;
    node[center].stat[s-1].service += service;
    node[center].stat[s-1].served++;
    node[center].event[s].t = node[ne_node_index].event[ne_event_index].t + service;
    node[center].event[s].status = BUSY;
  }
}

/* gestisce le uscite dal centro POLICE CONTROL */
void entry_next_node_police(){
  if(node[POLICE_CONTROL].number >= node[POLICE_CONTROL].num_servers){
    arrivals[5]++;
    entry_next_node(TURNSTILES);
  }else{
    arrivals[4]++;
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
  }else {
    exits[0]++;
    random = Uniform(0, 1);
    if(random <= P_PD0){
      arrivals[6]++;
      entry_next_node(TURNSTILES_D);
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
  }else {
    exits[1]++;
    random = Uniform(0, 1);
    if(random <= P_C3){
      arrivals[3]++;
      entry_next_node(DOC_PAT_D);
    }else{
      arrivals[2]++;
      entry_next_node(DOC_PAT);
    }
  }
}

/* gestisce le uscite dal centro DOC & PAT */
void routing_doc_pat(){
  node[2].index++;
  long double random = Uniform(0, 1);
  if (random <= P_SB2){
    g_numb--;
    dropped[2]++;
  }else {
    exits[2]++;
    entry_next_node_police();
  }
}

/* gestisce le uscite dal centro DOC & PAT D */
void routing_doc_pat_d(){
  node[3].index++;
  long double random = Uniform(0, 1);
  if (random <= P_SB3){
    g_numb--;
    dropped[3]++;
  }else {
    exits[3]++;
    arrivals[6]++;
    entry_next_node(TURNSTILES_D);
  }
}

/* gestisce le uscite dal centro POLICE CONTROL */
void routing_police_control(){
  node[4].index++;
  long double random = Uniform(0, 1);
  if (random <= P_CP4){
    g_numb--;
    dropped[4]++;
  }else {
    exits[4]++;
    arrivals[5]++;
    entry_next_node(TURNSTILES);
  }
}

/* gestisce le uscite dal centro TURNSTILES */
void routing_turnstiles(int from){
  if (from == 5){
    node[5].index++;
  } else {
    node[6].index++;
  }
  long double random = Uniform(0, 1);
  g_numb--;
  if (random <= P_BS5){
    if (from == 5){
      dropped[5]++;
    } else {
      dropped[6]++;
    }
  } else {
    if (from == 5){
      exits[5]++;
    } else {
      exits[6]++;
    }
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
      case DOC_PAT_D:
        routing_doc_pat_d();
        break;
      case POLICE_CONTROL:
        routing_police_control();
        break;
      case TURNSTILES:
        routing_turnstiles(5);
        break;
      case TURNSTILES_D:
        routing_turnstiles(6);
        break;
    }

    int busy_not_active_servers = 0;
    for (int i=1; i<max_size[ne_node_index]+1; i++){
      if (node[ne_node_index].event[i].is_active == NOT_ACTIVE && node[ne_node_index].event[i].status == BUSY){
        busy_not_active_servers++;
      }
    }
    if (node[ne_node_index].number >= (node[ne_node_index].num_servers + busy_not_active_servers) && node[ne_node_index].event[ne_event_index].is_active == ACTIVE) {
      long double service = GetService(ne_node_index);
      node[ne_node_index].stat[ne_event_index-1].service += service;
      node[ne_node_index].stat[ne_event_index-1].served++;
      node[ne_node_index].event[ne_event_index].t = current + service;
    } else {
      node[ne_node_index].event[ne_event_index].status = IDLE;
    }
  }else{
    node[ne_node_index].event[ne_event_index].is_ghost = false;
    long double service = GetService(ne_node_index);
    node[ne_node_index].stat[ne_event_index-1].service += service;
    node[ne_node_index].stat[ne_event_index-1].served++;
    node[ne_node_index].event[ne_event_index].t = current + service;
  }
}

/* controlla se vi sono eventi da processare */
bool control(){
  if(g_numb > 0)
    return true;
  for(int i = 0;i < 7;i++){
    if (node[i].event[0].status != 0)
      return true;
  }
  return false;
}

int main(int argc, char const *argv[]){

  if(argc < 2){
    printf("\n Usage: program <seed>");
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

  long double avg_intarr[7];
  long double avg_wait[7];
  long double avg_population[7];
  long double avg_service[7];
  long double avg_delay[7];
  long double avg_queue[7];

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
      for(int s = 0; s < 7; s++){
        node[s].event[0].status = IDLE;
      }
    }

    k++;
    NextEventNodes();
    t.next = node[ne_node_index].event[ne_event_index].t;
    node[ne_node_index].area += (t.next - get_max_last_event(ne_node_index)) * node[ne_node_index].number;

    t.current = t.next;

    if (ne_event_index == 0) {
      node[ne_node_index].number++;
      g_numb++;
      NewArrivalManage(t.current,phase);
    } else {
      if (!(node[ne_node_index].event[ne_event_index].is_ghost)){
        node[ne_node_index].number--;
      }
      NewCompletionManage(t.current);
    }
    node[ne_node_index].event[ne_event_index].t_last_event = t.current;
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
//
  long double batch_avg_intarr[7]     = {0.0,0.0,0.0,0.0,0.0,0.0};
  long double batch_avg_wait[7]       = {0.0,0.0,0.0,0.0,0.0,0.0};
  long double batch_avg_population[7] = {0.0,0.0,0.0,0.0,0.0,0.0};
  long double batch_avg_service[7]    = {0.0,0.0,0.0,0.0,0.0,0.0};
  long double batch_avg_delay[7]      = {0.0,0.0,0.0,0.0,0.0,0.0};
  long double batch_avg_queue[7]      = {0.0,0.0,0.0,0.0,0.0,0.0};
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
  printf("\n\n AVG_BATCH:");
  fflush(stdout);
  printf("\n Center\tavg_intarr\tavg_wait\tavg_population\tavg_service\tavg_delay\tavg_queue");
  fflush(stdout);
  for (int j=0; j<7; j++){
    printf("\n   %d|\t%Lf\t%Lf\t%Lf\t%Lf\t%Lf\t%Lf",
      j,
      batch_avg_intarr[j]/BATCH_K,
      batch_avg_wait[j]/BATCH_K,
      batch_avg_population[j]/BATCH_K,
      batch_avg_service[j]/BATCH_K,
      batch_avg_delay[j]/BATCH_K,
      batch_avg_queue[j]/BATCH_K);
    fflush(stdout);
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

  long double batch_means[7][6]  = {{0,0,0,0,0,0},
                                    {0,0,0,0,0,0},
                                    {0,0,0,0,0,0},
                                    {0,0,0,0,0,0},
                                    {0,0,0,0,0,0},
                                    {0,0,0,0,0,0},
                                    {0,0,0,0,0,0}};

  long double interval_est[7][6] = {{0,0,0,0,0,0},
                                    {0,0,0,0,0,0},
                                    {0,0,0,0,0,0},
                                    {0,0,0,0,0,0},
                                    {0,0,0,0,0,0},
                                    {0,0,0,0,0,0},
                                    {0,0,0,0,0,0}};

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

  for(int r_index = 0; r_index < 7; r_index++){
    printf("\n ****** the center %d Global statistics are:\n", r_index);
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
  }
  printf("\n\n");
  fflush(stdout);
  return 0;
}