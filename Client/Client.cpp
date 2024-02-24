#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "pdcurses.lib")
#include <WinSock2.h>
#define _WINSOCK_DEPRICATED_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <string>
#include <curses.h>

#pragma warning(disable: 4996)

SOCKET Connection;

char Username[16];

enum Packet {
	P_GlobalChatMessage,
	P_PrivateChatMessage,
	P_UserListRequest
};

bool ProcessPacket(Packet packettype) {
	switch (packettype) {
		case P_GlobalChatMessage:
		{
			char sender_username[16];
			int msg_size;
			recv(Connection, sender_username, sizeof(sender_username), NULL);
			recv(Connection, (char*)&msg_size, sizeof(int), NULL);
			char* msg = new char[msg_size + 1];
			msg[msg_size] = '\0';
			recv(Connection, msg, msg_size, NULL);
			printw(sender_username); addch(':'); printw(msg); addch('\n');
			delete[] msg;
			break;
		}
		case P_UserListRequest:
		{
			break;
		}
		default:
		{
			printw("Unrecognized packet: "); printw((char*)packettype); addch('\n');
			break;
		}
	}

	return true;
}

void ClientHandler() {
	Packet packettype;
	while (true) {
		recv(Connection, (char*)&packettype, sizeof(Packet), NULL);

		if (!ProcessPacket(packettype))
			break;

		refresh();
	}
	closesocket(Connection);
}

int main() {
	initscr();
	
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) // Проперка, загрузилась ли библиотека с сетевыми функциями
	{
		printw("Error! Lib doesn't load!"); addch('\n');
		exit(0);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		printw("Error! Failed to connect to the server!"); addch('\n');
		return 1;
	}

	printw("Connection success!\n");

	printw("Enter your username:");
	getstr(Username);
	send(Connection, Username, sizeof(Username), NULL);

	refresh();

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	while (true) {
		char buffer[512];
		getstr(buffer);
		std::string msg = buffer;
		int msg_size = msg.size();
		Packet packettype = P_GlobalChatMessage;
		send(Connection, (char*)&packettype, sizeof(Packet), NULL);
		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, msg.c_str(), msg_size, NULL);
		Sleep(10);
	}

	endwin();

	return 0;
}