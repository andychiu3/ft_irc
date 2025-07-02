/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:38:43 by jmaiolo           #+#    #+#             */
/*   Updated: 2025/03/23 23:54:13 by fiftyblue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
	this is the default constructor; it shouldnt be called but have just incase
	it uses exact same logic as the custom constructor below
*/
Server::Server(void)
{
	_config = new Config("6667", "password");
	_poll_info.fd = get_listener_socket(_config);
	_poll_info.events = POLLIN;
	_poll_info.revents = 0;

	_command_map["PASS"] = &Server::pass;
	_command_map["NICK"] = &Server::nick;
	_command_map["USER"] = &Server::user;
	_command_map["JOIN"] = &Server::join;
	_command_map["NOTICE"] = &Server::notice;
	_command_map["PRIVMSG"] = &Server::privmsg;
	_command_map["KICK"] = &Server::kick;
	_command_map["INVITE"] = &Server::invite;
	_command_map["TOPIC"] = &Server::topic;
	_command_map["MODE"] = &Server::mode;
	_command_map["PART"] = &Server::part;
	_command_map["QUIT"] = &Server::quit;
	_command_map["LIST"] = &Server::list;
	_command_map["NAMES"] = &Server::names;
	_command_map["PING"] = &Server::ping;
}

/*
	this is the custom constructor for the server
	it gets passed the port and password (which is gained from argv)
	it gets and binds a listener socket to get incoming data with get_listener_socket()
	then it populates the _command_map, which is used to map the command methods to strings
*/
Server::Server(std::string port, std::string password)
{
	_config = new Config(port, password);
	_poll_info.fd = get_listener_socket(_config);
	_poll_info.events = POLLIN;
	_poll_info.revents = 0;

	_command_map["PASS"] = &Server::pass;
	_command_map["NICK"] = &Server::nick;
	_command_map["USER"] = &Server::user;
	_command_map["JOIN"] = &Server::join;
	_command_map["NOTICE"] = &Server::notice;
	_command_map["PRIVMSG"] = &Server::privmsg;
	_command_map["KICK"] = &Server::kick;
	_command_map["INVITE"] = &Server::invite;
	_command_map["TOPIC"] = &Server::topic;
	_command_map["MODE"] = &Server::mode;
	_command_map["PART"] = &Server::part;
	_command_map["QUIT"] = &Server::quit;
	_command_map["LIST"] = &Server::list;
	_command_map["NAMES"] = &Server::names;
	_command_map["PING"] = &Server::ping;
}

/* seems that destructor would not be called if we end the program with signal */
Server::~Server(void)
{
	delete _config;

	if (_poll_info.fd >= 0)
		close(_poll_info.fd);
	
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		queue_for_delete(it->first);
	delete_queued_clients();

	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
		delete it->second;
	_channels.clear();
}

/*
	this creates a listener socket and binds it the port in config
	it creates a TCP socket for ipv4, and uses AI_PASSIVE for the address of the host
	it then creates the socket, and binds it using the address info (which had port specified)
	then starts listening on the socket with 24 max connections (random number lol)
	and uses fcntl to set as a non blocking socket (as per the subject pdf)
*/
int	Server::get_listener_socket(Config *config)
{
	int				listener;
	int				opt;
	struct addrinfo	hints, *address_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, config->get_port().c_str(), &hints, &address_info) < 0)
		print_error("getaddrinfo error");
	if (address_info == NULL)
	{
		freeaddrinfo(address_info);
		print_error("getaddrinfo error", 0);
	}

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0)
		print_error("socket creation error");

	opt = 1;
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		print_error("setsockopt error");
	
	if (bind(listener, address_info->ai_addr, address_info->ai_addrlen) < 0)
	{
		freeaddrinfo(address_info);
		print_error("bind error");
	}
	freeaddrinfo(address_info);

	if (listen(listener, 24) < 0)
		print_error("listen error");

	fcntl(listener, F_SETFL, O_NONBLOCK);
	return (listener);
}

