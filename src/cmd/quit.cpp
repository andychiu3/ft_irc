#include "Server.hpp"

void	Server::quit(Client *client, const std::vector<std::string> &tokens)
{
	std::string	message;
	std::cout << tokens.size();

	if (tokens.size() == 2) //more than 1 parameter just uses default quit message
		message = tokens[1];
	if (message.empty())
		message = client->get_nickname() + " has quit";

	std::map<std::string, Channel *>::iterator	chan_it = _channels.begin();
	for (; chan_it != _channels.end(); chan_it++)
	{
		Channel	*channel = chan_it->second;
		if (channel->is_member(client->get_fd()))
			channel->broadcast(":" + client->get_prefix() + " QUIT :" + message + "\r\n");
	}
	queue_for_delete(client->get_fd());
}
