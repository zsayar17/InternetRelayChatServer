#include "Command.hpp"

void NickCommand::setCommand(UserForm& form) {
	setCommandWitouthName(form);
}

bool NickCommand::setVariables(IServer& server, UserForm& form) {
	errno = 0;
	if (form.first.size() != 1) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false); //invalid command count
	}
	if (server.getUserByName(form.first[0])) {
		errno = ERRNO_ERRnickNameINUSE;
		return (false); //already exist name
	}
	return (true);
}

void NickCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	else if (errno == ERRNO_ERRnickNameINUSE) message.push_back(ERRnickNameINUSE(source, form.first[0]));
	form.second->sendMessageToUser(form.second, message);
}

void NickCommand::replySuccess(UserForm& form) {
	ContMessage message;

	message.push_back(RPL_NICK(Utils::getSorurce(form.second), form.first[0]));
	form.second->sendMessageToUser(form.second, message);
}

void NickCommand::execute(IServer& server, UserForm& form) {

	if (setVariables(server, form)) form.second->setNickName(form.first[0]);

	if (errno) {
		replyError(form);
		return ;
	}
	replySuccess(form);
	std::cout << "NICK work correctly\n";
}