/*
	this is used to accept incoming connections / clients
	it accepts the client and gets the socket,
	then sets the socket to non blocking
	if the client socket is valid, it creates a new client object and adds it to the _clients map
*/
void	Server::accept_new_client(void)
{
	int	client_socket;

	client_socket = accept(_poll_info.fd, NULL, NULL);
	fcntl(client_socket, F_SETFL, O_NONBLOCK);
	if (client_socket != -1)
		_clients[client_socket] = new Client(client_socket, "", "", "");
	else
		print_error("accept client error", 1, 0);
}

void	Server::create_channel(const std::string& channel_name) {
	Channel	*new_channel = new Channel(channel_name);
	
	// double checking
	if (!new_channel->is_valid_channel_name(channel_name))
		return delete new_channel;
	if (_channels.find(channel_name) != _channels.end())
		return delete new_channel;
	
	_channels[channel_name] = new_channel;
}

void	Server::remove_channel(const std::string& channel_name) {
	std::map<std::string, Channel *>::iterator	it = _channels.find(channel_name);
	if (it != _channels.end()) {
		delete it->second;
		_channels.erase(it);
	}
}

/*
	this is used for the poll() call throughout the server
	this checks all sockets (including the listener socket) for new events
	it first creates a new pollfd array, then iterates through all clients and
	fills this array with the current pollfd data (the last element in the array is listener)
	then, it calls poll() to check for new events on all the sockets in pollfds
	this updates the poll_info for all the sockets in pollfds
	it then goes over all clients and updates their poll_info with the new one from pollfds
	also does the same for the listener socket
	then it deletes the pollfds array and return's the result of the poll() call
	(this return is amount of sockets that had new events)
*/
int	Server::poll_sockets(void)
{
	int				num_fds = _clients.size() + 1;
	struct pollfd	*pollfds = new struct pollfd[num_fds];
	int				i = 0;
	int				result;

	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		pollfds[i] = it->second->get_poll_info();
		i++;
	}
	pollfds[i] = _poll_info;

	result = poll(pollfds, num_fds, 0);

	i = 0;
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		it->second->set_poll_info(pollfds[i]);
		i++;
	}
	_poll_info.revents = pollfds[i].revents;

	delete[] pollfds;
	return (result);
}

void	Server::queue_for_delete(int fd)
{
	std::map<int, Client *>::iterator	it = _clients.find(fd);
	if (it == _clients.end())
		return ;
		
	if (std::find(_delete_queue.begin(), _delete_queue.end(), it) == _delete_queue.end())
		_delete_queue.push_back(_clients.find(fd));
	//should also send channel PART and QUIT messages?
}

/*
	this goes over the _delete_queue var in the server class,
	it goes over all the clients that are in this, closes their fd,
	then it deletes their data and removes them from the _clients map
	after removing all clients from this queue, it clears the queue
	the queue_for_delete() above is used to add to this delete queue
*/
void	Server::delete_queued_clients(void)
{
	for (size_t i = 0; i < _delete_queue.size(); i++)
	{
		std::map<int, Client *>::iterator it = _delete_queue[i];
		Client	*client = it->second;
		std::cout << "Disconnecting client" << std::endl; //for logging
		std::map<std::string, Channel *>::iterator	chan_it = _channels.begin();
		for (; chan_it != _channels.end(); chan_it++) {
			Channel	*channel = chan_it->second;
			channel->rm_member(client); //returns false if user not in channel
		}
		delete client;
		_clients.erase(it);
	}
	_delete_queue.clear();
}

