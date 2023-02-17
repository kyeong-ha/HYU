#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct msgbuf {
	long msgtype;
	char mtext[20];
};

int main(void) {
	key_t key_id;
	int i=0;
	struct msgbuf sndbuf;

	//메세지 큐의 id를 반환하여 key_id에 저장
	key_id = msgget((key_t)1234, IPC_CREAT|0666);
	if(key_id == -1){
		perror("msgget error : ");
		return 0;
	}

	sndbuf.msgtype = 3;

	//end 를 입력할 때까지 반복 
	while(strcmp(sndbuf.mtext, "end") != 0) {
		fgets(sndbuf.mtext, sizeof(sndbuf.mtext), stdin);
		sndbuf.mtext[strlen(sndbuf.mtext)-1] = '\0'; //맨 뒤에 있는 개행문자 제거

		if(msgsnd(key_id, (void*)&sndbuf, sizeof(struct msgbuf), IPC_NOWAIT) == -1)
			perror("msgsnd error : ");

		printf("Sending message is succeed\n");
	}
	return 0;
}
