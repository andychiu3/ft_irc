/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:36:08 by jmaiolo           #+#    #+#             */
/*   Updated: 2025/04/02 11:08:12 by fiftyblue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"

/*
	this gets the incoming messages from a client using recv()
	it attempts to keep reading bytes into the recv_buffer until recv() returns 0 (or errors)
	every time it reads, it null terminates the end of where it read,
	then it adds the read info onto the incoming_data string (which stores the total current data read)
	after the while loop breaks, it adds the incoming_data to the previous_data
	previous_data stores any incomplete messages that werent terminated with \r\n
	after it has all data from the client, it goes over it and searches for \r\n delimiters
	in each loop of this iteration over the total data it adds each letter to partial_data
	when the \r\n is found it pushes the accumulated partial_data to a messages vector
	and then clears the partial_data for the next message
	after the loop is done, it sets the client buffer to the partial_data (what would be previous_data)
	so if a \r\n isnt found by the end (which suggest an unterminated message) it will be stored
*/
std::vector<std::string>	get_incoming_messages(Client *client, Server &server)
{
	std::vector<std::string>	messages;
	char						recv_buffer[1024];
	std::string					incoming_data;
	std::string					previous_data = client->get_recv_buffer();
	std::string					partial_data;
	int							bytes_read;

	while (true)
	{
		bytes_read = recv(client->get_fd(), recv_buffer, 1023, 0);

		if (bytes_read > 0)
		{
			recv_buffer[bytes_read] = '\0';
			incoming_data += recv_buffer;
		}
		else if (bytes_read == 0)
		{
			server.queue_for_delete(client->get_fd());
			return (messages);
		}
		else
		{
			if (errno == EWOULDBLOCK || errno == EAGAIN) // eval check list says no 
				break;
			else
			{
				print_error("recv error", 1, 0);
				server.queue_for_delete(client->get_fd());
				return (messages);
			}
		}
	}
	previous_data += incoming_data;

	for (size_t i = 0; i < previous_data.size(); i++)
	{
		if (previous_data[i] == '\r' && ((i + 1) < previous_data.size()) && previous_data[i + 1] == '\n')
		{
			messages.push_back(partial_data);
			partial_data.clear();
			i++;
		}
		else if (previous_data[i] == '\n') //needed for netcat, but not IRC clients so maybe remove?
		{
			messages.push_back(partial_data);
			partial_data.clear();
		}
		else
			partial_data += previous_data[i];
	}

	if (!partial_data.empty()) //log for eval
	{
		std::cout << std::endl << "storing partial recv()" << std::endl;
		std::cout << partial_data << std::endl;
	}

	client->set_recv_buffer(partial_data);

	return (messages);
}

/*
	this turns an incoming message into different "tokens" to split the command and params
	it uses a while loop to iterate over the message, storing each letter into partial_data
	when a space is found, all current data in partial_data is pushed to the tokens vector
	then it skips over any spaces until it finds a normal character
	if a colon is found, it pushes the partial data to the tokens vector,
	then the rest of the entire message is pushed to the tokens vector
	this all uses push_token() from utils.cpp
*/
std::vector<std::string>	tokenise_message(std::string message)
{
	std::vector<std::string>	tokens;
	std::string					partial_data;
	size_t						i = 0;

	while (i < message.size())
	{
		if (message[i] == ' ')
		{
			push_token(partial_data, tokens);
			while (i < message.size() && message[i] == ' ')
				i++;
			continue;
		}
		else if (message[i] == ':')
		{
			push_token(partial_data, tokens);
			partial_data = message.substr(i + 1);
			push_token(partial_data, tokens);
			break;
		}
		else
			partial_data += message[i++];
	}
	push_token(partial_data, tokens);
	return (tokens);
}

/*
	this is used when a new POLLIN from a client is detected
	it gets their message with get_incoming_messages()
	then it tokenises it with tokenise_message()
	then this message / command is processed and eventually executed with process_command()
*/
void	process_incoming_messages(Client *client, Server &server)
{
	std::vector<std::string>	messages = get_incoming_messages(client, server);

	for (size_t i = 0; i < messages.size(); i++)
	{
		std::vector<std::string>	tokens = tokenise_message(messages[i]);
		server.process_command(client, tokens);
	}
}

/*
	this has similar logic to get_incoming_messages(),
	while the send buffer is not empty, it attempts to send the stored data to the client
	if successful, it erases the sent data from the send buffer,
	on error is deletes the client,
	otherwise once all data for the client has been sent (i.e, the send buffer empty)
	the client's pollfd events are set to just POLLIN again so that POLLOUT doesnt trigger constantly
*/
void	send_outgoing_messages(Client *client, Server &server)
{
	std::string	&send_buffer = client->get_send_buffer();
	int			bytes_sent;
	pollfd		tmp = client->get_poll_info();

	while (!send_buffer.empty())
	{
		bytes_sent = send(client->get_fd(), send_buffer.c_str(), send_buffer.size(), 0);

		if (bytes_sent > 0)
			send_buffer.erase(0, bytes_sent);
		else if (bytes_sent == 0)
		{
			server.queue_for_delete(client->get_fd());
			return ;
		}
		else
		{
			if (errno == EWOULDBLOCK || errno == EAGAIN) // eval check list says no
				break;
			else
			{
				print_error("send error", 1, 0);
				server.queue_for_delete(client->get_fd());
				return ;
			}
		}
	}
	if (send_buffer.empty())
	{
		tmp.events = POLLIN;
		client->set_poll_info(tmp);
	}
}

/*
	this is used as the main loop for the server, it constantly polls all sockets
	and calls functions in response to what events are detected
	if there if a pollin event on the listener socket, it accepts a new client
	if there is a pollin event on a client socket, it processes the incoming messages
	(there isnt any logic right now for sending messages to clients)
*/
void	handle_socket_events(Server &server)
{
	int	num_events;

	num_events = server.poll_sockets();
	if (num_events > 0)
	{
		if (server.get_poll_info().revents & POLLIN)
		{
			num_events--;
			server.accept_new_client();
		}
		for (std::map<int, Client *>::iterator it = server.get_clients().begin(); it != server.get_clients().end() && num_events > 0; it++)
		{
			if (it->second->get_poll_info().revents & POLLIN)
			{
				num_events--;
				process_incoming_messages(it->second, server);
			}
			if (it->second->get_poll_info().revents & POLLOUT)
			{
				num_events--;
				send_outgoing_messages(it->second, server);
			}
		}
	}
	server.delete_queued_clients();
}

volatile sig_atomic_t	running = 1;

void	sig_handler(int signo) {
	if (signo == SIGINT)
		running = 0;
}

/*
	the main just quickly checks for correctly formatted usage of the ./ircserv
	then it creates a server object using the user's inputs and runs handle_socket_events()
*/
int	main(int argc, char **argv)
{
	if (argc != 3)
		print_error("Usage: ./ircserv {port} {password}", 0, 1);

	for (size_t i = 0; i < strlen(argv[1]); i++)
	{
		if (!isdigit(argv[1][i]))
			print_error("Error: port must numeric only!!", 0, 1);
	}
	
	Server server(argv[1], argv[2]);
	signal(SIGINT, sig_handler);
	while (running)
		handle_socket_events(server);
	std::cout << "server shutting down" << std::endl;
	return (0);
}