#include "Command.hpp"

void ACommand::setCommandWithName(UserForm& form) {
	ContMessage new_message;

	new_message = Utils::split(form.first[0], ':');

	form.first = Utils::split(new_message[0], ' ');
	form.first.erase(form.first.begin());

	if (new_message.size() > 1) {
		form.first.push_back(*(new_message.end() - 1));
	}
}

void ACommand::setCommandWitouthName(UserForm& form) {
	form.first = Utils::split(form.first[0], ' ');
	form.first.erase(form.first.begin());
}

void answerUser(IUser *user, const char *answer) {
	ContMessage message;

	message.push_back(answer);
	user->sendMessageToUser(user, message);
}
