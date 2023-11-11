#include "Command.hpp"

void NamesCommand::setCommand(UserForm& form) {
	setCommandWitouthName(form);
	if (form.first.size()) Utils::fixChannelNameFormat(form.first[1]);
}

bool NamesCommand::setVariables(IServer& server, UserForm& form, std::string& msg) {

	errno = 0;
	if (form.first.size() > 1) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false); //need more params
	}

	if (!form.first.size()) {
		msg = server.getUsersFromAllChannels();
		return (true);
	}
	msg = server.getUsersFromChannel(form.first[1]);
	if (msg.empty()) {
		errno = ERRNO_ERR_NOSUCHCHANNEL;
		return (false); //channel could not resolved;
	}
	return (true);
}

void NamesCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	else if (errno == ERRNO_ERR_NOSUCHCHANNEL) message.push_back(ERR_NOSUCHCHANNEL(source, form.first[1]));
	form.second->sendMessageToUser(form.second, message);
}

void NamesCommand::execute(IServer& server, UserForm& form) {
	std::string msg;

	setVariables(server, form, msg);

	if (errno) {
		replyError(form);
		return ;
	}

	form.first.clear();
	form.first.push_back(msg);
	form.second->sendMessageToUser(form.second, form.first);
	std::cout << "names executed\n";
}
