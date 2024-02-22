#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>

#pragma warning(disable: 4996)

SOCKET Connection;

void ClientHandler() {
	char msg[256];
	while (true) {
		recv(Connection, msg, sizeof(msg), NULL);

		std::cout << msg << "\n";
	}
}

int main() {

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) // ��������, ����������� �� ���������� � �������� ���������
	{
		std::cout << "Error! Lib doesn't load!\n";
		exit(0);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error! Failed to connect to the server!\n";
		return 1;
	}
	
	std::cout << "Connection success!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	char msg[256];
	while (true) {
		std::cin.getline(msg, sizeof(msg));
		send(Connection, msg, sizeof(msg), NULL);
		Sleep(10);
	}

	system("pause");
	return 0;
}