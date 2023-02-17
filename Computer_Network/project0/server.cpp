#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

#define PORT 4545
#define PACKET_SIZE 4096


int main() {
	WSADATA wsaData; //���� �ʱ�ȭ ������ �����ϱ� ���� ����ü
	WSAStartup(MAKEWORD(2, 2), &wsaData); //(���Ϲ���, ����ü �ּ�)

	//���ϻ���
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//���� ������� ä���
	SOCKADDR_IN sockAddr = {};
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY); //���� ��ǻ���� IP�ּҸ� �־���
	sockAddr.sin_port = htons(PORT); //������ ��Ʈ��ȣ�� �־���

	//bind
	bind(sock, (SOCKADDR*)&sockAddr, sizeof(sockAddr)); //���Ͽ� �ּ����� ����

	//listen
	listen(sock, SOMAXCONN); //���� �����ϴ� ����

	printf("tcpServer Listen.\n");

	while (true) {
		SOCKADDR_IN clntAddr = {};
		int clntSize = sizeof(clntAddr);
		SOCKET client = accept(sock, (SOCKADDR*)&clntAddr, &clntSize); //Ŭ���̾�Ʈ�� ���� ��û�� �ϸ� ��������.
		
		printf("connect\n");

		while (true) {
			char buffer[PACKET_SIZE] = {};
			
			int r = recv(client, buffer, PACKET_SIZE, 0); //�޼����� �ް�, �޼����� ������ ��� �����¿� �ӹ���.
			
			if (r == -1) {
				break;
				closesocket(client);
				
			}
			else {

				printf("[Client said]: %s\n", buffer);

				char msg[] = "tcpServer has received your message\n";
				send(client, msg, strlen(msg), 0); //�޼����� ����
			}
		}
		
	}
	WSACleanup();
}