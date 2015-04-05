#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <pthread.h>




	int c0=0;
	int c1=0;
	int c2=0;
	int c3=0; 
	int top_row_A[4];
	int bottom_row_A[4];
	int right_column_B[4];
	int left_column_B[4];
	
	//to parse argv and arrays storing digits
	int a = 0;
	int b = 0;
	int j = 0;
	int r = 0;
	int s = 0;
	int y;
	int x;//value determines size of temporary string to store passed integer
	
	
	
	//mutex
	pthread_mutex_t myLock1 =PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t myLock2 =PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t myLock3 =PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t myLock4 =PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t myLock5 =PTHREAD_MUTEX_INITIALIZER;


void *MC0(){	pthread_mutex_lock(&myLock1);
				printf("Value of display pthreadid: %d \n", (unsigned int)pthread_self());

				//compute c0
				for( r = 0; r < 4; r++){
				
					c0 = c0 + top_row_A[r]*left_column_B[r];
				
				}
				pthread_mutex_unlock(&myLock1);
				
				
				
				printf("value of c0 is %d \n", c0);
				
				printf("Message before MC0 terminates \n");

				pthread_exit(0);
}

void *MC1(){	pthread_mutex_lock(&myLock2);
				printf("Value of display pthreadid: %d \n", (unsigned int)pthread_self());

				//compute c0
				for( r = 0; r < 4; r++){
				
					c1 = c1 + top_row_A[r]*right_column_B[r];
				
				}
				pthread_mutex_unlock(&myLock2);
				
				
				
				printf("value of c1 is %d \n", c1);
				
				printf("Message before MC1 terminates \n");
				pthread_exit(0);
}

void *MC2(){	pthread_mutex_lock(&myLock3);
				printf("Value of display pthreadid: %d \n", (unsigned int)pthread_self());

				//compute c0
				for( r = 0; r < 4; r++){
				
					c2 = c2 + bottom_row_A[r]*left_column_B[r];
				
				}
				pthread_mutex_unlock(&myLock3);
				
				
				
				printf("value of c2 is %d \n", c2);
				
				printf("Message before MC2 terminates \n");
				pthread_exit(0);
}

void *MC3(){
				pthread_mutex_lock(&myLock4);
				printf("Value of display pthreadid: %d \n", (unsigned int)pthread_self());

				//compute c0
				for( r = 0; r < 4; r++){
				
					c3 = c3 + bottom_row_A[r]*right_column_B[r];
				
				}
				pthread_mutex_unlock(&myLock4);
				
				
				
				printf("value of c3 is %d \n", c3);
				
				printf("Message before MC3 terminates \n");
				pthread_exit(0);
}

void *display(){
				
				pthread_mutex_lock(&myLock5);
				printf("Value of display pthreadid: %d \n", (unsigned int)pthread_self());
				
				printf("The first row of the resulting matrix is:	%d	%d \n", c0, c1);
				printf("The second row of the resulting matrix is:	%d	%d \n", c2, c3);
				pthread_mutex_unlock(&myLock5);
				printf("message before display exits \n");
				pthread_exit(0);
}

