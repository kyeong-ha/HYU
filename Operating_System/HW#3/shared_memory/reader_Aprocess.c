#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

int main() {
	int shmid, ret;
	void *shmaddr;
	
	//공유메모리의 id를 얻어와 shmid에 저장 
	shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);
	if(shmid == -1) {
		perror("shared memory access is failed\n");
		return 0;
	}

	//공유메모리 부착
	shmaddr = shmat(shmid, (void*)0, 0);
	if(shmaddr == (char *)-1) {
		perror("attach failed\n");
		return 0;
	}

	//end를 받을 때까지 반복
	while(1) {		
		if(!strcmp((char*)shmaddr, "end\n"))
			break;
		
		//메세지에 아무것도 없는 경우
		if(((char*)shmaddr)[0] != '\0'){ 
			printf("data read from shared memory: %s", (char*)shmaddr);
			((char*)shmaddr)[0] = '\0'; //반복을 위해 메세지를 초기화해줌
		}
	}

	ret = shmdt(shmaddr);
	if(ret == -1) {
		perror("detach failed\n");
		return 0;
	}

	//공유메모리 삭제
	ret = shmctl(shmid, IPC_RMID, 0);
	if(ret == -1) {
		perror("remove failed\n");
		return 0;
	}
	
	return 0;
}

	
