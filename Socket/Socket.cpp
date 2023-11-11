#include "Socket.hpp"

// *******Hidden Choplin's form funcs******* //
Socket::Socket(const Socket& other) {
	(void)other;
}

Socket& Socket::operator=(const Socket& other) {
	(void)other;
	return (*this);
}
// *******End******* //

Socket::Socket() {
	sock.sock_state = SOCK_CREATE;
	sock.socket_fd = 0;
}

Socket::~Socket() {
	if (sock.socket_fd) close(sock.socket_fd);
	if (sock.sock_mode == SOCK_MAIN) {
		delete polls;
	}
}

void Socket::createSocket() {
	sock.socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock.socket_fd < 0) throw (SocketCreateException());
}

void Socket::setOptsSocket() {
	int yes;
	int ret;

	yes = 1;
	ret = setsockopt(sock.socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (ret < 0) throw (SocketSetOptException());
	fcntl(sock.socket_fd, F_SETFL, O_NONBLOCK);
}

void Socket::bindSocket() {
	struct sockaddr_in	sock_addr;
	int					ret_value;

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(sock.port_num);
	sock_addr.sin_addr.s_addr = inet_addr(sock.connect_ip.c_str());
	ret_value = bind(sock.socket_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
	if (ret_value < 0) throw (SocketBindException());
}

void Socket::listenSocket(int con_queue_count) {
	if (listen(sock.socket_fd, con_queue_count) < 0) throw (SocketListenException());
}

/*
	@brief port_num: Get virtual port number to listen requests.
	@brief connect_ip: Get ip4 adress of accepting requests.
	@brief con_queue_count: Get maximum count of connection queque member.
*/
void Socket::startSocket(short port_num, std::string connect_ip, short con_queue_count) {
	//if (sock.sock_state != SOCK_CREATE) throw(SocketCreateException());

	polls = new (std::vector<pollfd>);
	sock.sock_mode = SOCK_MAIN;
	sock.port_num = port_num;
	sock.connect_ip = connect_ip;

	createSocket();
	setOptsSocket();
	bindSocket();
	listenSocket(con_queue_count);
	sock.sock_state = SOCK_INIT;

	std::cout << sock << std::endl;
}

void Socket::startSocket(int fd, struct sockaddr_in& sock_addr) {
	sock.connect_ip = inet_ntoa(sock_addr.sin_addr);
	sock.port_num = ntohs(sock_addr.sin_port);
	sock.socket_fd = fd;
	sock.sock_mode = SOCK_CLIENT;
	sock.sock_state = SOCK_INIT;
	fcntl(sock.socket_fd, F_SETFL, O_NONBLOCK);
	polls = NULL;

	std::cout << sock << std::endl;
}

////////////////////////////////////////////////////////////////



Socket* Socket::acceptSocket() {
	Socket*				new_sock;
	struct sockaddr_in	sock_addr;
	int					sock_fd;
	unsigned int		sock_len;

	sock_len = sizeof(sock_addr);
	sock_fd = accept(sock.socket_fd, (struct sockaddr *)&sock_addr, &sock_len);
	if (sock_fd < 0)
		return (NULL);

	new_sock = new Socket();
	new_sock->startSocket(sock_fd, sock_addr);
	return (new_sock);
}

std::string Socket::reciveMessage() {
	std::string			msg;
	char				buffer[buffer_size];
	int					r_size;

	r_size = buffer_size - 1;
	while (1)
	{
		memset(buffer, 0, r_size + 1);
		r_size = recv(sock.socket_fd, buffer, buffer_size - 1, 0);
		if (r_size <= 0) break;
		msg += buffer;
	}
	return (msg);
}

void Socket::sendMessage(ContMessage& messages) {
	std::string			message;
	size_t				message_size;
	char				buffer[buffer_size];
	int					sent_len;


	while (messages.size()) {
		message += *messages.begin();
		messages.erase(messages.begin());
		if (messages.size() > 1) message += ' ';
	}

	while (message.length())
	{
		if (message.size() > buffer_size) message_size = buffer_size;
		else  message_size = message.size();

		memset(buffer, 0, buffer_size);
		memcpy(buffer, message.c_str(), message_size);
		message = message.substr(message_size);
		sent_len = send(sock.socket_fd, buffer, message_size, 0);
		if (sent_len < 0) return;
	}
}

void Socket::addToPoll(int fd) {
	IterPoll	iter;
	pollfd		new_poll;

	iter = polls->begin();
	while (iter != polls->end())
	{
		if (iter->fd == fd) break;
		iter ++;
	}
	if (iter != polls->end()) return;// already exist

	new_poll.events = POLLIN | POLLHUP;
	new_poll.fd = fd;
	polls->push_back(new_poll);
}

void Socket::removeFromPoll(int fd) {
	IterPoll	iter;

	iter = polls->begin();
	while (iter != polls->end())
	{
		if (iter->fd == fd) break;
		iter ++;
	}
	if (iter == polls->end()) return;// has not contain

	polls->erase(iter);
}

int Socket::polling() {
	int		ret_value;
	short	revent;
	int		fd;

	if (sock.sock_mode != SOCK_MAIN) goto RETURN;

	ret_value = poll(polls->data(), polls->size(), 0);
	if (ret_value <= 0) goto RETURN;
	for (size_t i = 0; i < polls->size(); i++)
	{
		revent = (*polls)[i].revents;
		fd = (*polls)[i].fd;
		if (revent & POLLHUP) return (-fd); //connection is gone
		if (revent & POLLIN) return (fd); //return fd of recieved message fd(also user id)
	}
	RETURN:
		return (-1); //there is no recieved message into pool
}

int Socket::getFd()
{
	return (sock.socket_fd);
}


const char* SocketCreateException::what() const throw() {
	return ("Socket could not created!");
}

const char* SocketBindException::what() const throw() {
	return ("Socket could not binded!");
}

const char* SocketListenException::what() const throw() {
	return ("Socket could not listened!");
}

const char* SocketSetOptException::what() const throw() {
	return ("Socket option Could not setted!");
}

std::ostream& operator<<(std::ostream& file, socket_t& socket) {
	return (file << "socket fd: " << socket.socket_fd << "\nsocket port: " << socket.port_num
		<< "\nsocket ip: " << socket.connect_ip);
}
