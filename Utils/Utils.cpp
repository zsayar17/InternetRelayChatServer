#include "Utils.hpp"

int Utils::u_signal = 0;

ContMessage Utils::split(std::string message, char delimiter) {
	ContMessage tokens;
	std::string token;
	std::stringstream stream(message);

	while (std::getline(stream, token, delimiter)) {
		if (token[0]) tokens.push_back(token);
		token.clear();
	}
	return tokens;
}

void Utils::reshapeCommand(std::string& command) {
	//check first character, if it is '/' then remove it
	if (command[0] == '/') command.erase(command.begin());
	if (!command.size()) return;

	//make upper case which are lower cases
	for (size_t i = 0; i < command.size(); i++) {
		if (command[i] >= 'a' && command[i] <= 'z')
			command[i] -= 32;
	}
}

// This method reshape messages as command form.
// make upper case first command which characters are lower case only
// and also delete '\n' and '\r' of last member' last character
void Utils::reshapeMessage(std::string& message) {
	IterMessage iter;

	// delete "\r" and "\n" from last member
	if (message.size() && *(message.end() - 1) == '\n') message.erase(message.end() - 1);
	if (message.size() && *(message.end() - 1) == '\r') message.erase(message.end() - 1);
}

void Utils::fixChannelNameFormat(std::string& str) {
	if (str[0] == '#') return ;
	str.insert(str.begin(), '#');
}

std::string Utils::getSorurce(IUser *user) {
	return (user->getNickName() + "!" + user->getUserName() + "@" + user->getHostName());
}

short Utils::convertToShort(std::string str) {
	double	d;
	char	*endptr;

	d = std::strtod(str.c_str(), &endptr);
	if (*endptr || str.find('.') != str.npos) return (-1);
	if (d > 0xffff || d <= 0) return (-1);
	return ((short)d);
}

int Utils::getSignal() {
	return (u_signal);
}

void Utils::setSignal(int new_signal) {
	u_signal = new_signal;
}

bool Utils::checkComplatedMassage(std::string& message) {
	if (message.size() && *(message.end() - 1) == '\n') return (true);
	if (message.size() && *(message.end() - 1) == '\r') return (true);
	return (false);
}

const char* InvalidArgumentException::what() const throw() {
	return ("Invalid Argument!");
}

const char* BadPortException::what() const throw() {
	return ("Bad Port Argument!");
}
