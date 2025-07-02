#include "Server.hpp"

void	Server::notice(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.size() < 2)
		return ;
	if (tokens.size() < 3)
		return ;

	std::string	msg;
	for (size_t i = 2; i < tokens.size(); i++) {
		if (i != 2)
			msg += " ";
		msg += tokens[i];
	}
		
	std::string	recipient = tokens[1];
	if (!recipient.empty() && recipient[0] == '#') {
		Channel	*channel = get_channel(recipient);
		if (channel && channel->is_member(client->get_fd()))
			channel->broadcast_but(client, NOTICE_CHANNEL(client, tokens[1], msg));
		return ;
	}

	std::vector<Client *> targets = get_clients(recipient);
	if (targets.size() == 1)
		targets[0]->send_message(NOTICE_CLIENT(client, targets[0], msg));
}
