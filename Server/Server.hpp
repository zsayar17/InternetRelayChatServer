#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include "../Utils/Types.hpp"
#include "../Socket/Socket.hpp"
#include "../Channel/Channel.hpp"
#include "../User/User.hpp"
#include "../Command/Command.hpp"

//if user and password did not entried make it entried
class IServer
{
	public:
		virtual ~IServer() {};

		//Channel Features
		virtual void createChannel(std::string channel_name) = 0;
		virtual void closeChannel(std::string channel_name) = 0;
		virtual IChannel* getChannelByName(std::string name) = 0;

		//User Features
		virtual void dissconnectUser(int user_id) = 0;
		virtual IUser* getUserByName(std::string name) = 0;

		//Getters
		virtual std::string getPassword() const = 0;
		virtual std::string getUsersFromChannel(std::string channel_name) = 0;
		virtual std::string getUsersFromAllChannels() = 0;
		virtual std::string getNicknamesFromChannel(std::string channel_name) = 0;
};

class Server : public IServer
{
	private:
		//Variables
		Socket		sock;
		ContUser	users;
		ContChannel	channels;
		ContCommand	commands;
		std::string	password;

		//Hidden Chaplin's Form
		Server();
		Server(const Server& other);
		Server& operator=(const Server& other);

		//Connect User Method
		void connectUser();

		//Private Destroy Methods
		void destroyEmptyChannels();
		void destroyCommands();

		//Private Communication Methods
		void recieveMessage(UserForm& form);
		void resetUserForm(UserForm& form);
		bool acceptHexChatCommand(UserForm& form);

		//Commands Methods
		void analyseCommand(UserForm& form);
		void executeCommand(UserForm& form);
		void initCommands();

	public:
		//Explicit Choplin's Form Methods
		Server(uint16_t port, std::string password, std::string server_ip = "0.0.0.0", int max_connect_count = 12);
		~Server();

		//Create-Destroy Feautures
		void createChannel(std::string channel_name);
		void closeChannel(std::string channel_name);
		void dissconnectUser(int user_id);

		//Getters
		IUser* getUserByName(std::string name);
		IChannel* getChannelByName(std::string name);
		std::string getPassword() const;
		std::string getUsersFromChannel(std::string channel_name);
		std::string getUsersFromAllChannels();
		std::string getNicknamesFromChannel(std::string channel_name);

		//Loop Method
		void serverLoop();

		//Destroy Method
		void destroyServer();
};

/////////std::ostream& operator<<(std::ostream& file, Server& serv);

#endif
