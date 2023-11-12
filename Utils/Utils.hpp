#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "../User/User.hpp"

class Utils
{
	private:
		static int u_signal;

	public:
		static int getSignal();
		static void setSignal(int new_signal);

		static ContMessage split(std::string message, char delimiter);
		static void reshapeCommand(std::string& command);
		static void reshapeMessage(std::string& message);
		static void fixChannelNameFormat(std::string& str);
		static std::string getSorurce(IUser *user);
		static short convertToShort(std::string str);
		static bool checkComplatedMassage(std::string& message);
};

class BadPortException : public std::exception {
	public:
		const char *what() const throw();
};

class InvalidArgumentException : public std::exception {
	public:
		const char *what() const throw();
};

#endif
