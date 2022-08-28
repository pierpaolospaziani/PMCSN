#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./Risorse/inout.c"
#include "./Risorse/rngs.c"


	// gcc -o main Base/main.c -lm
	// gcc -o main_batched Base/main_batched.c -lm
	// gcc -o fo Base/main_fo.c -lm
	// gcc -o fo_simulator Base/fo_simulator.c -lm

	// gcc -o better_call_main Migliorativo/better_call_main.c -lm
	// gcc -o better_call_batched Migliorativo/better_call_batched.c -lm
	// gcc -o fo_better Migliorativo/fo_better.c -lm
	// gcc -o better_fo_simulator Migliorativo/better_fo_simulator.c -lm

#define flush(stdin) while(getchar() != '\n')

bool choice(){
	char c;
reinsert4:
	scanf("%c", &c);
	if(c == 'y' || c == 'Y'){
		flush(stdin);
		return true;
	}else{
		if(c == 'n' || c == 'N'){
			flush(stdin);
			return false;
		}else{
			printf("Could you reinsert your choice please?\n");
			flush(stdin);
			goto reinsert4;
		}
	}
}

void f(char *s){
   printf("**********%*s%*s**********\n",(int)(10+strlen(s)/2),s,(int)(10-strlen(s)/2),"");
}

void print_graph_options(){
	printf("\t\t\tWhich graph do you want to show?\n");
	fflush(stdout);
	printf("\t\t\t1) Response Time Graph\n");
	printf("\t\t\t2) Percentage Police Control Graph\n");
	printf("\t\t\t3) Utilizations Graph\n");
	printf("\t\t\t4) Back\n");
	printf("\n\t\t\t[NOTE: Python Libraries numpy, matplotlib and csv required]\n\n");
}

void print_graph_options_fo_base(){
	printf("\t\t\tWhich configuration do you want to use?\n");
	fflush(stdout);
	printf("\t\t\t1) Not Stationary\n");
	printf("\t\t\t2) Only 1° Phase Stationary\n");
	printf("\t\t\t3) Adjust 2° Phase\n");
	printf("\t\t\t4) Optimal Configuration\n");
	printf("\t\t\t5) Back\n\n");
}

char* flag_setup_fo_base(){
	char char1[3] = "V1\0";
   char char2[3] = "V2\0";
	char char3[3] = "V3\0";
   char char4[3] = "V4\0";
	char *buffer;
	char op;
	char options[5] = {'1','2','3','4','5'};
rep:
	print_graph_options_fo_base();
	op = multiChoice("Select an option", options, 5, 0);
	buffer = malloc(1024*sizeof(char));
	if (buffer == NULL){
		printf("malloc error!\n");
		exit(-1);
	}
	memset(buffer, 0, 1024);
	switch(op){
		case '1':
			strncpy(buffer,(const char*)&char1,1024);
			return buffer;
		case '2':
			strncpy(buffer,(const char*)&char2,1024);
			return buffer;
		case '3':
			strncpy(buffer,(const char*)&char3,1024);
			return buffer;
		case '4':
			strncpy(buffer,(const char*)&char4,1024);
			return buffer;
		case '5':
			return "";
		default:
			printf(" \nCould you reinsert your choice please?\n");
			fflush(stdout);
			goto rep;
	}
	return buffer;
}

void print_graph_options_io_base(){
	printf("\t\t\tWhich configuration do you want to use?\n");
	fflush(stdout);
	printf("\t\t\t1) Under-provisioned     - 1° Phase\n");
	printf("\t\t\t2) Optimal Configuration - 1° Phase\n");
	printf("\t\t\t3) Stationary but NO QoS - 2° Phase\n");
	printf("\t\t\t4) Optimal Configuration - 2° Phase\n");
	printf("\t\t\t5) Stationary but NO QoS - 3° Phase\n");
	printf("\t\t\t6) Optimal Configuration - 3° Phase\n");
	printf("\t\t\t7) Back\n\n");
}

