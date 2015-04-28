#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
struct stat buf;
char *mm_file;
int file;
int main(int argc, char *argv[]){
char data[] = "with the mmap call, a disk file gets mapped into memory through demand\n paging, i.e., a page sized portion of the file is initially read into\n memory through a page fault. subsequent reads and writes to that portion\n of the file are handled as routine memory accesses. this improves file\n processing performance. in this assignment, the parent process memory maps\n this disk file first, and then creates two child processes that each make\n some changes to this file. when both child processes are done, changes\n made in memory are synch'ed to the disk file.\n";
//for process identification
	pid_t child1;
	pid_t child2;
	int status;
	int size;
	size = strlen(data);
	printf("size of data is: %d\n", size);
//check for file
if(argc != 2){
	printf("No File brah\n");
	exit(0);
}

if((file = open(argv[1],O_RDWR|O_CREAT,S_IRUSR | S_IWUSR))<0){
	printf("can't create %s for writing", argv[1]);
}

if(write(file, data, size) != size){
	printf("error breh /n");
}

fstat(file, &buf);//used to determine the size of the file
if ((mm_file = mmap(0, (size_t) buf.st_size, PROT_READ|PROT_WRITE,
 MAP_SHARED, file, 0)) == (caddr_t) - 1) {
	fprintf(stderr, "mmap call fails\n");
}

if((child1 = fork()) == 0){//begin forking
	printf("Child 1 %d reads: \n %s\n", getpid(), mm_file);
	//{convert file content to uppercase string};
	printf("after UPPERCASE file\n");
	int c;
	int i;
	for(i = 0;i < size; i++){
		c = toupper(mm_file[i]);
		mm_file[i] = c;
	}

	msync(0, (size_t) buf.st_size, MS_SYNC);
	printf("Child 1 %d reads again: \n %s\n", getpid(), mm_file);
	exit(0);
}

if((child2 = fork()) == 0){
	sleep(1); //so that child 2 will perform its task after child 1 finishes
	printf("Child 2 %d reads: \n %s\n", getpid(), mm_file);
	//{use a hyphen to replace the space between “PAGE” and “SIZED”};
	if(strstr(mm_file, "PAGE SIZED")){
		strncpy(strstr(mm_file, "PAGE SIZED"),"PAGE-SIZED",strlen("PAGE-SIZED"));
	}
	
	//{replace “MAPPED” with “LOADED”};
	if(strstr(mm_file, "MAPPED")) {
		strncpy(strstr(mm_file, "MAPPED"), "LOADED", strlen("LOADED"));
	}
	printf("Child 2 %d reads again: \n %s\n", getpid(), mm_file);
	exit(0);
}

waitpid(child1,&status,0);
waitpid(child2,&status,0);
close(file);
exit(0);

}/**end of main*/