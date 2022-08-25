
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

#include "Configurations/config.h"


t_center node[7];
int ne_node_index;
int ne_event_index;
int dropped_failed_police = 0;
int dropped_invalid_ticket = 0;
int dropped_wrong_sector_0 = 0;
int dropped_wrong_sector_1 = 0;
int dropped_wrong_sector_2 = 0;
int dropped_wrong_sector_3 = 0;
int g_numb = 0;
int max_size[7];

long int arrivals[7];
long int exits[7];
long int dropped[7];

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

void initialize_centers(){
  node[0].num_servers = NUM_SERVER_C0_1;
  node[1].num_servers = NUM_SERVER_C1_1;
  node[2].num_servers = NUM_SERVER_C2_1;
  node[3].num_servers = NUM_SERVER_C3_1;
  node[4].num_servers = NUM_SERVER_C4_1;
  node[5].num_servers = NUM_SERVER_C5_1;
  node[6].num_servers = NUM_SERVER_C6_1;

  max_num_centers();

  for(int i = 0;i < 7; i++){
    node[i].index    = 0;
    node[i].area     = 0.0;
    node[i].number   = 0;
    node[i].number_d = 0;
    arrivals[i]      = 0;
    exits[i]         = 0;
    dropped[i]       = 0;
    node[i].stat = malloc(sizeof(sum)*max_size[i]);
    if (node[i].stat == NULL){
      printf("malloc error!\n");
      fflush(stdout);
      exit(1);
    }
    for(int j = 0; j < max_size[i]; j++){
      node[i].stat[j].service   = 0.0;
      node[i].stat[j].served    = 0;
    }
    node[i].event = malloc(sizeof(event_list)*(max_size[i]+1));
    if (node[i].event == NULL){
      printf("malloc error!\n");
      fflush(stdout);
      exit(1);
    }
    for(int j = 0; j < max_size[i]+1; j++){
      node[i].event[j].status       = IDLE;
      node[i].event[j].is_active    = NOT_ACTIVE;
      node[i].event[j].t            = START;
      node[i].event[j].t_last_event = 0.0;
      node[i].event[j].is_ghost     = false;
      node[i].event[j].flag_d       = false;
    }

    for(int j = 0; j < node[i].num_servers+1; j++){
      node[i].event[j].is_active    = ACTIVE;
    }
  }
}

long double get_max_last_event(int center_index){
  long double last_center_event = 0.0;
  for (int j = 0; j < max_size[center_index]+1; j++){
    if (last_center_event < node[center_index].event[j].t_last_event){
      last_center_event = node[center_index].event[j].t_last_event;
    }
  }
  return last_center_event;
}

long double Exponential(long double m){
  return (-m * log(1.0 - Random()));
}

long double Uniform(long double a, long double b){
  return (a + (b - a) * Random());
}

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
      case 4:
        return NUM_SERVER_C4_2;
      case 5:
        return NUM_SERVER_C5_2;
      case 6:
        return NUM_SERVER_C6_2;
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
      case 4:
        return NUM_SERVER_C4_3;
      case 5:
        return NUM_SERVER_C5_3;
      case 6:
        return NUM_SERVER_C6_3;
    }
  }
  return 0;
}

