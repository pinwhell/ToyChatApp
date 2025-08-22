#include <ChatApp/Client.h>
#include <iostream>

int main(int argc, const char** argv)
{
	if (argc > 1 && !strcmp(argv[1], "help")) {
		std::cout << "chatapp-clnt-cli [ip] [port]\n";
		return 0u;
	}

	const char* ip = argc < 2 ? "127.0.0.1" : argv[1];
	int port = argc < 3 ? 3000 : atoi(argv[2]);

	ChatApp::Client(ip, port)
		.Run();

	return 0u;
}