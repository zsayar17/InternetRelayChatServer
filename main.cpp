#include "Server/Server.hpp"

//netstat -pa
//sudo wireshark
//strace -p <pid num>
//https://hea-www.harvard.edu/~fine/Tech/addrinuse.html
//ps -ef
//top -Hp <pid num>
//find . -name "*.cpp" -o -name "*.hpp" | xargs cat | wc -l

void sig_handler(int signo) {
	if (signo == SIGINT) Utils::setSignal(signo);
}

void loop (short port, char* password) {

	if (port < 0) throw(BadPortException());

	Server server(port, password);

	signal(SIGPIPE, sig_handler);
	signal(SIGINT, sig_handler);

	server.serverLoop();
}

int main(int argc, char **argv)
{
	try
	{
		if (argc != 3) throw(InvalidArgumentException());

		loop(Utils::convertToShort(argv[1]), argv[2]);
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}

	return 0;
}
