#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include "../Utils/Types.hpp"
#include "../Server/Server.hpp"
#include "../User/User.hpp"
#include "../Channel/Channel.hpp"

class ACommand
{
	protected:
		void setCommandWithName(UserForm& form);
		void setCommandWitouthName(UserForm& form);

	public:
		virtual ~ACommand() {}
		virtual void setCommand(UserForm& form) = 0;
		virtual void execute(IServer& server, UserForm& form) = 0;
};

class PassCommand : public ACommand
{
	private:
		bool setVariables(IServer& server, UserForm& form);
		void replyError(UserForm& form);
		void replySuccess(UserForm& form);

	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class NickCommand : public ACommand
{
	private:
		bool setVariables(IServer& server, UserForm& form);
		void replyError(UserForm& form);
		void replySuccess(UserForm& form);
	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class UserCommand : public ACommand
{
	private:
		bool setVariables(UserForm& form, std::string& real_name);
		void replyError(UserForm& form);
		void replySuccess(UserForm& form);

	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class QuitCommand : public ACommand
{
	private:
		bool setVariables(UserForm& form);
		void replyError(UserForm& form);
		void replySuccess(UserForm& form);
	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class JoinCommand : public ACommand
{
	private:
		bool setVariables(IServer& server, UserForm& form, IChannel** channel, std::string& channel_name);
		void replyError(UserForm& form);
		void replySuccess(IServer& server, UserForm& form);
	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class PartCommand : public ACommand
{
	private:
		bool setVariables(IServer& server, UserForm& form, IChannel** channel);
		void replyError(UserForm& form);
		void replySuccess(IServer& server, UserForm& form);

	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class ModeCommand : public ACommand
{
	private:
		void executeMode(char mode, IServer& server, UserForm& form);
		bool controlMode(std::string& mode, bool& is_active);
		bool setVariables(IServer& server, UserForm& form, std::string& mode, bool& active);
		void replyError(UserForm& form);
		void replySuccess(IServer& server, UserForm& form);
	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class InviteCommand : public ACommand
{
	private:
		bool setVariables(IServer& server, UserForm& form, IUser** user);
		void replyError(UserForm& form);
		void replySuccess(IServer& server, UserForm& form);
	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class KickCommand : public ACommand
{
	private:
		bool setVariables(IServer& server, UserForm& form, IUser** user);
		void replyError(UserForm& form);
		void replySuccess(IServer& server, UserForm& form);
	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class PrivMsgCommand : public ACommand
{
	private:
		bool setVariables(IServer& server, UserForm& form, IUser** user, IChannel** channel);
		void replyError(UserForm& form);
		void replySuccess(IServer& server, UserForm& form);
	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class NoticeCommand : public ACommand
{
	private:
		bool setVariables(IServer& server, UserForm& form, IUser** user);
		void replyError(UserForm& form);
		void replySuccess(IServer& server, UserForm& form);
	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class NamesCommand : public ACommand
{
	private:
		bool setVariables(IServer& server, UserForm& form, std::string& msg);
		void replyError(UserForm& form);
	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

class PingCommand : public ACommand
{
	private:
		bool setVariables(UserForm& form);
		void replyError(UserForm& form);
	public:
		void setCommand(UserForm& form);
		void execute(IServer& server, UserForm& form);
};

#endif
