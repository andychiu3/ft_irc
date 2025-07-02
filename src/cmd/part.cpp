#include "Server.hpp"

void	Server::part(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.size() < 2) {
		return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), 
		tokens[0]));
	}

	std::vector<std::string>	channels = split(tokens[1], ',');
	for (size_t i = 0; i < channels.size(); i++) {
		const std::string&	channel_name = channels[i];
		if (_channels.find(channel_name) == _channels.end()) {
			client->send_message(ERR_NOSUCHCHANNEL(channel_name));
			continue ;
		}

		Channel*	channel = _channels[channel_name];;
		if (!channel->is_valid_channel_name(channel_name)) {
			client->send_message(ERR_NOSUCHCHANNEL(channel_name));
			continue ;
		}

		int	fd = client->get_fd();
		if (!channel->is_member(fd)) {
			client->send_message(ERR_NOTONCHANNEL(channel_name));
			continue ;
		}
		std::string	reason = (tokens.size() > 2) ? tokens[2] : "";
		channel->broadcast(RPL_LEAVE(client, channel_name, reason));
		if (channel->rm_member(client))
			remove_channel(channel_name);
	}
}
