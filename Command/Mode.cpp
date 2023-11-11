#include "Command.hpp"

void ModeCommand::setCommand(UserForm& form) {
	setCommandWitouthName(form);
	if (form.first.size()) Utils::fixChannelNameFormat(form.first[0]);
}

bool ModeCommand::controlMode(std::string& mode, bool& is_active) {

	if (mode[0] == '-') is_active = false;
	else if (mode[0] == '+') is_active = true;
	else goto INVALIDSHAPE; //not begin with + or -
	mode.erase(mode.begin());
	if (!mode.size()) goto INVALIDSHAPE; //only "+" or "-"

	for (size_t i = 0; i < mode.size(); i++) {
		if (!Channel::getFlag(mode[i])) goto INVALIDSHAPE; //contain a invalid flag
	}
	return (true);

	INVALIDSHAPE:
	errno = ERRNO_ERR_UMODEUNKNOWNFLAG;
	return(false);
}

bool ModeCommand::setVariables(IServer& server, UserForm& form, std::string& mode, bool& active) {
	errno = 0;
	if (form.first.size() < 1 || form.first.size() > 3) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return (false); //need more params
	}


	if (!server.getChannelByName(form.first[0])) {
		errno = ERRNO_ERR_NOSUCHCHANNEL;
		return (false);//channel is not exist
	}

	if (form.first.size() == 1) return (false); //Hexchat send mode after join

	mode = form.first[1];
	if (!controlMode(mode, active)) return (false);

	return (true);
}

void ModeCommand::executeMode(char mode, IServer& server, UserForm& form) {
	IUser	*user;
	int		count;

	if (errno) return;

	if (form.first.size() != 3 && (mode == 'o' || mode == 'l' || mode == 'k')) {
		errno = ERRNO_ERR_NEEDMOREPARAMS;
		return ; //need more params
	}
	else if (mode == 'o') {
		user = server.getUserByName(form.first[2]);
		if (!user) {
			errno = ERRNO_ERR_NOSUCHNICK; //user could not found
			return ;
		}
		form.second->setAdminToChannel(form.first[0], user);
	}
	else if (mode == 'l') {
		count = Utils::convertToShort(form.first[2]);
		if (count <= 0) {
			errno = ERRNO_ERR_INVALIDCHANNELMODE;
			form.second->setChannelMode(form.first[0], Channel::MODE_I, false);
			return ; //invalid max member count
		}
		form.second->setChannelLimitedMemberCount(form.first[0], count);
	}
	else if (mode == 'k') {
		form.second->setChannelPassword(form.first[0], form.first[2]);
	}
}

void ModeCommand::replyError(UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (errno == ERRNO_ERR_NEEDMOREPARAMS) message.push_back(ERR_NEEDMOREPARAMS(source, "command"));
	else if (errno == ERRNO_ERR_NOSUCHCHANNEL) message.push_back(ERR_NOSUCHCHANNEL(source, form.first[0]));
	else if (errno == ERRNO_ERR_UMODEUNKNOWNFLAG) message.push_back(ERR_UMODEUNKNOWNFLAG(source, form.first[1]));
	else if (errno == ERRNO_ERR_NOSUCHNICK) message.push_back(ERR_NOSUCHNICK(source, form.first[3]));
	else if (errno == ERRNO_ERR_INVALIDCHANNELMODE) message.push_back(ERR_INVALIDCHANNELMODE(source, form.first[0]));
	else if (errno == ERRNO_ERR_NOTONCHANNEL) message.push_back(ERR_NOTONCHANNEL(source, form.first[0]));
	else if (errno == ERRNO_ERR_CHANOPRIVSNEEDED) message.push_back(ERR_CHANOPRIVSNEEDED(source, form.first[0]));
	else if (errno == ERRNO_ERR_USERNOTINCHANNEL) message.push_back(ERR_USERNOTINCHANNEL(source, form.first[3], form.first[0]));
	form.second->sendMessageToUser(form.second, message);
}

void ModeCommand::replySuccess(IServer& server, UserForm& form) {
	ContMessage message;
	std::string source;

	source = Utils::getSorurce(form.second);
	if (form.first.size() >= 3)
		message.push_back(RPL_MODE(source, form.first[0], form.first[1], form.first[2]));
	else
		message.push_back(RPL_MODE(source, form.first[0], form.first[1], ""));
	form.second->sendMessageToChannel(server.getChannelByName(form.first[0]), message, true);
	form.second->sendMessageToUser(form.second, message);
}

void ModeCommand::execute(IServer& server, UserForm& form) {

	std::string	mode;
	bool		active;

	if (setVariables(server, form, mode, active)) {
		for (size_t i = 0; i < mode.size() && !errno; i++) {
			form.second->setChannelMode(form.first[0], Channel::getFlag(mode[i]), active);
			if (active) executeMode(mode[i], server, form);
		}
	}

	if (errno)
	{
		replyError(form);
		return;
	}
	if (form.first.size() > 1) replySuccess(server, form);
	std::cout << "MODE work correctly\n";
}
