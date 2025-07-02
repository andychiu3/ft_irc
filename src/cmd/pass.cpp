#include "Server.hpp"

void	Server::pass(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.size() < 2)
		return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), tokens[0]));
	if (client->has_password() || client->has_registered())
		return client->send_message(ERR_ALREADYREGISTERED(client->get_nickname()));
	if (tokens[1] != _config->get_password())
	{
		client->send_message(ERR_PASSWDMISMATCH());
		queue_for_delete(client->get_fd());
		return ;
	}
	client->set_password(true);
}
