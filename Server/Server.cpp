#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <map>

#pragma warning(disable: 4996)

#define MAXUSERS 100

std::map<int, std::pair<SOCKET, char[16]>> Connections;

void ClientHandler(int index) {
	int msg_size;
	while (true) {
		recv(Connections[index].first, (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connections[index].first, msg, msg_size, NULL);

		for (int i = 0; i < Connections.size(); i++) {
			if (i == index)
				continue;

			send(Connections[i].first, Connections[index].second, sizeof(Connections[index].second), NULL);
			send(Connections[i].first, (char*)&msg_size, sizeof(int), NULL);
			send(Connections[i].first, msg, msg_size, NULL);
		}

		delete[] msg;
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

		Connections[i].first = newConnection;
		recv(Connections[i].first, Connections[i].second, sizeof(Connections[i].second), NULL);

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
	}

	system("pause");
	return 0;
}