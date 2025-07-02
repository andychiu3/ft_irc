#include "Server.hpp"

// invite	nickname	channel
// 0           1           2    
// RPL_AWAY??
void	Server::invite(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.size() < 3)
		return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), tokens[0]));

	Client	*target = get_client_by_nick(tokens[1]);
	if (!target)
		return client->send_message(ERR_NOSUCHNICK(client->get_nickname(), tokens[1]));

	std::string	channel_name = tokens[2];
	if (_channels.find(channel_name) == _channels.end()) {
		target->send_message(RPL_INVITATION(client, target->get_nickname(), channel_name));
		client->send_message(RPL_INVITING(channel_name, target->get_nickname()));
		return ;
	}

	Channel	*channel = _channels[channel_name];
	if (!channel->is_member(client->get_fd()))
		return client->send_message(ERR_NOTONCHANNEL(channel_name));
	if (!channel->is_operator(client->get_fd()))
		return client->send_message(ERR_CHANOPRIVSNEEDED(client->get_nickname(), channel_name));
	if (channel->is_member(target->get_fd()))
		return client->send_message(ERR_USERONCHANNEL(target->get_nickname(), channel_name));
	target->send_message(RPL_INVITATION(client, target->get_nickname(), channel_name));
	client->send_message(RPL_INVITING(channel_name, target->get_nickname()));
	channel->add_invite(target->get_fd());
}
