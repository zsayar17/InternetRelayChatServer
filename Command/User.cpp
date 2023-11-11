#include "Command.hpp"

void UserCommand::setCommand(UserForm& form) {
	setCommandWithName(form);
}

bool UserCommand::setVariables(UserForm& form, std::string& real_name) {
	errno = 0;
	if (form.first.size() < 4) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false);//not enough size
	}
	else if (form.second->isRegistered()) {
		errno = ERRNO_ERR_ALREADYREGISTERED;
		return (false);//already registered
	}

	for (size_t i = 3; i < form.first.size(); i++) {
		real_name += form.first[i];
		if (i != form.first.size() - 1) real_name += ' ';
	}
	return (true);
}

void UserCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	else if (errno == ERRNO_ERR_ALREADYREGISTERED) message.push_back(ERR_ALREADYREGISTERED(source));
	form.second->sendMessageToUser(form.second, message);
}

void UserCommand::replySuccess(UserForm& form) {
	(void)form;
	//ContMessage message;

	//message.push_back(RPL_USER(Utils::getSorurce(form.second)));
	//form.second->sendMessageToUser(form.second, message);
}

void UserCommand::execute(IServer& server, UserForm& form) {
	std::string real_name;

	(void)server;
	if (setVariables(form, real_name)) {
		form.second->setUserName(form.first[0]);
		form.second->setHostName(form.first[1]);
		form.second->setRealName(real_name);
	}

	if (errno) {
		replyError(form);
		return;
	}
	//replySuccess(form);
	std::cout << "USER work correctly\n";
}