int main(int argc, char *argv[]){
	
	if(pthread_mutex_init(&myLock1, NULL) !=0){
		printf("Mutex myLocke1 failed \n");
		return 1;
	}
	if(pthread_mutex_init(&myLock2, NULL) !=0){
		printf("Mutex myLocke2 failed \n");
		return 1;
	}
	if(pthread_mutex_init(&myLock3, NULL) !=0){
		printf("Mutex myLocke3 failed \n");
		return 1;
	}
	if(pthread_mutex_init(&myLock4, NULL) !=0){
		printf("Mutex myLocke4 failed \n");
		return 1;
	}
	if(pthread_mutex_init(&myLock5, NULL) !=0){
		printf("Mutex myLocke5 failed \n");
		return 1;
	}
	
	//create threads
	pthread_t myThread1;
	pthread_t myThread2;
	pthread_t myThread3;
	pthread_t myThread4;
	pthread_t myThread5;

	
	//value to store values from matrix operation
	
	//store digit values into respective arrays
	
	
	//check that 16 values where inputed
	if(argc == 17){
			 
			//check if values are digits
			for(a = 1; a < argc; a++) { 
				//recheck cases with '+' and '-' in front of digits
				if(isdigit(*argv[a])){ 
					y = strlen(argv[a]);
				
					for(b = 0; b < y; b++){
						
						if(isdigit(*argv[a])){
						}
						else if(isdigit(argv[a][b])){
						
						}
						else{
							printf("Not a digit. \n");
							exit(0);
						}
					}
				} 
				else if(*argv[a] == '+'){
					y = strlen(argv[a]);
					if( y < 2){
						printf("Not a digit! \n");
						exit(0);
					}
					for(b = 1; b < y; b++){
	
						if(isdigit(argv[a][b])){
						}
						else{
							printf("Not a digit. \n");
							exit(0);
						}
					}
				}
				else if(*argv[a] == '-'){	
					y = strlen(argv[a]);
					if( y < 2){
						printf("Not a digit! \n");
						exit(0);
					}
					for(b = 1; b < y; b++){
				
						if(isdigit(argv[a][b])){
						
						}
						else{
							printf("Not a digit. \n");
							exit(0);
						}
					}
				}
				else{
				
					printf("Not a digit. \n");
					
					exit(0);
				}
					
			}
			//parse first 4 values and store in array
			for(j = 1; j < 5; j++){
				x = strlen(argv[j]);//get length of string and store into x
				char temp[x+1];//create char array based string length
				
				strcpy(temp, argv[j]);//convert digit string to temp
				
				top_row_A[j-1] = atoi(temp);//convert string to an int value
				
							
				
			}
			//parse next 4 values and store in array
			for(j = 5; j < 9; j++){
				x = strlen(argv[j]);
				char temp[x+1];
				
				strcpy(temp, argv[j]);
				
				bottom_row_A[j-5] = atoi(temp);
				
							
				
			}
			
			//for column B-left
			for(j = 9; j < 17; j++){
				x = strlen(argv[j]);
				char temp[x+1];
				strcpy(temp, argv[j]);
				
				if(atoi(temp)%2 != 0){//pick values with odd index to create left column
					left_column_B[r] = atoi(temp);
					r++;
				}
				
			}
			//parse values for B-right
			for(j = 9; j < 17; j++){
				x = strlen(argv[j]);
				char temp[x+1];
				strcpy(temp, argv[j]);
				
				if(atoi(temp)%2 == 0){//pick values with even index to create right column
					right_column_B[s] = atoi(temp);
					s++;
				}
				
			}
			
			printf("Message before MC0 thread process created \n");
			//create threads
			
			pthread_create(&myThread1, NULL, (void *) MC0, NULL);
			printf("Message before MC1 thread process created \n");
			pthread_create(&myThread2, NULL, (void *) MC1, NULL);
			
			printf("Message before MC2 thread process created \n");
			pthread_create(&myThread3, NULL, (void *) MC2, NULL);
			
			printf("Message before MC3 thread process created \n");
			pthread_create(&myThread4, NULL, (void *) MC3, NULL);
			
			printf("Message before display thread process created \n");
			pthread_create(&myThread5, NULL, (void *) display, NULL);
	
			//wait for thread processes to finish
			
			printf("message before pthread_join \n");
			pthread_join(myThread1, NULL);
			pthread_join(myThread2, NULL);
			pthread_join(myThread3, NULL);
			pthread_join(myThread4, NULL);
			pthread_join(myThread5, NULL);
			
			
			pthread_mutex_destroy(&myLock1);
			pthread_mutex_destroy(&myLock2);
			pthread_mutex_destroy(&myLock3);
			pthread_mutex_destroy(&myLock4);
			pthread_mutex_destroy(&myLock5);
			printf("Message before parent exits \n");
			pthread_exit(0);
	
			
	
	
	}
	else{
		printf("Incorrect number of inputs \n");
	}

	
	

	return(0);


}