#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

#define SERVER_IP "192.168.219.108"
#define PORT 4545
#define PACKET_SIZE 4096

int main() {
	WSADATA wsaData; //소켓 초기화 정보를 저장하기 위한 구조체
	WSAStartup(MAKEWORD(2, 2), &wsaData); //(소켓버전, 구조체 주소)

	SOCKET hSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP); //Sever와 동일

	connect(hSock, (SOCKADDR*)&addr, sizeof(addr)); //서버에게 연결요청

	char msg[] = "hello, tcpServer!";
	send(hSock, msg, strlen(msg), 0); //서버에게 메세지를 보냄

	char buffer[PACKET_SIZE] = {};
	recv(hSock, buffer, PACKET_SIZE, 0); //서버에게서 메세지를 받음

	printf("[tcpServer said]: %s\n", buffer);

	closesocket(hSock); //소켓 닫기
	WSACleanup();
}