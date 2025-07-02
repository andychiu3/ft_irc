#include "Server.hpp"

void	Server::join(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.size() < 2) {
		return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), tokens[0]));
	}

	if (tokens[1] == "0") {
		std::vector<std::string>	user_channels = get_client_channels(client);
		if (user_channels.empty())
			return ; // or ERR_??
		std::string	all_chan = join_with_deli(user_channels, ",");
		std::vector<std::string>	new_tokens;
		new_tokens.push_back("PART");
		new_tokens.push_back(all_chan);
		return part(client, new_tokens);
	}
	
	std::vector<std::string>	channels = split(tokens[1], ',');
	std::vector<std::string>	keys;
	if (tokens.size() > 2) {
		keys = split(tokens[2], ',');
	}

	for (size_t i = 0; i < channels.size(); i++) {
		const std::string&	channel_name = channels[i];
		std::string	password = (i < keys.size()) ? keys[i] : "";

		if (!Channel::is_valid_channel_name(channel_name)) {
			client->send_message(ERR_BADCHANMASK(channel_name));
			continue ;
		}
		
		if (_channels.find(channel_name) == _channels.end()) {
			create_channel(channel_name);
		}

		Channel	*channel = _channels[channel_name];
		if (!channel->can_join(client->get_fd(), password)) {
			if (!channel->is_invited(client->get_fd()))
				client->send_message(ERR_INVITEONLYCHAN(client->get_nickname(), channel_name));
			else if (!channel->check_pass(password))
				client->send_message(ERR_BADCHANNELKEY(client->get_nickname(), channel_name));
			else if (channel->is_full())
				client->send_message(ERR_CHANNELISFULL(client->get_nickname(), channel_name));
			continue ;
		}	

		if (!channel->add_member(client))
			return client->send_message(ERR_USERONCHANNEL(client->get_nickname(), channel_name));
		channel->broadcast(RPL_JOIN(client, channel_name));
		if (!channel->get_topic().empty())
			client->send_message(RPL_TOPIC(client->get_nickname(), channel_name, channel->get_topic()));
		else
			client->send_message(RPL_NOTOPIC(client->get_nickname(), channel_name));
		client->send_message(RPL_NAMREPLY(client->get_nickname(), channel));
		client->send_message(RPL_ENDOFNAMES(client->get_nickname(), channel_name));

		if (channel->get_operators().empty())
			channel->add_operator(client);
	}	
}
