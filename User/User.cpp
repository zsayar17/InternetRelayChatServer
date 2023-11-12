#include "User.hpp"

//Choplin's Form Begin
User::User() {
	sock = NULL;
	user_info.is_active = false;
	user_info.user_id = -1;
}

User::~User() {
	if (sock) delete sock;
	channels.clear();
}

User::User(const User& other) {
	*this = other;
}

User& User::operator=(const User& other) {
	(void)other;
	return (*this);
}
//Choplin's Form End



//Socket
void User::bindSocket(Socket* sock) {
	if (user_info.user_id > 0) return; // already a socket binded !
	this->sock = sock;
	user_info.user_id = sock->getFd();
}
//Socket End



//Message Features Begin
std::string User::reciveMessage() {
	return (sock->reciveMessage());
}

void User::writeMessageToBuffer(std::string message) {
	user_info.message_buffer += message;
}

std::string User::readMessageBuffer() {
	return (user_info.message_buffer);
}

void User::cleanMessageBuffer() {
	user_info.message_buffer.clear();
}
//Message Features Begin


//Login Authentication Begin
void User::setActive() {
	user_info.is_active = true;
}

bool User::isActive() const {
	return (user_info.is_active);
}

bool User::isRegistered() const {
	if (user_info.is_active && user_info.nickname.size() && user_info.username.size())
		return (true);
	return (false);
}
//Login Authentication End



//User Infos Begin
void User::setNickName(CStringRef nickname) {
	user_info.nickname = nickname;
}

void User::setUserName(CStringRef username) {
	user_info.username = username;
}

void User::setRealName(CStringRef realname) {
	user_info.realname = realname;
}

void User::setHostName(CStringRef hostname) {
	user_info.hostname = hostname;
}

int User::getUserId() const {
	return (user_info.user_id);
}

CStringRef User::getNickName() const{
	return (user_info.nickname);
}

CStringRef User::getUserName() const {
	return (user_info.username);
}

CStringRef User::getRealName() const {
	return (user_info.realname);
}

CStringRef User::getHostName() const {
	return (user_info.hostname);
}
//User Infos End



//Log(in/out) Channel Features Begin
void User::joinToChannel(IChannel* channel, CStringRef password) {
	Channel*	b_channel;

	b_channel = dynamic_cast<Channel *>(channel);
	if (!checkJoin(b_channel, password)) return;

	channels[b_channel->getChannelName()] = b_channel;
	b_channel->addUser(this);

	if (b_channel->getUsersCount() == 1) b_channel->setAdmin(this); //if channel created new then set as admin
}

void User::leaveFromChannel(CStringRef channel_name) {
	Channel *channel;

	channel = findJoinedChannel(channel_name);
	if (!checkLeaveFromChannel(channel)) return;

	channel->kickUser(this);

	channels.erase(channels.find(channel_name));
}
//Log(in/out) Channel Features End



//Admin Features Begin
void User::kickUserFromChannel(IUser* kicked_user, CStringRef channel_name) {
	Channel*	channel;
	User*		b_kicked_user;

	b_kicked_user = dynamic_cast<User*>(kicked_user);
	channel = findJoinedChannel(channel_name);
	if (!checkKick(channel, b_kicked_user)) return;

	b_kicked_user->leaveFromChannel(channel_name);
}

void User::addUserToInviteList(IUser* invited_user, CStringRef channel_name) {
	Channel		*channel;
	User		*user;

	user = dynamic_cast<User*>(invited_user);
	channel = findJoinedChannel(channel_name);
	if (!checkInvite(channel, user)) return;

	user->invited_channels[channel->getChannelName()] = channel;
	channel->inviteUser(user);


}

void User::dropUserFromInviteList(CStringRef channel_name) {
	invited_channels.erase(invited_channels.find(channel_name));
}

void User::setChannelMode(CStringRef channel_name, int mode, bool situation) {
	Channel *channel;

	channel = findJoinedChannel(channel_name);
	if (!checkSetChannelMode(channel)) return;

	channel->setMode(mode, situation);
}

void User::setChannelPassword(CStringRef channel_name, CStringRef password) {
	Channel *channel;

	channel = findJoinedChannel(channel_name);
	channel->setPassword(password);
}

void User::setChannelLimitedMemberCount(CStringRef channel_name, int member_count) {
	Channel *channel;

	channel = findJoinedChannel(channel_name);
	channel->setLimitedMemberCount(member_count);
}

void User::setAdminToChannel(CStringRef channel_name, IUser *iuser) {
	Channel *channel;
	User	*user;

	channel = findJoinedChannel(channel_name);
	user = dynamic_cast<User*>(iuser);
	if (!checkSetAdminToChannel(user, channel)) return;

	channel->setAdmin(user);
}
//Admin Features Begin



//Communication Begin
void User::sendMessage(ContMessage messages) {
	sock->sendMessage(messages);
}

void User::sendMessageToUser(IUser* user, ContMessage messages) {
	if (!checkSendMessageToUser(user)) return;
	if (messages.size()) messages.push_back("\r\n");
	dynamic_cast<User*>(user)->sendMessage(messages);
}

void User::sendMessageToChannel(IChannel *ichannel, ContMessage& messages, bool ignore_control) {
	Channel*	channel;

	channel = dynamic_cast<Channel*>(ichannel);
	if (!ignore_control && !checkSendMessageToChannel(channel)) return;
	channel->sendMessageToChannel(this, messages);
}
//Communication End



//Controls Begin
Channel* User::findJoinedChannel(CStringRef channel_name) {
	IterChannel iter;

	iter = channels.find(channel_name);
	if (iter == channels.end()) return (NULL);
	return (iter->second);
}

