#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define COUNTING_NUMBER 2000000

struct smStruct {
	int processidassign;
	int turn;
	int flag[2];
	int critical_section_variable;
};

void lock(struct smStruct *smstruct, int myorder) {
	smstruct->turn = 1- myorder;
	smstruct->flag[myorder] = 1;
	while(smstruct->flag[1-myorder] == 1 && smstruct->turn == 1-myorder);
}

void unlock(struct smStruct *smstruct, int self){
	smstruct->flag[self] = 0;
}

int main(int argc, char* argv[]) {
 	int shmid, myOrder;
	int localCount = 0;
	void *shmaddr;

	if(strcmp(argv[0], "first") == 0)
		myOrder = 0;
	else if(strcmp(argv[0], "second") == 0)
		myOrder = 1;
	
	
	struct smStruct *smstruct;

	smstruct->processidassign = getpid();
	printf("Myorder = %d,process pid = %d\n", myOrder, smstruct->processidassign);

	shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);
	if(shmid < 0){
		perror("shmget");
		return 0;
	}

	shmaddr = shmat(shmid, (void*) 0, 0);
	if(shmaddr == (char*)-1) {
		perror("attach faild\n:");
		return 0;
	}

	smstruct = shmaddr;

	for(int i = 0; i<COUNTING_NUMBER; i++) {
		localCount++;
		lock(smstruct, myOrder);
		smstruct->critical_section_variable++;
		unlock(smstruct, myOrder);
	}
	printf("child finish! local count = %d\n", localCount);
  
   	return 0;
}
