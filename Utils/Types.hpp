#ifndef __TYPES_H__
#define __TYPES_H__

// C++98 libraries
#include <iostream>
#include <map>
#include <algorithm>
#include <utility>
#include <cstring>
#include <cerrno>
#include <vector>
#include <sstream>
#include <climits>

// C libraries
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>

//Error library
#include "Reply.h"

//Class Definitons
class Socket;

class IUser;
class User;

class IChannel;
class Channel;

class ACommand;

class IServer;
class Server;


class Utils;

//Type Definitons
typedef std::map<int, User*> ContUser;
typedef std::map<std::string, Channel*> ContChannel;
typedef std::map<std::string, ACommand*> ContCommand;
typedef std::vector<pollfd> ContPoll;
typedef std::vector<std::string> ContMessage;
typedef std::vector<User*> ContMember;

typedef std::pair<ContMessage, IUser*> UserForm;
typedef std::pair<ContMessage, IChannel*> ChannelForm;


typedef ContUser::iterator IterUser;
typedef ContChannel::iterator IterChannel;
typedef ContCommand::iterator IterCommand;
typedef ContPoll::iterator IterPoll;
typedef ContMessage::iterator IterMessage;
typedef ContMember::iterator IterMember;

typedef const std::string& CStringRef;

//User Attributes
typedef struct
{
	std::string	nickname;
	std::string username;
	std::string realname;
	std::string hostname;
	int		user_id;
	bool	is_active;
	std::string message_buffer;
} user_t;

//Mode Attributes
typedef struct {
	int					mode;
	int					member_count;
	ContMember			admins;
	ContMember 			invite_list;
	std::string			password;
} cmode_attr;

#endif
