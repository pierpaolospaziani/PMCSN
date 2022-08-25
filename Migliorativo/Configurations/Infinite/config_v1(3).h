#define PARK_ENTRY     0
#define TICKET_BUY     1
#define DOC_PAT        2
#define POLICE_CONTROL 3
#define TURNSTILES     4

/* number of servers in centers in phase 3 */
#define NUM_SERVER_C0_3  1
#define NUM_SERVER_C1_3  2
#define NUM_SERVER_C2_3  4
#define NUM_SERVER_C3_3  2
#define NUM_SERVER_C4_3  3

#define NUM_SERVER_C0_1  1
#define NUM_SERVER_C1_1  2
#define NUM_SERVER_C2_1  4
#define NUM_SERVER_C3_1  2
#define NUM_SERVER_C4_1  3

#define NUM_SERVER_C0_2  1
#define NUM_SERVER_C1_2  2
#define NUM_SERVER_C2_2  4
#define NUM_SERVER_C3_2  2
#define NUM_SERVER_C4_2  3

#define BATCH_K 256
#define BATCH_B 8192

#define BUSY 1
#define IDLE 0

#define ACTIVE 1
#define NOT_ACTIVE 0

#define START 0.0       	 /* initial (open the door)        */
#define STOP  10800.0    	 /* terminal (close the door) time */

#define P_C0  0.02      	 /* prob routing ingresso con parcheggio 				 */
#define P_C1  0.08	    	 /* prob routing acquisto biglietto 					 */
#define P_C2  0.99			 /* prob routing controllo documenti e pat down 		 */
#define P_CD  0.01			 /* prob routing controllo documenti e pat down disabili */
#define P_SB0 0.01			 /* prob settore sbagliato parcheggio 					 */
#define P_SB1 0.15			 /* prob settore sbagliato biglietti 					 */
#define P_SB2 0.05			 /* prob settore sbagliato doc e pat down				 */
#define P_PD0 0.65			 /* prob quelli del parcheggio sono disabili 			 */
#define P_CP3 0.05			 /* prob controllo polizia fallito 						 */
#define P_BS4 0.0025		 /* prob biglietto sbagliato ai tornelli 				 */

#define LAMBDA3 0.09778
#define LAMBDA1 0.09778
#define LAMBDA2 0.09778

#define AVG_SERVICE_C0 30.0  /* service time ingresso con parcheggio        		 */
#define AVG_SERVICE_C1 90.0  /* service time acquisto biglietto            			 */
#define AVG_SERVICE_C2 20.0  /* service time controllo documenti e pat down			 */
#define AVG_SERVICE_C3 40.0  /* service time controllo polizia a campione   		 */
#define AVG_SERVICE_C4 15.0  /* service time tornelli                       		 */

#define FHOUR 3600.0 		 /* end of the first phase  */
#define SHOUR 7200.0          /* end of the second phase */
#define THOUR 10800.0        /* end of the third phase  */