#include "Server.hpp"

// topic #channel :msg
// 0      1        2(optional)
void	Server::topic(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.size() < 2)
		return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), tokens[0]));
	
	std::map<std::string, Channel *>::iterator	it = _channels.find(tokens[1]);
	Channel	*channel = (it != _channels.end()) ? it->second : NULL;
	if (!channel)
		return client->send_message(ERR_NOSUCHCHANNEL(tokens[1]));
	if (!channel->is_member(client->get_fd()))
		return client->send_message(ERR_NOTONCHANNEL(channel->get_name()));

	if (tokens.size() == 2) {
		return client->send_message(channel->get_topic().empty() ?
		RPL_NOTOPIC(client->get_nickname(), tokens[1]) : RPL_TOPIC(client->get_nickname(), tokens[1], channel->get_topic()));
	}

	if (tokens.size() == 3) {
		if (channel->is_topic_restricted() && !channel->is_operator(client->get_fd()))
			return client->send_message(ERR_CHANOPRIVSNEEDED(client->get_nickname(), tokens[1]));
		if (!tokens[2].empty()) {
			if (tokens[2] == ":") {
				channel->set_topic("");
			} else if (tokens[2][0] == ':' && tokens[2].size() > 1) {
				channel->set_topic(tokens[2].substr(1));
			} else
				return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), tokens[0]));
		}
	}
}
