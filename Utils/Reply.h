
// ERROR REPLIES
#define ERR_UNKNOWNCOMMAND(source)															": 421 " + source + " :Unknown command"
#define ERR_NEEDMOREPARAMS(source, command)													": 461 " + source + " " + command + " :Not enough parameters"
#define ERR_NOTREGISTERED(source)															": 451 " + source + " :You have not registered"
#define ERR_ALREADYREGISTERED(source)														": 462 " + source + " :You may not reregister"
#define ERRserverPassMISMATCH(source)														": 464 " + source + " :Password incorrect"
#define ERR_NONICKNAMEGIVEN(source, nickname)												": 431 " + source + " " + nickname + " :Nickname not given"
#define ERRnickNameINUSE(source, nickname)													": 433 " + source + " " + nickname + " :Nickname is already in use"
#define ERR_NOTONCHANNEL(source, channel)													": 442 " + source + " " + channel + " :You're not on that channel"
#define ERR_NOSUCHCHANNEL(source, channel)													": 403 " + source + " " + channel + " :No such channel"
#define ERR_BADCHANNELKEY(source, channel)													": 475 " + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_NOSUCHNICK(source, nickname)													": 401 " + source + " " + nickname + " :No such nick/channel"
#define ERR_USERNOTINCHANNEL(source, nickname, channel)										": 441 " + source + " " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_CHANOPRIVSNEEDED(source, channel)												": 482 " + source + " " + channel + " :You're not channel operator"
#define ERR_CHANNELISFULL(source, channel)													": 471 " + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_CANNOTSENDTOCHAN(source, channel)												": 404 " + source + " " + channel + " :Cannot send to channel"
#define ERR_UMODEUNKNOWNFLAG(source, flag)													": 501 " + source + " " + flag + " :Unknown MODE flag"
#define ERR_INVITEONLYCHAN(source)															": 473 " + source + " :Cannot join channel (+i)"
#define ERR_USERONCHANNEL(source, channel)													": 443 " + source + " " + channel + " :is already on channel"
#define ERR_KICKSELF(source, channel) 														": 482 " + source + " " + channel + " :You cannot kick yourself"
#define ERR_KICKFELLOWOPERATOR(source, operator1, channel) 									": 491 " + source + " " +  operator1 + " " + channel + " :Cannot kick fellow operator"
#define ERR_INVALIDCHANNELMODE(source, channel) 											": 491 " + source  + " " +  channel + " :Invalid channel mode value"


// NUMERIC REPLIES
#define RPL_NAMREPLY(source, channel, users)												": 353 " + source + " = " + channel + " :" + users

// COMMAND REPLIES
#define RPL_JOIN(source, channel)															":" + source + " JOIN :" + channel
#define RPL_PART(source, channel)															":" + source + " PART :" + channel
#define RPL_PING(source, token)																":" + source + " PONG :" + token
#define RPL_PONG(source, token)																":" + source + " PONG :" + token
#define RPL_PRIVMSG(source, target)															":" + source + " PRIVMSG " + target + " :"
#define RPL_NOTICE(source, target)															":" + source + " NOTICE " + target + " :"
#define RPL_QUIT(source, message)															":" + source + " QUIT :Quit: " + message
#define RPL_KICK(source, channel, target)													":" + source + " KICK " + channel + " " + target + " :"
#define RPL_MODE(source, channel, modes, args)												":" + source + " MODE " + channel + " " + modes + " " + args
#define RPL_NICK(source, nickname)															":" + source + " NICK " + nickname
#define RPL_INVITE(source, channel)															":" + source + " INVITE " + " :You have been invited to channel " + channel + " you can type /join to join that channel"

// ERROR NUMBERS
#define ERRNO_ERR_UNKNOWNCOMMAND 0
#define ERRNO_ERR_NEEDMOREPARAMS 1
#define ERRNO_ERR_NOTREGISTERED 2
#define ERRNO_ERR_ALREADYREGISTERED 3
#define ERRNO_ERRserverPassMISMATCH 4
#define ERRNO_ERR_NONICKNAMEGIVEN 5
#define ERRNO_ERRnickNameINUSE 6
#define ERRNO_ERR_NOSUCHNICK 7
#define ERRNO_ERR_NOTONCHANNEL 8
#define ERRNO_ERR_NOSUCHCHANNEL 9
#define ERRNO_ERR_BADCHANNELKEY 10
#define ERRNO_ERR_USERNOTINCHANNEL 11
#define ERRNO_ERR_CHANOPRIVSNEEDED 12
#define ERRNO_ERR_CHANNELISFULL 13
#define ERRNO_ERR_CANNOTSENDTOCHAN 14
#define ERRNO_ERR_UMODEUNKNOWNFLAG 16
#define ERRNO_ERR_INVITEONLYCHAN 18
#define ERRNO_ERR_USERONCHANNEL 19
#define ERRNO_ERR_KICKSELF 20
#define ERRNO_ERR_KICKFELLOWOPERATOR 21
#define ERRNO_ERR_INVALIDCHANNELMODE 21
