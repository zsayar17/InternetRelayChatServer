#include "Command.hpp"

void JoinCommand::setCommand(UserForm& form) {
	setCommandWitouthName(form);
	if (form.first.size()) Utils::fixChannelNameFormat(form.first[0]);
}

bool JoinCommand::setVariables(IServer& server, UserForm& form, IChannel** channel, std::string& channel_name) {
	errno = 0;

	if (form.first.size() != 1 && form.first.size() != 2) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false);//not enough size
	}

	channel_name = form.first[0];
	*channel = server.getChannelByName(channel_name);
	return (true);
}

void JoinCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	else if (errno == ERRNO_ERR_USERONCHANNEL) message.push_back(ERR_USERONCHANNEL(source, form.first[0]));
	else if (errno == ERRNO_ERR_BADCHANNELKEY) message.push_back(ERR_BADCHANNELKEY(source, form.first[0]));
	else if (errno == ERRNO_ERR_CHANNELISFULL) message.push_back(ERR_CHANNELISFULL(source, form.first[0]));
	else if (errno == ERRNO_ERR_INVITEONLYCHAN) message.push_back(ERR_INVITEONLYCHAN(source));
	form.second->sendMessageToUser(form.second, message);
}

void JoinCommand::replySuccess(IServer& server, UserForm& form) {
	ContMessage	message;

	message.push_back(RPL_JOIN(Utils::getSorurce(form.second), form.first[0]));
	form.second->sendMessageToChannel(server.getChannelByName(form.first[0]), message, true);
	form.second->sendMessageToUser(form.second, message);

	form.second->informChannel(server.getChannelByName(form.first[0]), true);
}

void JoinCommand::execute(IServer& server, UserForm& form) {
	IChannel* 	channel;
	std::string channel_name;

	channel = NULL;
	if (!setVariables(server, form, &channel, channel_name)) goto REPLY;

	if (!channel) goto CREATENEW;
	if (form.first.size() >= 2) form.second->joinToChannel(channel, form.first[1]); //try join with password
	else form.second->joinToChannel(channel); //try join without password
	goto REPLY;

	CREATENEW:
	server.createChannel(channel_name);
	channel = server.getChannelByName(channel_name);
	form.second->joinToChannel(channel);

	REPLY:
	if (errno) {
		replyError(form);
		return ;
	}
	replySuccess(server, form);
	std::cout << "JOIN work correctly\n";
}
