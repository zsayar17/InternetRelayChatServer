#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include "../Utils/Utils.hpp"

typedef struct
{
	int			socket_fd;
	uint16_t	port_num;
	uint16_t	sock_mode;
	uint16_t	sock_state;
	std::string	connect_ip;
} socket_t;

/*
* Tcp/Ip4 Socket
*/
class Socket {
	private:
		//States
		enum {SOCK_CREATE, SOCK_INIT};

		//Modes
		enum {SOCK_MAIN, SOCK_CLIENT};


		//Variables
		static const int buffer_size = 4096;

		socket_t	sock;
		ContPoll*	polls;

		//Hidden Choplin's Form Methods
		Socket(const Socket& other);
		Socket& operator=(const Socket& other);

		//initilaze socket funcs
		void createSocket();
		void setOptsSocket();
		void bindSocket();
		void listenSocket(int con_queue_count);

		//Implicitly Init Socket Method
		void startSocket(int fd, struct sockaddr_in& sock_addr);

	public:
		//Explicit Choplin's Form Methods
		Socket();
		~Socket();

		//Getters
		int getFd();

		//explicitly Init Socket Method
		void startSocket(short port_num, std::string connect_ip, short con_queue_count);

		//Accept Methods
		Socket* acceptSocket();

		//Polling Methods
		void addToPoll(int fd);
		void removeFromPoll(int fd);
		int polling();

		//Communication Methods
		std::string reciveMessage();
		void sendMessage(ContMessage& messages);
};


//Exception classes
class SocketCreateException : public std::exception {
	public:
		const char *what() const throw();
};

class SocketBindException : public std::exception {
	public:
		const char *what() const throw();
};

class SocketListenException : public std::exception {
	public:
		const char *what() const throw();
};

class SocketSetOptException : public std::exception {
	public:
		const char *what() const throw();
};

std::ostream& operator<<(std::ostream& file, socket_t& socket);

#endif
