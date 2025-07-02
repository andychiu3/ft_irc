#include "Server.hpp"

void	Server::nick(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.size() < 2)
		return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), tokens[0]));
	if (!is_valid_nickname(tokens[1]))
		return client->send_message(ERR_ERRONEUSNICKNAME(tokens[1]));
	if (client->has_registered())
		return client->send_message(ERR_ALREADYREGISTERED(client->get_nickname()));
	if (get_client_by_nick(tokens[1]) != NULL)
		return client->send_message(ERR_NICKNAMEINUSE(tokens[1]));
	client->set_nickname(tokens[1]);
	register_client(client);
}
