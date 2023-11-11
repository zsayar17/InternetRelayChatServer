#include "Command.hpp"

void PrivMsgCommand::setCommand(UserForm& form) {
	setCommandWithName(form);
}

bool PrivMsgCommand::setVariables(IServer& server, UserForm& form, IUser** user, IChannel** channel) {
	errno = 0;
	if (form.first.size() < 2) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false); //invalid command count
	}

	if (form.first[0][0] == '#') *channel = server.getChannelByName(form.first[0]);
	else *user = server.getUserByName(form.first[0]);
	if (!(*user) && !(*channel)) {
		errno = ERRNO_ERR_NOSUCHNICK; //there is no channel or nick within this name
		return (false);
	}
	return (true);
}

void PrivMsgCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	else if (errno == ERRNO_ERR_CANNOTSENDTOCHAN) message.push_back(ERR_CANNOTSENDTOCHAN(source, form.first[0]));
	else if (errno == ERRNO_ERR_NOSUCHNICK) message.push_back(ERR_NOSUCHNICK(source, form.first[0]));
	form.second->sendMessageToUser(form.second, message);
}

void PrivMsgCommand::replySuccess(IServer& server, UserForm& form) {
	ContMessage message;
	std::string source;

	message = form.first;
	message.erase(message.begin());
	source = Utils::getSorurce(form.second);
	message.insert(message.begin(), RPL_PRIVMSG(source, form.first[0]));
	message[0] += message[1];
	message.erase(message.begin() + 1);//do not let to put space after ':'

	if (server.getChannelByName(form.first[0]))
		form.second->sendMessageToChannel(server.getChannelByName(form.first[0]), message);
	else
		form.second->sendMessageToUser(server.getUserByName(form.first[0]), message);
}

void PrivMsgCommand::execute(IServer& server, UserForm& form) {
	IChannel	*channel;
	IUser		*user;
	std::string	first_command;

	user = NULL; channel = NULL;
	if (!setVariables(server, form, &user, &channel)) goto REPLY;

	replySuccess(server, form);

	REPLY:
	if (errno) {
		replyError(form);
		return ;
	}
	std::cout << "PRIVMSG work correctly\n";
}
