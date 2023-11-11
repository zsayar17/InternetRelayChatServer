#include "Channel.hpp"

//Explicit Choplin's Form Methods Begin
Channel::Channel() {
	mode_attr.mode = 0;
}

Channel::~Channel() {
	users.clear();
}

Channel::Channel(const Channel& other) {
	(void)other;
}

Channel& Channel::operator=(const Channel& other) {
	(void)other;
	return (*this);
}
//Explicit Choplin's Form Methods End


//Init-Destroy Begin
void Channel::initChannel(CStringRef channel_name) {
	this->channel_name = channel_name;
}

void Channel::shutDownChannel() {
	while (users.size())
		(*users.begin())->leaveFromChannel(channel_name);

	while (mode_attr.invite_list.size())
		dropInvitedUser(*mode_attr.invite_list.begin());
}
//Init-Destroy End


//Channel Info Begin
int Channel::getUsersCount() {
	return (users.size());
}

CStringRef Channel::getChannelName() {
	return (channel_name);
}

std::string Channel::getUsersNickNames(bool getfeatures) {
	std::string users_text;
	IterMember iter;

	iter = users.begin();
	while (iter != users.end())
	{
		if (iter != users.begin()) users_text += " ";
		if (isAdmin(*iter)) users_text += "@";
		users_text += (*iter)->getNickName();
		iter++;
	}

	if (!getfeatures || !(getMode() & Channel::MODE_I)) return (users_text);
	iter = mode_attr.invite_list.begin();
	while (iter != mode_attr.invite_list.end())
	{
		users_text += " (i)" + (*iter)->getNickName();
		iter++;
	}
	return (users_text);
}
//Channel Info End



//In/Out Features Begin
void Channel::addUser(User *user) {
	IterMember iter;

	users.push_back(user);
	//if invite mode is activated then drop user from invited list
	if (!(getMode() & Channel::MODE_I)) return; //not invite mode
	iter = std::find(mode_attr.invite_list.begin(), mode_attr.invite_list.end(), user);
	if (iter != mode_attr.invite_list.end()) dropInvitedUser(user);
}

void Channel::kickUser(User *user) {
	IterMember admin_iter;
	IterMember user_iter;

	user_iter = std::find(users.begin(), users.end(), user);
	users.erase(user_iter);

	admin_iter = std::find(mode_attr.admins.begin(), mode_attr.admins.end(), user);
	if (admin_iter != mode_attr.admins.end()) { //if who kicked is admin create new admin
		mode_attr.admins.erase(admin_iter);
		if (!mode_attr.admins.size() && users.size()) //if there is no admin left anymore
			setAdmin(*(users.begin()));
	}
}
//In/Out Features Begin


//Communication Begin
void Channel::sendMessageToChannel(User* user, ContMessage& messages) {
	IterMember iter;

	iter = users.begin();
	while (iter != users.end())
	{
		if (*iter == user) goto ITER; // do not send to itself
		user->sendMessageToUser(*iter, messages);

		ITER:
		iter++;
	}
}
//Communication End


//Mode Begin
void Channel::setMode(int mode, bool situation)
{
	IterMember iter;

	if(situation) mode_attr.mode |= mode;
	else mode_attr.mode &= ~mode;

	if (!(!situation && mode & MODE_I)) return ; //pass to dont clear invite list

	iter = mode_attr.invite_list.begin();
	while (iter != mode_attr.invite_list.end())
	{
		dropInvitedUser(*iter);
		iter = mode_attr.invite_list.begin();
	}
}

void Channel::setAdmin(User* user) {
	mode_attr.admins.push_back(user);
}

void Channel::setPassword(CStringRef password) {
	mode_attr.password = password;
}

void Channel::setLimitedMemberCount(int member_limit_count) {
	mode_attr.member_count = member_limit_count;
}

void Channel::inviteUser(User *user) {
	mode_attr.invite_list.push_back(user);
}

void Channel::dropInvitedUser(User *user) {
	IterMember iter;

	if (!user) return; // null value
	iter = std::find(mode_attr.invite_list.begin(), mode_attr.invite_list.end(), user);
	if (iter == mode_attr.invite_list.end()) return ; //not in invited list
	(*iter)->dropUserFromInviteList(getChannelName());
	mode_attr.invite_list.erase(iter);
}

bool Channel::isAdmin(User* user) {
	IterMember iter;

	if (!user) return (false); //null value
	iter = std::find(mode_attr.admins.begin(), mode_attr.admins.end(), user);
	if (iter == mode_attr.admins.end()) return (false); //non admin
	return (true); //it's admin
}

int Channel::getMode() {
	return (mode_attr.mode);
}

int Channel::getLimitedMemberCount() const {
	return (mode_attr.member_count);
}

CStringRef Channel::getPassword() const {
	return (mode_attr.password);
}

int Channel::getFlag(char flag) {
	if (flag == 'i') return (MODE_I);
	if (flag == 'k') return (MODE_K);
	if (flag == 'l') return (MODE_L);
	if (flag == 'm') return (MODE_M);
	if (flag == 'n') return (MODE_N);
	if (flag == 'o') return (MODE_O);
	return (0);
}
//Mode End