char* flag_setup_io_base(){
	char char1[3] = "V1\0";
   char char2[3] = "V2\0";
	char char3[3] = "V3\0";
   char char4[3] = "V4\0";
	char char5[3] = "V5\0";
   char char6[3] = "V6\0";
	char *buffer;
	char op;
	char options[7] = {'1','2','3','4','5','6','7'};
rep:
	print_graph_options_io_base();
	op = multiChoice("Select an option", options, 7, 0);
	buffer = malloc(1024*sizeof(char));
	if (buffer == NULL){
		printf("malloc error!\n");
		exit(-1);
	}
	memset(buffer, 0, 1024);
	printf("\n");
	fflush(stdout);
	switch(op){
		case '1':
			strncpy(buffer,(const char*)&char1,1024);
			return buffer;
		case '2':
			strncpy(buffer,(const char*)&char2,1024);
			return buffer;
		case '3':
			strncpy(buffer,(const char*)&char3,1024);
			return buffer;
		case '4':
			strncpy(buffer,(const char*)&char4,1024);
			return buffer;
		case '5':
			strncpy(buffer,(const char*)&char5,1024);
			return buffer;
		case '6':
			strncpy(buffer,(const char*)&char6,1024);
			return buffer;
		case '7':
			return "";
		default:
			printf(" \nCould you reinsert your choice please?\n");
			fflush(stdout);
			goto rep;
	}
	return buffer;
}

void print_graph_options_fo_better(){
	printf("\n\t\t\tWhich configuration do you want to use?\n");
	fflush(stdout);
	printf("\t\t\t1) Same as server number as the Base\n");
	printf("\t\t\t2) Less server number than the Base\n");
	printf("\t\t\t3) Back\n\n");
}

char* flag_setup_fo_better(){
	char char1[3] = "V1\0";
   char char2[3] = "V2\0";
	char *buffer;
	char op;
	char options[3] = {'1','2','3'};
rep:
	print_graph_options_fo_better();
	op = multiChoice("Select an option", options, 3, 0);
	buffer = malloc(1024*sizeof(char));
	if (buffer == NULL){
		printf("malloc error!\n");
		exit(-1);
	}
	memset(buffer, 0, 1024);
	switch(op){
		case '1':
			strncpy(buffer,(const char*)&char1,1024);
			return buffer;
		case '2':
			strncpy(buffer,(const char*)&char2,1024);
			return buffer;
		case '3':
			return "";
		default:
			printf(" \nCould you reinsert your choice please?\n");
			fflush(stdout);
			goto rep;
	}
	return buffer;
}

void print_graph_options_io_better(){
	printf("\t\t\tWhich configuration do you want to use?\n");
	fflush(stdout);
	printf("\t\t\t1) Same as server number as the Base - 1° Phase\n");
	printf("\t\t\t2) Same as server number as the Base - 2° Phase\n");
	printf("\t\t\t3) Same as server number as the Base - 3° Phase\n");
	printf("\t\t\t4) Less server number than the Base  - 1° Phase\n");
	printf("\t\t\t5) Less server number than the Base  - 2° Phase\n");
	printf("\t\t\t6) Less server number than the Base  - 3° Phase\n");
	printf("\t\t\t7) Back\n\n");
}

char* flag_setup_io_better(){
	char char1[3] = "V1\0";
   char char2[3] = "V2\0";
	char char3[3] = "V3\0";
   char char4[3] = "V4\0";
	char char5[3] = "V5\0";
   char char6[3] = "V6\0";
	char *buffer;
	char op;
	char options[7] = {'1','2','3','4','5','6','7'};
rep:
	print_graph_options_io_better();
	op = multiChoice("Select an option", options, 7, 0);
	buffer = malloc(1024*sizeof(char));
	if (buffer == NULL){
		printf("malloc error!\n");
		exit(-1);
	}
	memset(buffer, 0, 1024);
	printf("\n");
	fflush(stdout);
	switch(op){
		case '1':
			strncpy(buffer,(const char*)&char1,1024);
			return buffer;
		case '2':
			strncpy(buffer,(const char*)&char2,1024);
			return buffer;
		case '3':
			strncpy(buffer,(const char*)&char3,1024);
			return buffer;
		case '4':
			strncpy(buffer,(const char*)&char4,1024);
			return buffer;
		case '5':
			strncpy(buffer,(const char*)&char5,1024);
			return buffer;
		case '6':
			strncpy(buffer,(const char*)&char6,1024);
			return buffer;
		case '7':
			return "";
		default:
			printf(" \nCould you reinsert your choice please?\n");
			fflush(stdout);
			goto rep;
	}
	return buffer;
}

