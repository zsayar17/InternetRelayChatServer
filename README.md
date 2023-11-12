# InternetRelayChatServer

This C++ project is a small IRC server designed in an object-oriented fashion, specifically tailored for use with the HexChat interface but also available for use on netcat. The project utilizes system socket functions from <sys/socket.h> and polling. The server is implemented as a single process and a single thread.

The server supports essential IRC commands such as mode, invite, join, kick, names, nick, notice, part, pass, ping, privmsg, quit, and user. Additionally, the server includes various channel options like 'l', 'k', 'i', 'm', 'n', 'o', which are controllable by channel operators.
