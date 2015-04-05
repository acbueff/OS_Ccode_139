#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>

int main(int argc, char *argv[]){
	
	//for process identification
	pid_t MC0;
	pid_t MC1;
	pid_t MC2;
	pid_t MC3;
	pid_t display;
	
	//for piping 
	int p0[2];
	int p1[2];
	int p2[2];
	int p3[2];
	
	//to parse argv and arrays storing digits
	int a = 0;
	int b = 0;
	int j = 0;
	int r = 0;
	int s = 0;
	int y;
	int x;//value determines size of temporary string to store passed integer
	
	int status;
	
	//value to store values from matrix operation
	int c0=0;
	int c1=0;
	int c2=0;
	int c3=0; 
	//store digit values into respective arrays
	int top_row_A[4];
	int bottom_row_A[4];
	int right_column_B[4];
	int left_column_B[4];
	
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
			
			//create pipes
			pipe(p0);
			pipe(p1);
			pipe(p2);
			pipe(p3);
			
			printf("Message before MC0 child process created \n");
			if((MC0 = fork()) == 0){//begin forking
				
				printf("Value of MC0 pid: %d \n", getpid());
				printf("Value of MC0 ppid: %d \n", getppid());
				close(p0[0]);//close read end of pipe
				
				//compute c0
				for( r = 0; r < 4; r++){
					c0 = c0 + top_row_A[r]*left_column_B[r];
				}
				
				
				//write to pipe value of c0
				write(p0[1],(void*) &c0, sizeof(c0));
				printf("value of c0 is %d \n", c0);
				close(p0[1]);//close write end of pipe
				printf("Message before MC0 terminates \n");
				exit(0);
			}
			printf("Message before MC1 child process created \n");
			if((MC1 = fork()) == 0){
				
				printf("Value of MC1 pid: %d \n", getpid());
				printf("Value of MC1 ppid: %d \n", getppid());
				
				close(p1[0]);//close read end of pipe
				
				//compute c0
				for( r = 0; r < 4; r++){
		
					c1 = c1 + top_row_A[r]*right_column_B[r];
				}
				
				
				
				write(p1[1],(void*) &c1, sizeof(c1));
				printf("value of c1 is %d \n", c1);
				close(p1[1]);
				printf("Message before MC1 terminates \n");
				exit(0);
			}
			printf("Message before MC2 child process created \n");
			if((MC2 = fork()) == 0){
				
				printf("Value of MC2 pid: %d \n", getpid());
				printf("Value of MC2 ppid: %d \n", getppid());
				
				close(p2[0]);//close read end of pipe
				
				//compute c0
				for( r = 0; r < 4; r++){
					c2 = c2 + bottom_row_A[r]*left_column_B[r];
				}
				
				
				
				write(p2[1],(void*) &c2, sizeof(c2));
				printf("value of c2 is %d \n", c2);
				close(p2[1]);
				printf("Message before MC2 terminates \n");
				exit(0);
			}
			printf("Message before MC3 child process created \n");
			if((MC3 = fork()) == 0){
				
				printf("Value of MC3 pid: %d \n", getpid());
				printf("Value of MC3 ppid: %d \n", getppid());
				
				close(p3[0]);//close read end of pipe
				
				//compute c0
				for( r = 0; r < 4; r++){
					c3 = c3 + bottom_row_A[r]*right_column_B[r];
				}
				
				
				
				write(p3[1],(void*) &c3, sizeof(c3));
				printf("value of c3 is %d \n", c3);
				close(p3[1]);
				printf("Message before MC3 terminates \n");
				exit(0);;
			}
			
			printf("Message before MC3 child process created \n");
			if((display =fork()) == 0){
				printf("Value of display pid: %d \n", getpid());
				printf("Value of display ppid: %d \n", getppid());
				//close write ends
				close(p0[1]);
				close(p1[1]);
				close(p2[1]);
				close(p3[1]);
				//values to store write output
				int val1;
				int val2;
				int val3;
				int val4;
				//read values to local variables
				read(p0[0],(void*)&val1, sizeof(c0));
				
				close(p0[0]);//close read end
				
				read(p1[0],(void*)&val2, sizeof(c1));
				
				close(p1[0]);
				
				read(p2[0],(void*)&val3, sizeof(c2));
				
				close(p2[0]);
				
				read(p3[0],(void*)&val4, sizeof(c3));
				
				close(p3[0]);
				
				printf("The first row of the resulting matrix is:	%d	%d \n", val1, val2);
				printf("The second row of the resulting matrix is:	%d	%d \n", val3, val4);
				printf("message before display exits \n");
				exit(0);
			}
			
			//wait for child processes to finish
			printf("Message before issuing waitpid \n");
			waitpid(MC0,&status,0);
			waitpid(MC1,&status,0);
			waitpid(MC2,&status,0);
			waitpid(MC3,&status,0);
			waitpid(display,&status,0);
			
			printf("Message before parent exits \n");
			exit(0);
	
			
	
	
	}
	else{
		printf("Incorrect number of inputs \n");
	}


	return(0);


}