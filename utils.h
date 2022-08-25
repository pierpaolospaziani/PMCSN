#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct stat_sum_count{			/* accumulated sums of                */
    long double service;                /* service times                      */
    long int served;                   	/* number served nel singolo server   */
}sum;

typedef struct event{
	bool status;						/* IDLE = 0; BUSY= 1 				  */
	bool is_active;
	long double t;						/* next event time	 				  */
	long double t_last_event;			/* ultimo evento accaduto nel nodo	  */
	long double t_last_event_d;			/* ultimo evento accaduto nel nodo	  */
	bool is_ghost;						/* flag di riconoscimento switch	  */
	bool flag_d;						/* flag di riconoscimento disabili	  */
}event_list;

typedef struct center_s{
	int num_servers;					/* number of servers           		  */
	long int index;             		/* used to count processed jobs  	  */
	long int index_d;             		/* used to count processed jobs  	  */
	long double area;            		/* time integrated number in the node */
	long double area_d;            		/* time integrated number in the node */
	int number;							/* number of jobs in the system 	  */
	int number_d;					    /* number of d_job in the system 	  */
	event_list *event;  				/* event list of a center      		  */
	sum *stat;							/* pointer to get execution infos	  */
	sum *stat_d;							/* pointer to get execution infos	  */
}t_center;

typedef struct batch_stats{
	long double avg_intarr[7];
    long double avg_wait[7];
    long double avg_population[7];
    long double avg_service[7];
    long double avg_delay[7];
    long double avg_queue[7];
}t_batch_stats;