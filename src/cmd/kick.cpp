#include "Server.hpp"

// kick	#channel1,#channel2 user1,user2 :msg
// 0           1                 2       3
void	Server::kick(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.size() < 3)
		return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), tokens[0]));
	
	std::vector<std::string>	chans = split(tokens[1], ',');
	std::vector<std::string>	usrs = split(tokens[2],',');
	if (chans.empty() || usrs.empty())
		return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), tokens[0]));
		
	for (size_t i = 0; i < chans.size(); ++i) {
		std::map<std::string, Channel *>::iterator	it = _channels.find(chans[i]);
		Channel	*channel = (it != _channels.end()) ? it->second : NULL;
		if (!channel) {
			client->send_message(ERR_NOSUCHCHANNEL(chans[i]));
			continue ;
		} if (!channel->is_member(client->get_fd())) {
			client->send_message(ERR_NOTONCHANNEL(chans[i]));
			continue ;
		} if (!channel->is_operator(client->get_fd())) {
			client->send_message(ERR_CHANOPRIVSNEEDED(client->get_nickname(), chans[i]));
			continue ;
		}
		for (size_t j = 0; j < usrs.size(); j++) {
			Client	*tmp = channel->get_client_by_nickname(usrs[j]);
			if (!tmp) {
				client->send_message(ERR_USERNOTINCHANNEL(usrs[j], chans[i]));
				continue ;
			}
			std::string	msg = (tokens.size() > 3) ? tokens[3] : "";
			channel->broadcast(RPL_KICK(client, chans[i], tmp->get_nickname(), msg));
			if (channel->rm_member(tmp))
				remove_channel(channel->get_name());
		}
	}
}
