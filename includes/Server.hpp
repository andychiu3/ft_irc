/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:38:34 by jmaiolo           #+#    #+#             */
/*   Updated: 2025/03/20 16:28:15 by fiftyblue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>

#include "Config.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "Replies.hpp"

#include <string>
#include <map>
#include <vector>
#include <unistd.h>
#include <signal.h>
#include <algorithm>

class Server
{
	private:
		Config																				*_config;
		struct pollfd																		_poll_info;
		std::map<int, Client *>																_clients;
		std::map<std::string, Channel *>	_channels;
		std::vector<std::map<int, Client *>::iterator>										_delete_queue;
		std::map<std::string, void (Server::*)(Client*, const std::vector<std::string>&)>	_command_map;

	public:
		Server(void);
		Server(std::string port, std::string password);
		~Server(void);

		int		get_listener_socket(Config *config);
		void	accept_new_client(void);
		int		poll_sockets(void);
		void	queue_for_delete(int fd);
		void	delete_queued_clients(void);
		void	process_command(Client *client, const std::vector<std::string> &tokens);
		void	register_client(Client *client);

		void	create_channel(const std::string& channel_name);
		void	remove_channel(const std::string& channel_name);

		void	pass(Client *client, const std::vector<std::string> &tokens);
		void	nick(Client *client, const std::vector<std::string> &tokens);
		void	user(Client *client, const std::vector<std::string> &tokens);
		void	join(Client *client, const std::vector<std::string> &tokens);
		void	notice(Client *client, const std::vector<std::string> &tokens);
		void	privmsg(Client *client, const std::vector<std::string> &tokens);
		void	kick(Client *client, const std::vector<std::string> &tokens);
		void	invite(Client *client, const std::vector<std::string> &tokens);
		void	topic(Client *client, const std::vector<std::string> &tokens);
		void	mode(Client *client, const std::vector<std::string> &tokens);
		void	part(Client *client, const std::vector<std::string> &tokens);
		void	quit(Client *client, const std::vector<std::string> &tokens);

		// adding
		void	list(Client *client, const std::vector<std::string> &tokens);
		void	names(Client *client, const std::vector<std::string> &tokens);
		void	ping(Client *client, const std::vector<std::string> &tokens);

		std::map<int, Client *>	&get_clients(void);
		const struct pollfd		&get_poll_info(void) const;
		std::vector<std::string>	get_client_channels(Client *client);
		Client*	get_client_by_nick(const std::string& nick);
		Channel*	get_channel(const std::string& channelname);
		std::vector<Client *>	get_clients(const std::string& id);
};

#endif