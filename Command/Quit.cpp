#include "Command.hpp"

void QuitCommand::setCommand(UserForm& form) {
	setCommandWitouthName(form);
}

bool QuitCommand::setVariables(UserForm& form) {
	errno = 0;
	if (form.first.size()) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false);  //invalid password
	}
	return (true);
}

void QuitCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	form.second->sendMessageToUser(form.second, message);
}

void QuitCommand::replySuccess(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	message.push_back(RPL_QUIT(source, form.first[0]));
	form.second->sendMessageToUser(form.second, message);
}

void QuitCommand::execute(IServer& server, UserForm& form) {
	setVariables(form);
	if (errno) {
		replyError(form);
		return ;
	}
	replySuccess(form);
	std::cout << "QUIT work correctly\n";
	server.dissconnectUser(form.second->getUserId());
}
