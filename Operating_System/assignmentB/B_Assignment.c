#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define COUNTING_NUMBER 2000000

struct smStruct {
	int processidassign; //Process ID
	int turn; //Other Process
	int flag[2];
	int critical_section_variable;
};

void shm_init(struct smStruct* smstruct, void* shmadd) {
	smstruct->turn = 0;
	smstruct->processidassign = 0;
	smstruct->flag[0] = 0;
	smstruct->flag[1] = 0;
	smstruct->critical_section_variable = 0;
}

int main(void) {

	pid_t pid1, pid2;
	int process_num[2] = { 0, 1 };
	int status1, status2;
	int shmid, ret;
	void *shmaddr;
	struct smStruct *smstruct;

	shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);
	if(shmid == -1) {
		perror("shared memory access is failed\n");
		return 0;
	}
	
	shmaddr = shmat(shmid, (void*)0, 0);
	if(shmaddr == (char*)-1) {
		perror("attach failed\n");
		return 0;	
	} 

	smstruct = shmaddr;
	shm_init(smstruct, shmaddr); //shared memory initialize

	pid1 = fork();
	if (pid1 == 0){
		execl("/home/kyeongha/HW#5/B_Assignment/child", "first", NULL);
	}

	pid2 = fork();
	if (pid2 == 0){
		execl("/home/kyeongha/HW#5/B_Assignment/child", "second", NULL);
	}

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);

  	if( (0 == status1) && (0 == status2) ){
		printf("Actual Count: %d | Expected Count : %d\n", smstruct->critical_section_variable, COUNTING_NUMBER*2);
	 	return 0;
	}
}


