#ifndef __USER_HPP__
#define __USER_HPP__

#include "../Utils/Types.hpp"
#include "../Socket/Socket.hpp"
#include "../Channel/Channel.hpp"

//set admin
class IUser
{
	public:
		virtual ~IUser() {}

		//Login Authentication
		virtual bool isActive() const = 0;
		virtual bool isRegistered() const = 0;
		virtual void setActive() = 0;

		//User Infos
		virtual void setNickName(CStringRef nickname) = 0;
		virtual void setUserName(CStringRef username) = 0;
		virtual void setRealName(CStringRef realname) = 0;
		virtual void setHostName(CStringRef hostname) = 0;
		virtual CStringRef getNickName() const = 0;
		virtual CStringRef getUserName() const = 0;
		virtual CStringRef getRealName() const = 0;
		virtual CStringRef getHostName() const = 0;
		virtual int getUserId() const = 0;

		//Admin Features
		virtual void kickUserFromChannel(IUser* kicked_user, CStringRef channel_name) = 0;
		virtual void addUserToInviteList(IUser* invited_user, CStringRef channel_name) = 0;
		virtual void setChannelMode(CStringRef channel_name, int mode, bool situation) = 0;
		virtual void setChannelPassword(CStringRef channel_name, CStringRef password) = 0;
		virtual void setChannelLimitedMemberCount(CStringRef channel_name, int member_count) = 0;
		virtual void setAdminToChannel(CStringRef channel_name, IUser *user) = 0;

		//In/Out Feautures
		virtual void joinToChannel(IChannel* channel, CStringRef password = "") = 0;
		virtual void leaveFromChannel(CStringRef channel_name) = 0;

		//information
		virtual void informChannel(IChannel *channel, bool self_inform = false) = 0;

		//Communication
		virtual void sendMessageToUser(IUser* user, ContMessage messages) = 0;
		virtual void sendMessageToChannel(IChannel *channel, ContMessage& messages, bool ignore_control = false) = 0;
};

class User : public IUser
{
	private:
		//Variables
		ContChannel	channels;
		ContChannel invited_channels;
		Socket		*sock;
		user_t		user_info;

		//Hidden Choplin's Form Methods
		User(const User& other);
		User& operator=(const User& other);

		//Communication
		void sendMessage(ContMessage messages);

		//Controls
		Channel* findJoinedChannel(CStringRef channel_name);
		Channel* findInvitedChannel(CStringRef channel_name);
		bool checkJoin(Channel* channel, CStringRef password);
		bool checkKick(Channel *channel, User* kicked_user);
		bool checkInvite(Channel *channel, User* user);
		bool checkSendMessageToChannel(Channel *channel);
		bool checkSendMessageToUser(IUser *user);
		bool checkSetChannelMode(Channel *channel);
		bool checkLeaveFromChannel(Channel *channel);
		bool checkSetAdminToChannel(User* user, Channel *channel);

		//information
		void informChannel(IChannel *channel, bool self_inform = false);

	public:
		//Explicit Choplin's Form Methods
		User();
		~User();

		//Socket
		void bindSocket(Socket* sock);

		//Message Features
		std::string reciveMessage();
		void writeMessageToBuffer(std::string message);
		std::string readMessageBuffer();
		void cleanMessageBuffer();

		//Login Authentication
		void setActive();
		bool isActive() const;
		bool isRegistered() const;

		//User Infos
		void setNickName(CStringRef nickname);
		void setUserName(CStringRef username);
		void setRealName(CStringRef realname);
		void setHostName(CStringRef hostname);
		int getUserId() const;
		CStringRef	getNickName() const;
		CStringRef	getUserName() const;
		CStringRef	getRealName() const;
		CStringRef	getHostName() const;

		//Log(in/out) Channel Features
		void joinToChannel(IChannel* channel, CStringRef password = "");
		void leaveFromChannel(CStringRef channel_name);

		//Admin Features
		void kickUserFromChannel(IUser* kicked_user, CStringRef channel_name);
		void setChannelMode(CStringRef channel_name, int mode, bool situations);
		void setAdminToChannel(CStringRef channel_name, IUser *user);
		void setChannelPassword(CStringRef channel_name, CStringRef password);
		void setChannelLimitedMemberCount(CStringRef channel_name, int member_count);
		void addUserToInviteList(IUser* invited_user, CStringRef channel_name);
		void dropUserFromInviteList(CStringRef channel_name);

		//Communication
		void sendMessageToUser(IUser* user,ContMessage messages);
		void sendMessageToChannel(IChannel *channel, ContMessage& messages, bool ignore_control = false);


		void shutDownUser();
};

#endif
