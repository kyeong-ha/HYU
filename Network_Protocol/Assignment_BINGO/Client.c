#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20
#define SIZE 5

void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char *msg);
void * choise_number(void* arg);
void * make_bingo();
void * print_bingo();
void * check_bingo(int board[][SIZE]);

char name[NAME_SIZE]="[DEF]";
int bingo[SIZE][SIZE] = {0,};

struct Board{
	int start; //게임시작 상태인지
	int turn; //나의 턴인지
	int game_turn;
	int win_flag; //빙고를 맞췄는지
	int board[SIZE][SIZE]; //현재 플레이어의 빙고판 상태
	int bingo[SIZE][SIZE];
	int num_of_bingo; //현재 플레이어의 빙고개수
};

struct Board MyBoard = {0, };

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t choise_thread, recv_thread;
	void *thread_return;

	make_bingo();

	if(argc!=4){
		printf("usage:%s <IP> <port> <name> \n", argv[0]);
		exit(1);
	}


	sprintf(name, "[%s]", argv[3]);
	sock=socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");

	

	pthread_create(&choise_thread, NULL, choise_number, (void*)&sock);
	pthread_create(&recv_thread, NULL, recv_msg, (void*)&sock);

	pthread_join(choise_thread, &thread_return);
	pthread_join(recv_thread, &thread_return);

if(1){
	close(sock);
	return 0;
}
}

void * make_bingo(){
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			printf("[%d][%d]번째 숫자 : ", i, j);
			scanf("%d", &MyBoard.board[i][j]);
		}
	}
	print_bingo();

}

void * print_bingo(){
	char* star ="*";
	printf("========================\n");
	printf("빙고판\n");
	printf("------------------------\n");
	printf("\n\n");
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			if(MyBoard.bingo[i][j]==1)
				printf("%4s", star);
			else
				printf("%4d", MyBoard.board[i][j]);
		}
		printf("\n\n");
	}
	printf("------------------------\n");
	if(MyBoard.turn==1)
		printf("당신의 차례입니다.\n숫자를 입력해주세요!\n");
	else if(MyBoard.turn==0 && MyBoard.start==1)
		printf("상대방이 숫자를 입력 중입니다...\n");
	printf("========================\n");
}

void * choise_number(void *arg){
	int sock=*((int*)arg);
	
	char name_set[NAME_SIZE+BUF_SIZE+1]; //서버에 이름을 저장하기 위해 클라이언트 이름 전송
	 sprintf(name_set, "%s%s", "!set", name);
	write(sock, name_set, strlen(name_set));

	while(1){
		char msg[BUF_SIZE];
		char name_msg[NAME_SIZE+BUF_SIZE+1];

		if(MyBoard.turn==1 && MyBoard.start==1){
			scanf("%s", msg);

			sprintf(name_msg, "%s%s", name, msg);
			write(sock, name_msg, strlen(name_msg));
			MyBoard.turn--;
			
		}
	}
}

void * recv_msg(void * arg){
	int sock=*((int*) arg);
	char FLAG[1+NAME_SIZE+BUF_SIZE];
	ssize_t str_len;
	int num;
	


	while(1){
		char msg[1+NAME_SIZE+BUF_SIZE];
		str_len=read(sock, msg, 1+NAME_SIZE+BUF_SIZE);
		int j=0;

			if(!strncmp(msg, "!start", 6)){ //게임시작
				MyBoard.start=1;
			}
			else if(!strncmp(msg, "!turn", 5)){ //0번째 플레이어의 턴으로 설정
				
				char temp_name2[10];
				for(int i=5,j=0; i<15; i++){
					if(msg[i]!=34) {
						temp_name2[j]=msg[i]; j++;
					}
				}
				if(!strcmp(temp_name2, name)){
					MyBoard.turn++;}
			}
			else if(!strncmp(msg, "!end", 4)){ 
				char temp_msg[BUF_SIZE];
				for(int i=4,j=0; i<BUF_SIZE+4; i++){
					temp_msg[j++] = msg[i];
				}

				printf("%s", temp_msg);

				close(sock);
				exit(0);

			}
			else if(!strncmp(msg, "!number", 7)){
				char temp_num[BUF_SIZE];
				for(int i=7,j=0; i<BUF_SIZE+7; i++){
					temp_num[j++] = msg[i];
				}
				//printf("전:%s\n",msg);
				//printf("수:%s\n", temp_num);
				/*if(temp_num[1]==10)
					num = temp_num[0]-48;
				else
					num = (temp_num[0]-48)*10 + temp_num[1]-48;
*/

				num = atoi(temp_num);
				for(int i=0; i<SIZE; i++){
					for(int j=0; j<SIZE; j++){
						if(MyBoard.board[i][j] == num){
							MyBoard.bingo[i][j] = 1;
						}	
					}
				}
				
				for(int i=0; i<BUF_SIZE;i++)
					FLAG[i]='\0';

				MyBoard.num_of_bingo = check_bingo(MyBoard.bingo);

				if(MyBoard.num_of_bingo >= 3){
					sprintf(FLAG, "%s%s%s", "!win", "1", name);
					write(sock, FLAG, strlen(FLAG));
					
				}else{
					sprintf(FLAG, "%s%s%s", "!win", "0", name);
					write(sock, FLAG, strlen(FLAG));
				}
			}

			//printf("%s/%s\n", temp_name, temp_num);
			//printf("%s\n", msg);
			//printf(strcmp(msg, start_str));
			
			printf("%d / %d\n", MyBoard.start, MyBoard.turn);
			system("clear");

			printf("결과 수집중입니다...\n");
			sleep(2);
			system("clear");


			for(int i=0; i<BUF_SIZE;i++)
				msg[i] = '\0';


			print_bingo();		
	}
	
	
}

void * check_bingo(int board[][SIZE]){
	int i;
	int count=0;

	for(i=0; i < SIZE; i++) 
	{
		if(board[i][0]==1&&board[i][1]==1&&board[i][2]==1&&board[i][3]==1&&board[i][4]==1) //가로
		{
			count++;
		}
		if(board[0][i]==1&&board[1][i]==1&&board[2][i]==1&&board[3][i]==1&&board[4][i]==1) //세로
			count++;
	}
	if(board[0][0]==1&&board[1][1]==1&&board[2][2]==1&&board[3][3]==1&&board[4][4]==1)
		count++;
	if(board[0][4]==1&&board[1][3]==1&&board[2][2]==1&&board[3][1]==1&&board[4][0]==1)
		count++;
	return count;
}

void error_handling(char * msg){
	fputs(msg, stderr);
	fputc("\n", stderr);
	exit(1);
}






