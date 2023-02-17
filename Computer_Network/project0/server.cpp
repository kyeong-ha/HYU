#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

#define PORT 4545
#define PACKET_SIZE 4096


int main() {
	WSADATA wsaData; //소켓 초기화 정보를 저장하기 위한 구조체
	WSAStartup(MAKEWORD(2, 2), &wsaData); //(소켓버전, 구조체 주소)

	//소켓생성
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//소켓 구성요소 채우기
	SOCKADDR_IN sockAddr = {};
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY); //현재 컴퓨터의 IP주소를 넣어줌
	sockAddr.sin_port = htons(PORT); //지정한 포트번호를 넣어줌

	//bind
	bind(sock, (SOCKADDR*)&sockAddr, sizeof(sockAddr)); //소켓에 주소정보 연결

	//listen
	listen(sock, SOMAXCONN); //연결 수신하는 상태

	printf("tcpServer Listen.\n");

	while (true) {
		SOCKADDR_IN clntAddr = {};
		int clntSize = sizeof(clntAddr);
		SOCKET client = accept(sock, (SOCKADDR*)&clntAddr, &clntSize); //클라이언트가 접속 요청을 하면 승인해줌.
		
		printf("connect\n");

		while (true) {
			char buffer[PACKET_SIZE] = {};
			
			int r = recv(client, buffer, PACKET_SIZE, 0); //메세지를 받고, 메세지가 없으면 계속 대기상태에 머문다.
			
			if (r == -1) {
				break;
				closesocket(client);
				
			}
			else {

				printf("[Client said]: %s\n", buffer);

				char msg[] = "tcpServer has received your message\n";
				send(client, msg, strlen(msg), 0); //메세지를 보냄
			}
		}
		
	}
	WSACleanup();
}