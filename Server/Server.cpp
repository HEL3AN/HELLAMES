#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <map>

#pragma warning(disable: 4996)

#define MAXUSERS 100

std::map<int, SOCKET> Connections;

void ClientHandler(int index) {
	char msg[256];
	while (true) {
		recv(Connections[index], msg, sizeof(msg), NULL);

		for (int i = 0; i < Connections.size(); i++) {
			if (i == index)
				continue;

			send(Connections[i], msg, sizeof(msg), NULL);
		}
	}
}

int main() {

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) // Проперка, загрузилась ли библиотека с сетевыми функциями
	{
		std::cout << "Error! Lib doesn't load!\n";
		exit(0);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < MAXUSERS; ++i) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0)
			std::cout << "Error! User doesn't connected to the server!\n";
		else
			std::cout << "Client connected!\n";

		Connections[i] = newConnection;

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
	}

	system("pause");
	return 0;
}