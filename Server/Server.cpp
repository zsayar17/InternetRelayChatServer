#include "Server.hpp"

//Chaplin's Form begin
Server::Server() {

}

Server::Server(uint16_t port, std::string password, std::string server_ip, int max_connect_count) {
	this->password = password;
	sock.startSocket(port, server_ip, max_connect_count);
	initCommands();
}

Server::Server(const Server& other) {
	*this = other;
}

Server& Server::operator=(const Server& other) {
	(void)other;
	return (*this);
}

Server::~Server() {
	destroyServer();
}
//Chaplin's Form end


//Channel Methods Begin
void Server::createChannel(std::string channel_name) {
	Channel			*channel;
	IterChannel		iter;

	iter = channels.find(channel_name);
	if (iter != channels.end()) return; //already there is a channel with same name
	channel = new Channel();
	channel->initChannel(channel_name);
	channels[channel_name] = channel;
}

void Server::closeChannel(std::string channel_name) {
	IterChannel	iter;

	iter = channels.find(channel_name);
	if (iter == channels.end()) return; // not into channels
	iter->second->shutDownChannel();
	delete iter->second;
	channels.erase(iter);
}

void Server::destroyEmptyChannels() {
	IterChannel iter;
	IterChannel temp_iter;

	iter = channels.begin();
	while (iter != channels.end())
	{
		temp_iter = iter;
		iter++;
		if (temp_iter->second->getUsersCount()) continue;
		temp_iter->second->shutDownChannel();
		delete temp_iter->second;
		channels.erase(temp_iter);
	}
}

void Server::destroyCommands() {
	ACommand *command;

	while (commands.size()) {
		command = commands.begin()->second;
		commands.erase(commands.begin());
		delete command;
	}
}
//Channel Methods End


//User Methods Begin
void Server::connectUser() {
	Socket		*new_sock;
	User		*user;

	new_sock = sock.acceptSocket();
	if (!new_sock) return; // no one accepted
	user = new User();
	user->bindSocket(new_sock);
	users[user->getUserId()] = user;

	sock.addToPoll(user->getUserId());
}

void Server::dissconnectUser(int user_id) {
	IterUser iter;

	iter = users.find(user_id);
	if (iter == users.end()) return; //not in users
	iter->second->shutDownUser();
	delete iter->second;
	users.erase(iter);

	sock.removeFromPoll(user_id);
	destroyEmptyChannels();
}

//User Methods End


//Communication Methods Begin
void Server::recieveMessage(UserForm& form) {
	int			user_id;
	std::string	message;

	user_id = sock.polling();
	if (user_id == -1) return; //poll could not read anything
	else if (user_id < -1) {
		dissconnectUser(-user_id);
		return ; //client is disconnected
	}
	message = users[user_id]->reciveMessage();
	if (!message[0]) {
		dissconnectUser(user_id);
		return ; //client killed communicataion with signal
	}

	if (!Utils::checkComplatedMassage(message)) {
		users[user_id]->writeMessageToBuffer(message);
		return;
	}
	message = users[user_id]->readMessageBuffer() + message;
	users[user_id]->cleanMessageBuffer();

	form.first.push_back(message);
	form.second = reinterpret_cast<IUser*>(users[user_id]);
}

void Server::analyseCommand(UserForm& form) {
	ContMessage message;

	errno = 0;
	if (!form.second || !form.first.size()) return;


	Utils::reshapeMessage(form.first[0]);
	if (!form.first[0].size()) goto FAILED; //after reduce '\n' and '\r' empty command

	message = Utils::split(form.first[0], ' ');
	if (!message.size()) goto FAILED; //no any command

	Utils::reshapeCommand(message[0]);
	if (!message[0].size()) goto FAILED; //after reduce '/' empty command

	form.first.insert(form.first.begin(), message[0]);
	if (acceptHexChatCommand(form)) return;
	return ;

	FAILED:
	errno = ERRNO_ERR_UNKNOWNCOMMAND;
	form.first.clear();
	return ;
}

void Server::resetUserForm(UserForm& form) {
	form.first.clear();
	form.second = NULL;
	errno = 0;
}

bool Server::acceptHexChatCommand(UserForm& form) {
	ContMessage message;

	if (form.first[0] == "WHO") {
		resetUserForm(form);
		return (true);
	}
	else if (!(form.first[1].find('\n') != form.first[1].npos)) return (false);

	message = Utils::split(form.first[1], '\n');
	for (size_t i = 0; i < message.size(); i++) {
		if (message[i].find("CAP") == 0) continue;
		form.first.clear();
		form.first.push_back(message[i]);
		analyseCommand(form);
		executeCommand(form);
	}
	resetUserForm(form);
	return (true);
}
//Communication Methods Begin