/*
	this is used to process the incoming commands / messages from clients
	it first checks the tokens parsed, and if theyre empty it just returns
	then sets the command variable to the first token (just for clarity)
	then it checks with .find() if the command is valid / in the _command_map
	otherwise it checks if the client has done PASS for auth (excludes the PASS command)
	otherwise it checks if the client has registered (excludes PASS, NICK, USER)
	if none of these error checks occur, the method is called from the _command_map
*/
void	Server::process_command(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.empty())
		return;

	std::string	command = tokens[0];
	//debug print tokens - remove later
	std:: cout << std::endl;
	for (size_t i = 0; i < tokens.size(); i++)
		std::cout << "tokens[" << i << "]: " << tokens[i] << std::endl;
	//
	if (_command_map.find(command) == _command_map.end())
	{
		//if they try a command that doesnt exist
		client->send_message(ERR_UNKNOWNCOMMAND(client->get_nickname(), command));
		return;
	}
	if (command != "PASS"  && command != "QUIT" && !client->has_password())
	{
		//if they try use commands without password (dont think there is specific response for this?)
		client->send_message(ERR_NOTREGISTERED());
		return;
	}
	if (command != "PASS" && command != "NICK" && command != "USER" && command != "QUIT" && !client->has_registered())
	{
		//if they try use commands without registering
		client->send_message(ERR_NOTREGISTERED());
		return;
	}
	(this->*_command_map[command])(client, tokens);
}

/*
	attempts to register client, checks if client has needed params
	then registers and sends welcome messages, otherwise does nothing
*/
void	Server::register_client(Client *client)
{
	if (client->has_password() && !client->get_nickname().empty() && !client->get_username().empty() && !client->get_realname().empty())
	{
		client->set_registered(true);
		client->send_message(RPL_WELCOME(client->get_nickname(), client->get_username()));
		client->send_message(RPL_YOURHOST(client->get_nickname()));
		client->send_message(RPL_CREATED(client->get_nickname()));
		client->send_message(RPL_MYINFO(client->get_nickname()));
	}
}

std::map<int, Client *>	&Server::get_clients(void)
{
	return (_clients);
}

const struct pollfd	&Server::get_poll_info(void) const
{
	return (_poll_info);
}

std::vector<std::string>	Server::get_client_channels(Client *client) {
	std::vector<std::string>	channels;
	for (std::map<std::string, Channel *>::iterator	it = _channels.begin(); it != _channels.end(); it++) {
		Channel	*channel = it->second;
		if (channel->is_member(client->get_fd()))
			channels.push_back(it->first);
	}
	return channels;
}

Client*	Server::get_client_by_nick(const std::string& nick) {
	for (std::map<int, Client *>::iterator	it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second->get_nickname() == nick)
			return it->second;
	}
	return NULL;
}

Channel*	Server::get_channel(const std::string& channelname) {
	std::map<std::string, Channel *>::iterator	it = _channels.find(channelname);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

std::vector<Client *>	Server::get_clients(const std::string& id) {
	std::vector<Client *>	matches;
	if (id.find('!') == std::string::npos && id.find('@') == std::string::npos) {
		Client	*client = get_client_by_nick(id);
		if (client)
			matches.push_back(client);
	}
	else if (id.find('!') == std::string::npos && id.find('@') != std::string::npos) {
		std::string	usr = id.substr(0, id.find('@'));
		std::string	host = id.substr(id.find('@') + 1);

		for (std::map<int, Client *>::iterator	it = _clients.begin(); it != _clients.end(); it++) {
			if (it->second->get_username() == usr && host == "ft_irc")
				matches.push_back(it->second);
		}
	}
	else if (id.find('!') != std::string::npos && id.find('@') != std::string::npos) {
		std::string	nick = id.substr(0, id.find('!'));
		std::string	usr = id.substr(id.find('!') + 1, id.find('@') - id.find('!') - 1);
		std::string	host = id.substr(id.find('@') + 1);

		// std::cout << nick << " " << usr << " " << host << std::endl;

		for (std::map<int, Client *>::iterator	it = _clients.begin(); it != _clients.end(); it++) {
			if (it->second->get_nickname() == nick 
			&& it->second->get_username() == usr 
			&& host == "ft_irc")
				matches.push_back(it->second);
		}
	}
	return matches;
}