void switch_phase(int phase){
  int old_num_servers;
  int old_queue;
  int diff;
  int new_center_index;
  for (int i = 0; i < 7; i++){
    old_num_servers = node[i].num_servers;
    old_queue = node[i].number - old_num_servers;
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
    if (node[ne_node_index].number <= (node[ne_node_index].num_servers + busy_not_active_servers)) {
      long double service = GetService(ne_node_index);
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

    if (node[dest].event[0].t > STOP){
      node[dest].event[0].status = IDLE;
      g_numb--;
    }
    // if ((arrivals[0]+dropped[1]+arrivals[2]+arrivals[3]) == BATCH_B){
    //   for (int i=0; i<7; i++){
    //     node[i].event[0].status = IDLE;
    //   }
    //   g_numb--;
    // }
  }
}

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

void NextEventNodes(){
  int current;
  long double t_min = 1000*STOP;
  for(int i = 0; i < 7; i++){
    current = NextEventSingleNode(node[i].event,i);
    if (current == -1){
      continue;
    }
    if(t_min > node[i].event[current].t){
      t_min = node[i].event[current].t;
      ne_node_index = i;
      ne_event_index = current;
    }
  }
}

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

void entry_next_node_police(){
  if(node[POLICE_CONTROL].number >= node[POLICE_CONTROL].num_servers){
    arrivals[5]++;
    entry_next_node(TURNSTILES);
  }else{
    arrivals[4]++;
    entry_next_node(POLICE_CONTROL);
  }
}

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
      printf("\nUsage: program <seed>");
      exit(-1);
    }

    FILE *out_file;

    long seed = atol(argv[1]);
    printf("\n ---> Input seed: %ld\n\n", seed);
    fflush(stdout);

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

    initialize_centers();
    PlantSeeds(seed);
    t.current = START;
    dest = initial_routing_func();

    node[dest].event[0].t = GetArrival(phase);
    node[dest].event[0].status = BUSY;
    g_numb++;
    int k=0;
    while (control()) {
      k++;
      NextEventNodes();

      t.next = node[ne_node_index].event[ne_event_index].t;
      node[ne_node_index].area += (t.next - get_max_last_event(ne_node_index)) * node[ne_node_index].number;

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

    long int sum_served = 0;
    for (int s = 0; s < max_size[ne_node_index]; s++){
      sum_served += node[ne_node_index].stat[s].served;
    }

    if((out_file = fopen("CSV/statistics.csv", "a")) == NULL){
      printf("\n fopen error");
      exit(-1);
    }

    for(int ind = 4; ind < 7; ind++){
      node[ind].event[0].t = node[ind].event[0].t_last_event;
    }

    for(int i = 0; i < 7; i++){

      long double last_center_event = get_max_last_event(i);
      long double utilization_sum = 0.0;

      printf("\nCENTER %d:", i);
      fflush(stdout);

      avg_intarr[i] = node[i].event[0].t / node[i].index;
      avg_wait[i] = node[i].area / node[i].index;
      avg_population[i] = node[i].area / last_center_event;

      long int sum_served = 0;
      for (s = 0; s < max_size[i]; s++){
        sum_served += node[i].stat[s].served;
      }

      printf("\nfor %ld jobs the service node statistics are:\n\n", node[i].index - dropped[i]);
      fflush(stdout);
      printf("  # arrivals ......... = %ld\n", arrivals[i]);
      fflush(stdout);
      printf("  # dropped .......... = %ld\n", dropped[i]);
      fflush(stdout);
      printf("  # exits ............ = %ld\n\n", exits[i]);
      fflush(stdout);
      printf("  avg interarrivals .. = %6.5Lf\n", avg_intarr[i]);
      fflush(stdout);
      printf("  avg # in node ...... = %6.5Lf\n", avg_population[i]);
      fflush(stdout);
      printf("  avg wait ........... = %6.5Lf\n", avg_wait[i]);
      fflush(stdout);

      long double service_time = 0.0;
      for (s = 0; s < max_size[i]; s++){
        node[i].area -= node[i].stat[s].service;
        service_time += node[i].stat[s].service;
        utilization_sum += node[i].stat[s].service / last_center_event;
      }

      avg_service[i] = service_time / node[i].index;
      avg_delay[i] = node[i].area / node[i].index;
      avg_queue[i] = node[i].area / last_center_event;
      
      printf("  avg service ........ = %6.5Lf\n", avg_service[i]);
      fflush(stdout);
      printf("  avg delay .......... = %6.5Lf\n", avg_delay[i]);
      fflush(stdout);
      printf("  avg # in queue ..... = %6.5Lf\n", avg_queue[i]);
      fflush(stdout);
      printf("  avg utilization .... = %6.5Lf\n", utilization_sum/max_size[i]);
      fflush(stdout);

      printf("\n****** the server statistics are:\n\n");
      printf("    server     utilization     avg service        share       served\n");

      long double sum_share = 0.0;

      fflush(stdout);
      for (s = 0; s < max_size[i]; s++){
        sum_share += (long double)node[i].stat[s].served / node[i].index;
        printf("%8d %14.5Lf %15.5Lf %15.5Lf %10ld\n",
          s+1,
          node[i].stat[s].service / last_center_event,
          node[i].stat[s].service / node[i].stat[s].served,
          (long double)node[i].stat[s].served / node[i].index,
          node[i].stat[s].served);
        fflush(stdout);
      }

      if(fprintf(out_file, "%Lf,%Lf,%Lf,%Lf,%Lf,%Lf\n", avg_intarr[i], avg_wait[i], avg_service[i], avg_population[i], avg_delay[i], avg_queue[i]) < 0){
        perror("\nfwite error (1)");
        exit(-1);
      }
      fflush(out_file);
      printf("\n");
      fflush(stdout);
    }

    if(fclose(out_file) != 0){
      printf("\n fclose error");
      exit(-1);
    }
  return 0;
}