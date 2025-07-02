#include "Server.hpp"

void	Server::privmsg(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.size() < 2)
		return client->send_message(ERR_NORECIPIENT(client->get_nickname(), tokens[0]));
	if (tokens.size() < 3)
		return client->send_message(ERR_NOTEXTTOSEND(client->get_nickname()));

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
			channel->broadcast_but(client, PRIVMSG_CHANNEL(client, tokens[1], msg));
		else
			client->send_message(ERR_NOSUCHNICK(client->get_nickname(), recipient));
		return ;
	}

	std::vector<Client *> targets = get_clients(recipient);
	if (targets.empty())
		client->send_message(ERR_NOSUCHNICK(client->get_nickname(), recipient));
	else if (targets.size() > 1)
		client->send_message(ERR_TOOMANYTARGETS(client->get_nickname()));
	else
		targets[0]->send_message(PRIVMSG_CLIENT(client, targets[0], msg));	
}
