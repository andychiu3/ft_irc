#include "Server.hpp"

void	Server::user(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.size() < 5)
		return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), tokens[0]));
	if (client->has_registered())
		return client->send_message(ERR_ALREADYREGISTERED(client->get_nickname()));

	std::string	username = tokens[1];
	std::string	realname = tokens[4];

	if (username.length() > 9)
		username = username.substr(0, 9);

	client->set_username(username);
	client->set_realname(realname);
	register_client(client);
}