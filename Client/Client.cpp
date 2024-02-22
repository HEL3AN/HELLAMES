#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>

#pragma warning(disable: 4996)

SOCKET Connection;

char Username[16];

void ClientHandler() {
	char sender_username[16];
	int msg_size;
	while (true) {
		recv(Connection, sender_username, sizeof(sender_username), NULL);
		recv(Connection, (char*)&msg_size, sizeof(int), NULL);
		char *msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, msg, msg_size, NULL);
		std::cout << sender_username << ": " << msg << "\n";
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

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error! Failed to connect to the server!\n";
		return 1;
	}
	
	std::cout << "Connection success!\n";

	std::cout << "Enter your username:";
	std::cin.getline(Username, sizeof(Username));
	send(Connection, Username, sizeof(Username), NULL);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	while (true) {
		std::string msg;
		std::getline(std::cin, msg);
		int msg_size = msg.size();
		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, msg.c_str(), msg_size, NULL);
		Sleep(10);
	}

	system("pause");
	return 0;
}