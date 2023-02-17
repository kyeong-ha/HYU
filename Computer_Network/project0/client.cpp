#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

#define SERVER_IP "192.168.219.108"
#define PORT 4545
#define PACKET_SIZE 4096

int main() {
	WSADATA wsaData; //���� �ʱ�ȭ ������ �����ϱ� ���� ����ü
	WSAStartup(MAKEWORD(2, 2), &wsaData); //(���Ϲ���, ����ü �ּ�)

	SOCKET hSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP); //Sever�� ����

	connect(hSock, (SOCKADDR*)&addr, sizeof(addr)); //�������� �����û

	char msg[] = "hello, tcpServer!";
	send(hSock, msg, strlen(msg), 0); //�������� �޼����� ����

	char buffer[PACKET_SIZE] = {};
	recv(hSock, buffer, PACKET_SIZE, 0); //�������Լ� �޼����� ����

	printf("[tcpServer said]: %s\n", buffer);

	closesocket(hSock); //���� �ݱ�
	WSACleanup();
}