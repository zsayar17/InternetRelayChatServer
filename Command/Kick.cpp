#include "Command.hpp"

void KickCommand::setCommand(UserForm& form) {
	setCommandWithName(form);
	if (form.first.size() > 1) Utils::fixChannelNameFormat(form.first[0]);
}

bool KickCommand::setVariables(IServer& server, UserForm& form, IUser** user) {
	errno = 0;
	if (form.first.size() < 2) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false);//not enough size
	}

	*user = server.getUserByName(form.first[1]);
	if (!*user) {
		errno = ERRNO_ERR_NOSUCHNICK;
		return (false);//invalid user
	}
	else if (!server.getChannelByName(form.first[0])) {
		errno = ERRNO_ERR_NOSUCHCHANNEL;
		return (false); //no such a channel with this name
	}
	return (true);
}

void KickCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	else if (errno == ERRNO_ERR_NOSUCHNICK) message.push_back(ERR_NOSUCHNICK(source, form.first[0]));
	else if (errno == ERRNO_ERR_NOSUCHCHANNEL) message.push_back(ERR_NOSUCHCHANNEL(source, form.first[1]));
	else if (errno == ERRNO_ERR_KICKSELF) message.push_back(ERR_KICKSELF(source, form.first[1]));
	else if (errno == ERRNO_ERR_NOTONCHANNEL) message.push_back(ERR_NOTONCHANNEL(source, form.first[1]));
	else if (errno == ERRNO_ERR_USERNOTINCHANNEL) message.push_back(ERR_USERNOTINCHANNEL(source, form.first[0], form.first[1]));
	else if (errno == ERRNO_ERR_CHANOPRIVSNEEDED) message.push_back(ERR_CHANOPRIVSNEEDED(source, form.first[1]));
	else if (errno == ERRNO_ERR_KICKFELLOWOPERATOR) message.push_back(ERR_KICKFELLOWOPERATOR(source, form.first[0], form.first[1]));
	form.second->sendMessageToUser(form.second, message);
}

void KickCommand::replySuccess(IServer& server, UserForm& form) {
	ContMessage message;
	std::string source;

	message = form.first;
	message.erase(message.begin(), message.begin() + 1);
	source = Utils::getSorurce(form.second);
	message.insert(message.begin(), RPL_KICK(source, form.first[0], form.first[1]));

	form.second->sendMessageToChannel(server.getChannelByName(form.first[0]), message, true);
	form.second->sendMessageToUser(server.getUserByName(form.first[1]), message);
	form.second->sendMessageToUser(form.second, message);
}

void KickCommand::execute(IServer& server, UserForm& form) {
	IUser		*i_user;

	i_user = NULL;
	if (setVariables(server, form, &i_user)) form.second->kickUserFromChannel(i_user, form.first[0]);

	if (errno) {
		replyError(form);
		return;
	}
	replySuccess(server, form);
	std::cout << "KICK work correctly\n";
}
