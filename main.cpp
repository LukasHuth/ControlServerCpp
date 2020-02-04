#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{
	string errs = ["Cant't Intialize winsock! Quitting", "Can't create a socket! Quitting"];
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hC, 10);
	bool cn = false;
	WSADATA wsad;
	WORD ver = MAKEWORD(2, 2);
	SetConsoleTitle("*server*");
	int wsas = WSAStartup(ver, &wsad);
	if (wsas != 0)
	{
		cerr << errs[0] << endl;
		return 0;
	}

	SOCKET sl = socket(AF_INET, SOCK_STREAM, 0);
	if (sl == INVALID_SOCKET)
	{
		cerr << errs[1] << endl;
		return 0;
	}

	sockaddr_in sain;
	sain.sin_family = AF_INET;
	sain.sin_port = htons(54000);
	sain.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(sl, (sockaddr*)&sain, sizeof(sain));

	listen(sl, SOMAXCONN);

	sockaddr_in cl;

	int clsi = sizeof(cl);

	SOCKET clso = accept(sl, (sockaddr*)&cl, &clsi);

	char host[NI_MAXHOST];
	char sv[NI_MAXHOST];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(sv, NI_MAXHOST);
	string tHost;
	string tsv;
	if (getnameinfo((sockaddr*)&cl, sizeof(cl), host, NI_MAXHOST, sv, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << sv << endl;
		cn = true;
		tHost = host;
		tsv = sv;
	}
	else
	{
		inet_ntop(AF_INET, &cl.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(cl.sin_port) << endl;
		cn = true;
		tHost = host;
		tsv = sv;
	}

	closesocket(sl);

	char buf[4096];
	string ui;
	bool wh = true;
	cout << "Commands:" << endl << endl;
	cout << "echo title [title]" << endl;
	cout << "echo msg [message]" << endl;
	cout << "exit_user" << endl;
	cout << "exit" << endl;
	cout << "console show" << endl;
	cout << "console hide" << endl;
	cout << "console clear" << endl;
	cout << "cursor [freze|setlocation|move]" << endl;
	cout << "shutdown" << endl;
	cout << "reload" << endl;
	cout << "shutdown" << endl;
	cout << endl;
	string eu = "exit_user";
	do
	{
		cout << "> ";
		getline(cin, ui);

		if (ui == "exit") {
			send(clsi, eu.c_str(), eu.size() + 1, 0);
			closesocket(clso);
			WSACleanup();
			system("cls");
			return 0;
		}
		else if (ui == "reload")
		{
			closesocket(clso);
			WSACleanup();
			system("cls");
			return main();
		}

		if (ui.size() > 0)
		{
			int sendResult = send(clso, ui.c_str(), ui.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buf, 4096);
				if (ui == "exit_user")
				{
					closesocket(clso);
					WSACleanup();
					system("cls");
					return main();
				}
			}
		}
	} while (ui.size() > 0);

	closesocket(clso);

	WSACleanup();
	return 0;
}
