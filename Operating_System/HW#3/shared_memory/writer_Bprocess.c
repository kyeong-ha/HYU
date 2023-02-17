#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

int main() {
	int shmid, i;
	void *shmaddr;
	char buff[128];

 	shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);
	if(shmid < 0) {
		perror("shmget");
		return 0;
	}
	
	//공유메모리 부착
	shmaddr = shmat(shmid, (void* )0, 0);
	if(shmaddr == (char *)-1) {
		perror("attach failed\n");
		return 0;
	}
	
	while(1) {

		fgets(buff, sizeof(buff), stdin);
		strcpy((char*)shmaddr, buff); //입력받은 한 줄을 공유메모리에 넣음

		if(!strcmp((char*)shmaddr, "end\n")) //end 문장 입력시 종료
			break;
	}
	
	int ret = shmdt(shmaddr);
	if(ret == -1) {
		perror("detach failed\n");
		return 0;
	}

	return 0;
}
