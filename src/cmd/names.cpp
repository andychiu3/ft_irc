#include "Server.hpp"

void	Server::names(Client *client, const std::vector<std::string> &tokens) {
	if (tokens.size() == 1) {
		std::map<std::string, Channel *>::iterator	chan_it = _channels.begin();
		for (; chan_it != _channels.end(); chan_it++) {
			Channel	*chan = chan_it->second;
			client->send_message(RPL_NAMREPLY(client->get_nickname(), chan));
		}
		client->send_message(RPL_ENDOFNAMES(client->get_nickname(), "*"));
	}

	if (tokens.size() == 2) {
		std::vector<std::string> channel_name = split(tokens[1], ',');
		for (size_t i = 0; i < channel_name.size(); i++) {
			if (_channels.find(channel_name[i]) != _channels.end()) {
				Channel	*channel = _channels[channel_name[i]];
				client->send_message(RPL_NAMREPLY(client->get_nickname(), channel));
				client->send_message(RPL_ENDOFNAMES(client->get_nickname(), channel->get_name()));
			}
		}
	}
}
