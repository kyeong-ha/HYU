#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define PIPENAME "./named_pipe_file"

int main(){
	int ret;
	char msg[512];
	int fd;
	pid_t pid;

	//파이프가 존재하는지 조사
	ret = access(PIPENAME, F_OK);
	if(ret == 0)
		unlink(PIPENAME);

	//파이프 생성
	ret = mkfifo(PIPENAME, 0666);
	if(ret < 0){
		printf("Creation of named pipe failed\n");
		return 0;
	}

	fd = open(PIPENAME, O_RDWR);
	if(fd < 0){
		printf("Opening of named pipe failed\n");
		return 0;
	}

	//end를 받으면 종료함
	while(strcmp(msg, "end") != 0){
		ret = read(fd, msg, sizeof(msg));
		if(ret < 0){
			printf("Read failed\n");
			return 0;
		}
		printf("%s\n", msg);
	}
	return 0;
}
