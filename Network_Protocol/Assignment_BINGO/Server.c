#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256
#define NAME_SIZE 10

void * handle_clnt(void *arg);
void send_msg(char *msg, int len);
void error_handling(char *msg);
void* handle_game(void* arg);

int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
int start = 0;
int win_sig = 0;
pthread_mutex_t mutx;

struct MyClnt{
	char ip[16];
	int port;
	char name[NAME_SIZE];
	int bingo;
};

struct MyClnt clnt[MAX_CLNT];

int main(int argc, char * argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	pthread_t t_id2;
	if(argc!=2){
		printf("usage:%s<port>\n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutx, NULL);
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	pthread_create(&t_id, NULL, handle_game, (void*)&clnt_sock);
	pthread_detach(t_id);

	while(start!=1){
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt]=clnt_sock;
		pthread_mutex_unlock(&mutx);

		pthread_create(&t_id2, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id2);
		strcpy(clnt[clnt_cnt].ip,inet_ntoa(clnt_adr.sin_addr));//클라이언트 주소저장
		clnt[clnt_cnt].port=ntohs(clnt_adr.sin_port);//클라이언트의 포트저장
		
		clnt_cnt++;
		printf("connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}


	close(serv_sock);
	return 0;
}

void* handle_game(void* arg){
	while(1){
		
		char temp_msg[1+BUF_SIZE+NAME_SIZE];
		
		if(clnt_cnt==2 && start==0){ //클라이언트가 2개 접속하면 게임시작
			
			pthread_mutex_init(&mutx, NULL);
			send_msg("!start", 1+BUF_SIZE+NAME_SIZE);
			
			sprintf(temp_msg, "%s%s", "!turn", clnt[0].name);
			send_msg(temp_msg, 1+BUF_SIZE+NAME_SIZE);
			start = 1;

			pthread_mutex_unlock(&mutx);
		}

		
		/*char temp[1+NAME_SIZE+BUF_SIZE];
		char temp2[1+NAME_SIZE+BUF_SIZE];
		
		if(win_sig==1){
			if(clnt[0].bingo==0){ //0번째 클라이언트 LOSE
				sprintf(temp, "%s%s", "!end", "당신이 이겼습니다.");
				write(clnt_socks[1], temp, NAME_SIZE+BUF_SIZE+1);
				sprintf(temp2, "%s%s", "!end", "당신은 졌습니다.");
				write(clnt_socks[0], temp2, NAME_SIZE+BUF_SIZE+1);
			}else if(clnt[0].bingo==1){
				sprintf(temp, "%s%s", "!end", "당신이 이겼습니다.");
				write(clnt_socks[0], temp, NAME_SIZE+BUF_SIZE+1);
				sprintf(temp2, "%s%s", "!end", "당신은 졌습니다.");
				write(clnt_socks[1], temp2, NAME_SIZE+BUF_SIZE+1);
			}
		}*/

	}
}

void * handle_clnt(void *arg){
	int clnt_sock=*((int*)arg);
	int str_len=0, i;
	char msg[NAME_SIZE+BUF_SIZE+1];
	
	while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0){
		int j=0;
		printf("client to : %s\n", msg);

		char temp_name[NAME_SIZE];
		for(int i=0; i<NAME_SIZE;i++){
			temp_name[j++]=msg[i];
			if(msg[i]==93)
				break;
		}

		char temp_num[BUF_SIZE];
		for(int i=0; i<BUF_SIZE; i++)
			temp_num[i] = msg[i+j];
		temp_num[BUF_SIZE] = '\0';

		//printf("name:%s //// num:%s\n", temp_name, temp_num);

		if(!strncmp(msg,"!win", 4)){
			char temp_name2[NAME_SIZE];
			for(int i=5,k=0; i<15; i++){
				temp_name2[k++]=msg[i];
				if(msg[i]==93)
					break;
			}
			//printf("win:name:  %s", temp_name2);
			for(int i=0; i<clnt_cnt; i++){
				if((!strcmp(clnt[i].name, temp_name2)) && msg[4]==49){
					//clnt[i].bingo=1;
					//win_sig=1;
					char temp[1+NAME_SIZE+BUF_SIZE];
					char temp2[1+NAME_SIZE+BUF_SIZE];

					if(i==1){
						sprintf(temp, "%s%s", "!end", "당신이 이겼습니다.\n");
						write(clnt_socks[1], temp, NAME_SIZE+BUF_SIZE+1);
						sprintf(temp2, "%s%s", "!end", "당신은 졌습니다.\n");
						write(clnt_socks[0], temp2, NAME_SIZE+BUF_SIZE+1);
					}else{
						
						sprintf(temp, "%s%s", "!end", "당신은 졌습니다.\n");
						write(clnt_socks[1], temp, NAME_SIZE+BUF_SIZE+1);
						sprintf(temp2, "%s%s", "!end", "당신이 이겼습니다.\n");
						write(clnt_socks[0], temp2, NAME_SIZE+BUF_SIZE+1);
					}
				}
			}
			
		}
		else if(!strncmp(msg, "!set", 4)){ //플레이어 이름 설정_ok
			char temp_name2[10];
			for(int i=4,j=0; i<14; i++){
				if(msg[i]!=34) {
					temp_name2[j++]=msg[i];
				}
			}
			strcpy(clnt[clnt_cnt-1].name, temp_name2);
		}

		else {
			for(int i=0; i<clnt_cnt; i++){
				if(!strcmp(clnt[i].name, temp_name)){
					char temp[NAME_SIZE+BUF_SIZE+1];
					char temp2[NAME_SIZE+BUF_SIZE+1];

					if(i==clnt_cnt-1){
						sprintf(temp, "%s%s", "!turn", clnt[0].name);
						write(clnt_socks[0], temp, NAME_SIZE+BUF_SIZE+1);
					}else{
						sprintf(temp, "%s%s", "!turn", clnt[i+1].name);
						write(clnt_socks[i+1], temp, NAME_SIZE+BUF_SIZE+1);
					}
					
				
					sleep(1);
					sprintf(temp2, "%s%s", "!number", temp_num);
					send_msg(temp2, 1+NAME_SIZE+BUF_SIZE);
				}

			}
		}





		for(int i=0; i<1+NAME_SIZE+BUF_SIZE;i++){
			msg[i]='\0';
		}
		for(int i=0; i<NAME_SIZE; i++){
			temp_name[i]='\0';
		}
		for(int i=0; i<BUF_SIZE; i++){
			temp_num[i]='\0';
		}
		
	}




	pthread_mutex_lock(&mutx);
	for(i=0;i<clnt_cnt;i++){
		if(clnt_sock==clnt_socks[i]){
			while(i++<clnt_cnt-1)
				clnt_socks[i]=clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	
	

	return NULL;
	
}

void send_msg(char *msg, int len){
	int i;
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++){
		write(clnt_socks[i], msg, len);
	}
	pthread_mutex_unlock(&mutx);
}

void error_handling(char * msg){
	fputs(msg, stderr);
	fputc("\n", stderr);
	exit(1);
}









