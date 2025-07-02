#include "Server.hpp"

// does not implement third arg for targeting Server
void	Server::list(Client *client, const std::vector<std::string> &tokens) {
	client->send_message(RPL_LISTSTART(client->get_nickname()));
	if (tokens.size() == 1) {
		for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++) {
			client->send_message(RPL_LIST(client->get_nickname(), it->second));
		}
	}
	
	if (tokens.size() == 2 && !tokens[1].empty()) {
		std::vector<std::string>	channel_name = split(tokens[1], ',');
		for (size_t i = 0; i < channel_name.size(); i++) {
			std::map<std::string, Channel *>::iterator	it = _channels.find(channel_name[i]);
			if (it != _channels.end()) {
				client->send_message(RPL_LIST(client->get_nickname(), it->second));
			}
		}
	}
	client->send_message(RPL_LISTEND(client->get_nickname()));
}
