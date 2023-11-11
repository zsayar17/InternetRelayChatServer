#include "Command.hpp"

void PassCommand::setCommand(UserForm& form) {
	setCommandWitouthName(form);
}

bool PassCommand::setVariables(IServer& server, UserForm& form) {
	errno = 0;
	if (form.first.size() != 1) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false); //invalid command count
	}
	else if (form.second->isRegistered()) {
		errno = ERRNO_ERR_ALREADYREGISTERED;
		return (false); //User Registered Already
	}
	else if (form.first[0].compare(server.getPassword())) {
		errno = ERRNO_ERRserverPassMISMATCH;
		return (false);  //invalid password
	}
	return (true);
}

void PassCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	else if (errno == ERRNO_ERR_ALREADYREGISTERED) message.push_back(ERR_ALREADYREGISTERED(source));
	else if (errno == ERRNO_ERRserverPassMISMATCH) message.push_back(ERRserverPassMISMATCH(source));
	form.second->sendMessageToUser(form.second, message);
}

void PassCommand::replySuccess(UserForm& form) {
	(void)form;
	//ContMessage message;

	//message.push_back(RPL_PASS(Utils::getSorurce(form.second)));
	//form.second->sendMessageToUser(form.second, message);
}

void PassCommand::execute(IServer& server, UserForm& form) {

	if (setVariables(server, form)) form.second->setActive();

	if (errno) {
		replyError(form);
		return;
	}
	//replySuccess(form);
	std::cout << "PASS work correctly\n";
}
