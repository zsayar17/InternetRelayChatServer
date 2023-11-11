#include "Command.hpp"

void NoticeCommand::setCommand(UserForm& form) {
	setCommandWithName(form);
}

bool NoticeCommand::setVariables(IServer& server, UserForm& form, IUser** user) {
	errno = 0;
	if (form.first.size() < 2) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false); //invalid command count
	}

	*user = server.getUserByName(form.first[0]);
	if (!(*user)) {
		errno = ERRNO_ERR_NOSUCHNICK; //there is no channel or nick within this name
		return (false);
	}
	return (true);
}

void NoticeCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	else if (errno == ERRNO_ERR_NOSUCHNICK) message.push_back(ERR_NOSUCHNICK(source, form.first[0]));
	form.second->sendMessageToUser(form.second, message);
}

void NoticeCommand::replySuccess(IServer& server, UserForm& form) {
	ContMessage message;
	std::string source;

	message = form.first;
	message.erase(message.begin());
	source = Utils::getSorurce(form.second);
	message.insert(message.begin(), RPL_NOTICE(source, form.first[0]));
	message[0] += message[1];
	message.erase(message.begin() + 1);//do not let to put space after ':'

	form.second->sendMessageToUser(server.getUserByName(form.first[0]), message);
}

void NoticeCommand::execute(IServer& server, UserForm& form) {
	IUser		*user;

	user = NULL;
	if (!setVariables(server, form, &user)) goto REPLY;

	replySuccess(server, form);

	REPLY:
	if (errno) {
		replyError(form);
		return ;
	}
	std::cout << "NOTICE work correctly\n";
}