void call_func(int value){
	char buffer[1024];
	char gcc_buffer[1024];
	char op;
	char options[4] = {'1','2','3','4'};
	char* flag;

	long double seed;

	printf("\nPlease insert seed: ");
	scanf("%Lf",&seed);
	flush(stdin);
	PlantSeeds(seed);
	SelectStream(9);
	printf("\n");
	fflush(stdout);

	memset(buffer, 0, 1024);
	memset(gcc_buffer, 0, 1024);

	switch(value) {
		case 1:
			flag = flag_setup_fo_base();
			if (strncmp(flag,"",1) != 0){
				snprintf(gcc_buffer, sizeof(gcc_buffer), "gcc -o fo Base/main_fo.c -lm -D%s", flag);
				system(gcc_buffer);
				memset(gcc_buffer, 0, 1024);
				snprintf(gcc_buffer, sizeof(gcc_buffer), "gcc -o fo_simulator Base/fo_simulator.c -lm -D%s", flag);
				system(gcc_buffer);
				snprintf(buffer, sizeof(buffer), "./fo_simulator %Lf", seed);
			}
			break;
		case 2:
			flag = flag_setup_io_base();
			if (strncmp(flag,"",1) != 0){
				snprintf(gcc_buffer, sizeof(gcc_buffer), "gcc -o main_batched Base/main_batched.c -lm -D%s", flag);
				system(gcc_buffer);
				snprintf(buffer, sizeof(buffer), "./main_batched %Lf", seed);
			}
			break;
		case 3:
			flag = flag_setup_fo_better();
			if (strncmp(flag,"",1) != 0){
				snprintf(gcc_buffer, sizeof(gcc_buffer), "gcc -o fo_better Migliorativo/fo_better.c -lm -D%s", flag);
				system(gcc_buffer);
				memset(gcc_buffer, 0, 1024);
				snprintf(gcc_buffer, sizeof(gcc_buffer), "gcc -o better_fo_simulator Migliorativo/better_fo_simulator.c -lm -D%s", flag);
				system(gcc_buffer);
				snprintf(buffer, sizeof(buffer), "./better_fo_simulator %Lf", seed);
			}
			break;
		case 4:
			flag = flag_setup_io_better();
			if (strncmp(flag,"",1) != 0){
				snprintf(gcc_buffer, sizeof(gcc_buffer), "gcc -o better_call_batched Migliorativo/better_call_batched.c -lm -D%s", flag);
				system(gcc_buffer);
				snprintf(buffer, sizeof(buffer), "./better_call_batched %Lf", seed);
			}
			break;
		case 5:
			snprintf(gcc_buffer, sizeof(gcc_buffer), "gcc -o main Base/main.c -lm");
			system(gcc_buffer);
			snprintf(buffer, sizeof(buffer), "./main %Lf", seed);
			break;
		case 6:
			snprintf(gcc_buffer, sizeof(gcc_buffer), "gcc -o better_call_main Migliorativo/better_call_main.c -lm");
			system(gcc_buffer);
			snprintf(buffer, sizeof(buffer), "./better_call_main %Lf", seed);
			break;
	}

	system(buffer);

	memset(buffer, 0, 1024);
label:
	if (value == 1 && strncmp(flag,"",1) != 0 || value == 3 && strncmp(flag,"",1) != 0){
		// serve menu multichoce da 4 che, finita la run, mi chiede quale dei 3 grafici voglio
		// se chiudo il grafo, mi chiede se voglio vedere un altro grafo o tornare indietro
		//resp time pol control utilization exit

		print_graph_options();
		op = multiChoice("Select an option", options, 4, 0);

		switch(op){
			case '1':
				if(value == 3)
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/grafo.py 0 d");
				else
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/grafo.py 0");
				system(buffer);
				goto label;
			case '2':
				if(value == 3)
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/grafo.py 1 d");
				else
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/grafo.py 1");
				system(buffer);
				goto label;
			case '3':
				if(value == 3)
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/grafo.py 2 d");
				else
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/grafo.py 2");
				system(buffer);
				goto label;
			default:
				break;
		}
	}else if(value == 2 && strncmp(flag,"",1) != 0 || value == 4 && strncmp(flag,"",1) != 0){

		print_graph_options();
		op = multiChoice("Select an option", options, 4, 0);

		switch(op){
			case '1':
				if(value == 4)
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/batch_graph.py 0 d");
				else
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/batch_graph.py 0");
				system(buffer);
				goto label;
			case '2':
				if(value == 4)
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/batch_graph.py 1 d");
				else
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/batch_graph.py 1");
				system(buffer);
				goto label;
			case '3':
				if(value == 4)
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/batch_graph.py 2 d");
				else
					snprintf(buffer, sizeof(buffer), "python3 Python_Scripts/batch_graph.py 2");
				system(buffer);
				goto label;
			default:
				break;
		}
	}
}



