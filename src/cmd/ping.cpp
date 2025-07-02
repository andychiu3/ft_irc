#include "Server.hpp"

void	Server::ping(Client *client, const std::vector<std::string> &tokens) {
	if (tokens.size() == 2) {
		client->send_message("PONG " + tokens[1]);
	}
}
