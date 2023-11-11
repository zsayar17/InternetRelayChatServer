#include "Command.hpp"

void InviteCommand::setCommand(UserForm& form) {
	setCommandWitouthName(form);
	if (form.first.size() == 2) Utils::fixChannelNameFormat(form.first[1]);
}

bool InviteCommand::setVariables(IServer& server, UserForm& form, IUser** user) {

	errno = 0;

	if (form.first.size() != 2) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false);//not enough size
	}

	*user = server.getUserByName(form.first[0]);
	if (!*user) {
		errno = ERRNO_ERR_NOSUCHNICK;
		return (false);//invalid user
	}

	if (!server.getChannelByName(form.first[1])) {
		errno = ERRNO_ERR_NOSUCHCHANNEL;
		return (false);//invalid user
	}
	return (true);
}

void InviteCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	else if (errno == ERRNO_ERR_NOSUCHNICK) message.push_back(ERR_NOSUCHNICK(source, form.first[0]));
	else if (errno == ERRNO_ERR_NOSUCHCHANNEL) message.push_back(ERR_NOSUCHCHANNEL(source, form.first[1]));
	else if (errno == ERRNO_ERR_NOTONCHANNEL) message.push_back(ERR_NOTONCHANNEL(source, form.first[1]));
	else if (errno == ERRNO_ERR_CHANOPRIVSNEEDED) message.push_back(ERR_CHANOPRIVSNEEDED(source, form.first[1]));
	else if (errno == ERRNO_ERR_USERONCHANNEL) message.push_back(ERR_USERONCHANNEL(source, form.first[1]));
	form.second->sendMessageToUser(form.second, message);
}

void InviteCommand::replySuccess(IServer& server, UserForm& form) {
	ContMessage	message;

	message.push_back(RPL_INVITE(Utils::getSorurce(form.second), form.first[1]));
	form.second->sendMessageToUser(server.getUserByName(form.first[0]), message);
}

void InviteCommand::execute(IServer& server, UserForm& form) {
	IUser	 *i_user;

	i_user = NULL;
	if (setVariables(server, form, &i_user)) form.second->addUserToInviteList(i_user, form.first[1]);

	if (errno)
	{
		replyError(form);
		return ;
	}
	replySuccess(server, form);
	std::cout << "INVITE work correctly\n";
}
