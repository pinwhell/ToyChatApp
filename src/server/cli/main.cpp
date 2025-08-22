#include <ChatApp/Server.h>
#include <iostream>

int main(int argc, const char** argv)
{
	if (argc > 1 && !strcmp(argv[1], "help")) {
		std::cout << "chatapp-clnt-sv [port]\n";
		return 0u;
	}

	int port = argc < 2 ? 3000 : atoi(argv[1]);

	ChatApp::Server(port)
		.Run();
}
