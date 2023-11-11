#include "Command.hpp"

void PartCommand::setCommand(UserForm& form) {
	setCommandWitouthName(form);
	if (form.first.size()) Utils::fixChannelNameFormat(form.first[0]);
}

bool PartCommand::setVariables(IServer& server, UserForm& form, IChannel** channel) {
	errno = 0;
	if (form.first.size() < 1) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false); //invalid command count
	}
	*channel = server.getChannelByName(form.first[0]);
	if (!*channel) {
		errno = ERRNO_ERR_NOSUCHCHANNEL;
		return (false); //no such a channe with this name
	}
	return (true);
}

void PartCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	else if (errno == ERRNO_ERR_NOSUCHCHANNEL) message.push_back(ERR_NOSUCHCHANNEL(source, form.first[0]));
	else if (errno == ERRNO_ERR_NOTONCHANNEL) message.push_back(ERR_NOTONCHANNEL(source, form.first[0]));
	form.second->sendMessageToUser(form.second, message);
}

void PartCommand::replySuccess(IServer& server, UserForm& form) {
	ContMessage message;

	form.second->informChannel(server.getChannelByName(form.first[0]));
	if (form.first.size() > 1) {
		message = form.first;
		message.erase(message.begin());
		message.insert(message.begin(), RPL_PART(Utils::getSorurce(form.second), form.first[0]));
	}
	else message.push_back(RPL_PART(Utils::getSorurce(form.second), form.first[0]));
	form.second->sendMessageToUser(form.second, message);
	form.second->sendMessageToChannel(server.getChannelByName(form.first[0]), message, true);
}

void PartCommand::execute(IServer& server, UserForm& form) {
	IChannel *channel;

	channel = NULL;
	if (setVariables(server, form, &channel)) form.second->leaveFromChannel(form.first[0]);

	if (errno) {
		replyError(form);
		return ;
	}

	replySuccess(server, form);

	if (!channel->getUsersCount()) {
		server.closeChannel(form.first[0]);
	}
	std::cout << "PART work correctly\n";
}
