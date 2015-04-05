#include   <semaphore.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <pthread.h>

#define SLOTSIZE 18
#define SLOTCNT 9
char buffer[SLOTCNT][SLOTSIZE];
//file I/O
FILE *infile;
FILE *outfile;


sem_t buf_lock; //=1
sem_t slot_avail; //=n
sem_t item_avail; // =0

int flag = 0; //indicate when the producing/consuming process ends
int slot = 0;//next available slot for producer
int item_buff=0; //number items in buffer
//int num_bytes; //number of bytes in slot

//create threads
pthread_t produce;
pthread_t consume;

//thread methods
void *producer();

void *consumer();



int main(int argc, char *argv[]){

	

	
	//create semaphores
	sem_init(&buf_lock, 0, 1);
	sem_init(&slot_avail, 0, 9);
	sem_init(&item_avail, 0, 0);
	

	//check for to file inputs
	if(argc != 3)
	{
		printf("Correct Usage: prog3 infile outfile \n");
		exit(0);
	}
	else{
		//argv[1] is name of infile  
		infile = fopen(argv[1], "r"); //read from infile 
		outfile = fopen(argv[2], "w");//write to outfile
		if( infile == 0 )
		{
			printf("Could not open file\n");
		}
		else{
				
			memset(buffer, 0, sizeof buffer);		
			
			//create threads with individual functions
			pthread_create(&produce, NULL,  producer,(void *) infile);
			pthread_create(&consume, NULL,  consumer,(void *) outfile);
			//wait for threads to end
			pthread_join(produce, NULL);
			pthread_join(consume, NULL);	
			//close files
			fclose(infile);
			fclose(outfile);
		}
	}
	
	pthread_exit(0);
}


void *producer(FILE *infile){
	int c;
	int i;
	int j;
	int index = 0;
	int next_produced[18];
	int end_producer = 0;//flag
	
	do{
		/*produce an time in next_produced*/

			sem_wait(&slot_avail);
		
			sem_wait(&buf_lock);
			//check if available with slot_avail
			/*add next_produced to the buffer*/
			for(i = 0; i < SLOTSIZE; i++){

				//check for EOF
				if((c = fgetc(infile)) != EOF){
					
					next_produced[i] = c;
					
				}
				else{
					//no more bytes in infile
					end_producer = 1;
				}
			}
			for(j = 0; j < SLOTSIZE; j++){ 
				buffer[index][j] = (char)next_produced[j];
			}
			
			index++;
			index = index % SLOTCNT;
			slot++;
			
			memset(next_produced, 0, sizeof next_produced);
			
			sem_post(&buf_lock);
			sem_post(&item_avail);
		
		if(end_producer){ 
			//tell consumer producer is done
			flag = end_producer;
			break;
		}
		
	}while(1);
	
	pthread_exit(0);
}



void *consumer(FILE *outfile){
	int s;
	int r;
	
	char next_consumed[18];
    do{
	
			sem_wait(&item_avail);
			sem_wait(&buf_lock);
			/*check if item avialable with item_avail
			/*remove an item fro  buffer to next_consumed*/
		
			slot--;//because slot is getting consumed
			
			//iterate through buffer
			for(r=0; r<SLOTSIZE; r++){
				next_consumed[r] = buffer[item_buff][r];
			}
			
			/*consume the item in next_consumed*/
			for(r=0; r<SLOTSIZE;r++){
					
					fputc(next_consumed[r],outfile);
			}
			memset(next_consumed, 0, sizeof next_consumed);
			item_buff++;
			item_buff= item_buff % SLOTCNT;
	
			sem_post(&buf_lock);
			sem_post(&slot_avail);
			

		if(flag && slot == 0){ //producer done and buffer empty

			break;
		}
			
		
	}while(1);

	pthread_exit(0);
}
