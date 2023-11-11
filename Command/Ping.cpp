#include "Command.hpp"

void PingCommand::setCommand(UserForm& form) {
	setCommandWitouthName(form);
}

bool PingCommand::setVariables(UserForm& form) {
	if (form.first.size() < 1) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false);  //invalid password
	}
	return (true);
}

void PingCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	form.second->sendMessageToUser(form.second, message);
}

void PingCommand::execute(IServer& server, UserForm& form) {
	(void)server;
	setVariables(form);

	if (errno) {
		replyError(form);
		return;
	}
	form.first.insert(form.first.begin(), RPL_PONG(Utils::getSorurce(form.second), form.first[0]));
	form.first.erase(form.first.begin() + 1);
	form.second->sendMessageToUser(form.second, form.first);
	std::cout << "PING executed\n";
}