//Command Methods Begin
void Server::initCommands() {
	commands["PASS"] = new PassCommand;
	commands["NICK"] = new NickCommand;
	commands["USER"] = new UserCommand;
	commands["QUIT"] = new QuitCommand;
	commands["JOIN"] = new JoinCommand;
	commands["PART"] = new PartCommand;
	commands["MODE"] = new ModeCommand;
	commands["KICK"] = new KickCommand;
	commands["PING"] = new PingCommand;
	commands["NAMES"] = new NamesCommand;
	commands["INVITE"] = new InviteCommand;
	commands["NOTICE"] = new NoticeCommand;
	commands["PRIVMSG"] = new PrivMsgCommand;
}

void Server::executeCommand(UserForm& form) {
	IterCommand	iter;
	std::string	command;
	ContMessage	message;

	if (errno && form.second) goto ERROR;
	if (!form.second || form.first.empty()) return; //has not contain user or message(could be edited recived message)
	command = form.first[0];
	form.first.erase(form.first.begin());

	iter = commands.find(command);
	if (iter == commands.end()) {
		errno = ERRNO_ERR_UNKNOWNCOMMAND;
		goto ERROR; //wrong command
	}
	if (form.second->isRegistered()) goto EXECUTE;

	// New User
	if (command == "PASS") goto EXECUTE;
	else if (form.second->isActive() && (command == "NICK" || command == "USER")) goto EXECUTE;
	errno = ERRNO_ERR_NOTREGISTERED;
	goto ERROR; // has not registered for run a server command

	EXECUTE:
	iter->second->setCommand(form);
	iter->second->execute(*this, form);
	return ;

	ERROR:
	if (errno == ERRNO_ERR_NOTREGISTERED) message.push_back(ERR_NOTREGISTERED(Utils::getSorurce(form.second)));
	else if (errno == ERRNO_ERR_UNKNOWNCOMMAND) message.push_back(ERR_UNKNOWNCOMMAND(Utils::getSorurce(form.second)));

	form.second->sendMessageToUser(form.second, message);
}
//Command Methods End


//Loop Begin
void Server::serverLoop() {
	UserForm form;

	while (Utils::getSignal() != SIGINT)
	{
		resetUserForm(form);
		connectUser();
		recieveMessage(form);
		analyseCommand(form);
		executeCommand(form);
	}
}
//Loop End


//Getters Begin
IUser* Server::getUserByName(std::string name) {
	IterUser	begin;
	IterUser	end;

	if (!name.size()) return (NULL); //name has no character
	begin = users.begin();
	end = users.end();
	while (begin != end)
	{
		if (!name.compare(begin->second->getNickName()))
			return reinterpret_cast<IUser*>(begin->second);
		begin++;
	}
	return (NULL); //name could not found
}

IChannel* Server::getChannelByName(std::string name) {
	IterChannel iter;

	iter = channels.find(name);
	if (iter != channels.end())
		return (reinterpret_cast<IChannel*>(iter->second)); // Could found
	return(NULL); //Couldn't found
}

std::string Server::getPassword() const {
	return (password);
}

std::string Server::getNicknamesFromChannel(std::string channel_name) {
	return (channels.find(channel_name)->second->getUsersNickNames(false));
}

std::string Server::getUsersFromChannel(std::string channel_name) {
	IterChannel iter;
	std::string	string;
	std::string mode;

	iter = channels.find(channel_name);
	if (iter == channels.end()) return (""); //have not this channel

	if (iter->second->getMode() & Channel::MODE_I) mode += 'i';
	if (iter->second->getMode() & Channel::MODE_K) mode += 'k';
	if (iter->second->getMode() & Channel::MODE_L) mode += 'l';
	if (iter->second->getMode() & Channel::MODE_M) mode += 'm';
	if (iter->second->getMode() & Channel::MODE_N) mode += 'n';

	string = channel_name + "(" + mode + ") :";
	string +=  iter->second->getUsersNickNames();

	return (string);
}

std::string Server::getUsersFromAllChannels() {
	std::string msg;
	IterChannel iter;

	iter = channels.begin();
	while (iter != channels.end()) {
		msg += getUsersFromChannel(iter->first);
		iter++;
		if (iter != channels.end()) msg += '\n';
	}
	return (msg);
}
//Getters End


//Destroy Begin
void Server::destroyServer() {
	while (users.size()) {
		dissconnectUser(users.begin()->first);
	}
	destroyCommands();
	password.clear();
}
//Destroy End