Channel* User::findInvitedChannel(CStringRef channel_name) {
	IterChannel iter;

	iter = invited_channels.find(channel_name);
	if (iter == invited_channels.end()) return (NULL);
	return (iter->second);
}


bool User::checkJoin(Channel* channel, CStringRef password) {
	int mode;

	//check is user in channel already
	if (findJoinedChannel(channel->getChannelName())) {
		errno = ERRNO_ERR_USERONCHANNEL;
		return (false); // already exist !
	}
	//check modes if they are activated
	mode = channel->getMode();
	if (mode & IChannel::MODE_K && password.compare(channel->getPassword()))
	{
		errno = ERRNO_ERR_BADCHANNELKEY;
		return (false);// wrong password
	}
	else if (mode & IChannel::MODE_L && channel->getUsersCount() + 1 > channel->getLimitedMemberCount())
	{
		errno = ERRNO_ERR_CHANNELISFULL;
		return (false);// full capacity
	}
	else if (mode & IChannel::MODE_I && !findInvitedChannel(channel->getChannelName()))
	{
		errno = ERRNO_ERR_INVITEONLYCHAN;
		return (false);// invited mode activated and user not into invite list
	}
	return (true);
}

bool User::checkKick(Channel *channel, User* kicked_user) {

	if (kicked_user == this) {
		errno = ERRNO_ERR_KICKSELF;
		return (false); // kicked byhimself
	}
	else if (!channel) {
		errno = ERRNO_ERR_NOTONCHANNEL;
		return (false); // is not in channel !
	}
	else if (!kicked_user->findJoinedChannel(channel->getChannelName())) {
		errno = ERRNO_ERR_USERNOTINCHANNEL;
		return (false); // kicked user not in channel !
	}
	else if (!channel->isAdmin(this)) {
		errno = ERRNO_ERR_CHANOPRIVSNEEDED;
		return (false); //not admin
	}
	else if (channel->isAdmin(kicked_user)) {
		errno = ERRNO_ERR_KICKFELLOWOPERATOR;
		return (false); //try kick admin !
	}
	return (true);
}

bool User::checkInvite(Channel *channel, User* user) {
	if (!channel) {
		errno = ERRNO_ERR_NOTONCHANNEL;
		return (false); // is not member of channel
	}
	else if (!channel->isAdmin(this)) {
		errno = ERRNO_ERR_CHANOPRIVSNEEDED;
		return (false); //is not admin
	}
	else if (user->findJoinedChannel(channel->getChannelName())) {
		errno = ERRNO_ERR_USERONCHANNEL;
		return (false); //already in channel
	}
	else if (!(channel->getMode() & IChannel::MODE_I)) {
		//not errno setted because there is no error on this situation
		return (false); // is not invite mode
	}
	else if (user->findInvitedChannel(channel->getChannelName())) {
		//not errno setted because there is no error on this situation
		return (false);//already invited
	}
	return (true);
}

bool User::checkSendMessageToChannel(Channel *channel) {
	int mode;

	mode = channel->getMode();
	if (mode & IChannel::MODE_N && !findJoinedChannel(channel->getChannelName())) {
		errno = ERRNO_ERR_CANNOTSENDTOCHAN;
		return (false); //has not joined to channel mode_n opened
	}
	else if (mode & IChannel::MODE_M && !channel->isAdmin(this)){
		errno = ERRNO_ERR_CANNOTSENDTOCHAN;
		return (false);//modarated mode is active, except admin can not send message
	}
	return (true);
}

bool User::checkSendMessageToUser(IUser *user) {
	if (user != this && !user->isRegistered()) {
		errno = ERRNO_ERR_NOSUCHNICK;
		return (false);
	}
	return (true);
}

bool User::checkSetChannelMode(Channel *channel) {
	if (!channel) {
		errno = ERRNO_ERR_NOTONCHANNEL;
		return (false); //channel could not resolved
	}
	if (!channel->isAdmin(this)) {
		errno = ERRNO_ERR_CHANOPRIVSNEEDED;
		return (false); //is not admin
	}
	return (true);
}

bool User::checkLeaveFromChannel(Channel *channel) {
	if (!channel) {
		errno = ERRNO_ERR_NOTONCHANNEL;
		return (false); //not in channel !
	}
	return (true);
}

bool User::checkSetAdminToChannel(User* user, Channel *channel) {
	if (!user->findJoinedChannel(channel->getChannelName())) {
		errno = ERRNO_ERR_USERNOTINCHANNEL;
		return (false); //who wants to set admin is not member of channel
	}
	if (channel->isAdmin(user)) {
		//its not an error
		return (false); //user is admin already
	}
	return (true);
}
//Controls End

//Inform Begin
void User::informChannel(IChannel *channel, bool self_inform) {
	ContMessage message;
	Channel		*b_channel;

	b_channel = dynamic_cast<Channel *>(channel);
	message.push_back(RPL_NAMREPLY(Utils::getSorurce(this), b_channel->getChannelName(), b_channel->getUsersNickNames()));

	sendMessageToChannel(channel, message, true);
	if (self_inform) sendMessageToUser(this, message);
}
//Inform End


//Destroy User Begin
void User::shutDownUser() {
	ContMessage message;


	while (invited_channels.size())
		invited_channels.begin()->second->dropInvitedUser(this);

	while (channels.size()) {
		message.push_back(RPL_PART(Utils::getSorurce(this), channels.begin()->first));
		sendMessageToChannel(channels.begin()->second, message, true);
		leaveFromChannel(channels.begin()->first);
	}
}
//Destroy User End
