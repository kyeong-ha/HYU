#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MSG_SIZE 80
#define PIPENAME "./named_pipe_file"

int main(void){
	char msg[MSG_SIZE];
	int fd, ret, i;

	fd = open(PIPENAME, O_WRONLY);
	if(fd < 0){
		printf("Open failed\n");
		return 0;
	}
	
	//end 를 입력할 때까지 반복 
	while(strcmp(msg, "end") != 0){
		sleep(1);

		fgets(msg, sizeof(msg), stdin);
		msg[strlen(msg)-1] = '\0'; //개행문자 제거

		ret = write(fd, msg, sizeof(msg)); //입력받은 메세지 작성
		if(ret < 0){
			printf("Write failed\n");
			return 0;
		}
	}
	return 0;
}
