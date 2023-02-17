#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

struct msgbuf{
	long msgtype;
	char mtext[20];
};

int main(){
	key_t key_id;
	int i=0;
	int msgtype=3;
	struct msgbuf rsvbuf;
	
	//메세지 큐의 id를 반환하여 key_id에 저장
	key_id = msgget((key_t)1234, IPC_CREAT);
	if(key_id == -1){
		perror("msgget error : ");
		return 0;
	}

	//end 문장이 나오면 종료함 
	while(strcmp(rsvbuf.mtext, "end") != 0){
		if(msgrcv(key_id, (void*)&rsvbuf, sizeof(struct msgbuf), msgtype, 0) == -1)
			perror("msgrcv error : ");
		else
			printf("The received message is: %s\n", rsvbuf.mtext);
	}
	return 0;
}