void base_menu(){

	char options[4] = {'a','b', 'c', 'd'};
	char op;

	printf("\033[2J\033[H");
	f("\t\t\tSIMULATION CONSOLE\t\t\t");
	printf("\n\t\t\t1) Base Simulation\n\n");
	printf("\t\t\t   -> 1.a) Base Finite Horizon\n");
	printf("\t\t\t   -> 1.b) Base Infinite Horizon\n");
	printf("\t\t\t   -> 1.c) Single Base Execution\n");
	printf("\t\t\t   -> 1.d) Back to Main Menu\n\n");
	printf("\t\t\t2) Improved Simulation\n");
	printf("\t\t\t3) Back\n\n");

reinsert_1:
	op = multiChoice("Select an option", options, 4, 1);
	switch(op) {
		case 'a':
			call_func(1);
			break;
		case 'b':
			call_func(2);
			break;
		case 'c':
			call_func(5);
			break;
		case 'd':
			break;
		default:
			printf(" Could you reinsert your choice please?\n");
			fflush(stdout);
			goto reinsert_1;
	}
}


void improved_menu(){

	char options[4] = {'a','b', 'c','d'};
	char op;

	long double seed;

	char buffer[1024];

	printf("\033[2J\033[H");
	f("\t\t\tSIMULATION CONSOLE\t\t\t");
	printf("\n\t\t\t1) Base Simulation\n");
	printf("\t\t\t2) Improved Simulation\n\n");
	printf("\t\t\t  -> 2.a) Improved Finite Horizon\n");
	printf("\t\t\t  -> 2.b) Improved Infinite Horizon\n");
	printf("\t\t\t  -> 2.c) Single Improved Execution\n");
	printf("\t\t\t  -> 2.d) Back to Main Menu\n\n");
	printf("\t\t\t3) Back\n\n");

reinsert:
	op = multiChoice("Select an option", options, 4, 2);
	switch(op) {
		case 'a':
			call_func(3);
			break;
		case 'b':
			call_func(4);
			break;
		case 'c':
			call_func(6);
			break;
		case 'd':
			break;
		default:
			printf(" Could you reinsert your choice please?\n");
			fflush(stdout);
			goto reinsert;
	}
}



int main(int argc, char const *argv[])
{
	char options[3] = {'1','2', '3'};
	char op;

	while(true) {
		printf("\033[2J\033[H");
		f("\t\t\tSIMULATION CONSOLE\t\t\t");
		printf("\n\t\t\t1) Base Simulation\n");
		printf("\t\t\t2) Improved Simulation\n");
		printf("\t\t\t3) Quit\n\n");
		
reinsert_main:
		op = multiChoice("Select an option", options, 3, 0);
		switch(op) {
			case '1':
				base_menu();
				break;
			case '2':
				improved_menu();
				break;
			case '3':
				printf("\nBye!\n");
				fflush(stdout);
				return 0;
				break;
			default:
				printf(" \nCould you reinsert your choice please?\n");
				fflush(stdout);
				goto reinsert_main;
		}

		printf("Do you want to continue operations? [y/n]\n");
		if(choice())
			continue;
		else
			return 0;
	}
}
