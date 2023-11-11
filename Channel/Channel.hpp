#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "../Utils/Types.hpp"
#include "../User/User.hpp"

class IChannel
{
	public:
		virtual ~IChannel() {}

		//modes
		static const int MODE_O = 1 << 0; //give oper permission //+1
		static const int MODE_N = 1 << 1; //external message
		static const int MODE_I = 1 << 2; //invite only
		static const int MODE_M = 1 << 3; //only modarated could send message
		static const int MODE_L = 1 << 4; //set limited channel member count //+1
		static const int MODE_K = 1 << 5; //password for join channel //+1

		virtual int getUsersCount() = 0;
		virtual int getMode() = 0;
};

class Channel : public IChannel
{
	private:
		//Variables
		ContMember	users;
		std::string	channel_name;
		cmode_attr	mode_attr;

		//Hidden Choplin's Form Methods
		Channel(const Channel& other);
		Channel& operator=(const Channel& other);

	public:
		//Explicit Choplin's Form Methods
		Channel();
		~Channel();

		//Init-Destroy
		void initChannel(CStringRef channel_name);
		void shutDownChannel();

		//Channel Info
		std::string getUsersNickNames(bool getfeatures = true);
		CStringRef getChannelName();
		int getUsersCount();

		//In/Out Feautures
		void addUser(User *user);
		void kickUser(User *user);

		//Communication
		void sendMessageToChannel(User* user, ContMessage& messages);

		//Mode
		void setAdmin(User* user);
		void setMode(int mode, bool situation);
		void setPassword(CStringRef password);
		void setLimitedMemberCount(int member_limit_count);
		void inviteUser(User *user);
		void dropInvitedUser(User *user);
		int getMode();
		int getLimitedMemberCount() const;
		bool isAdmin(User* user);
		CStringRef getPassword() const;
		static int getFlag(char flag);
};

#endif
