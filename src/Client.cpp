/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:39:27 by jmaiolo           #+#    #+#             */
/*   Updated: 2025/03/08 09:16:06 by fiftyblue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
{
}

Client::Client(int fd, std::string username, std::string nickname, std::string realname)
{
	_poll_info.fd = fd;
	_poll_info.events = POLLIN;
	_poll_info.revents = 0;
	_username = username;
	_nickname = nickname;
	_realname = realname;
	_has_password = false;
	_has_registered = false;
}

Client::~Client(void)
{
	if (_poll_info.fd >= 0)
		close(_poll_info.fd);
}

/* getter */
const std::string&	Client::get_nickname() const {
	return _nickname;
}

const std::string&	Client::get_username() const {
	return _username;
}

const std::string&	Client::get_realname() const {
	return _realname;
}

std::string	Client::get_prefix() const {
	return _nickname + "!" + _username + "@ft_irc";
}

const struct pollfd	&Client::get_poll_info(void) const
{
	return (_poll_info);
}

void	Client::set_poll_info(const struct pollfd &poll_info)
{
	_poll_info = poll_info;
}

const int	&Client::get_fd(void) const
{
	return (_poll_info.fd);
}

std::string	&Client::get_recv_buffer(void)
{
	return (_recv_buffer);
}

void	Client::set_nickname(std::string nickname)
{
	_nickname = nickname;
}

void	Client::set_username(std::string username)
{
	_username = username;
}

void	Client::set_realname(std::string realname)
{
	_realname = realname;
}

void	Client::set_recv_buffer(std::string recv_buffer)
{
	_recv_buffer = recv_buffer;
}

std::string	&Client::get_send_buffer(void)
{
	return (_send_buffer);
}

void	Client::set_send_buffer(std::string send_buffer)
{
	_send_buffer = send_buffer;
}

/*
	this appends the inputted string to the client's _send_buffer
	then it adds POLLOUT to the client's poll_info events
	POLLOUT only needs to be set when there is data (otherwise it always triggers)
*/
void	Client::send_message(std::string message)
{
	pollfd	tmp = _poll_info;

	if (message.empty())
		return ;
	_send_buffer += message;
	tmp.events = POLLIN | POLLOUT;
	set_poll_info(tmp);
}

bool	Client::has_password(void)
{
	return (_has_password);
}

void	Client::set_password(bool has_password)
{
	_has_password = has_password;
}

bool	Client::has_registered(void)
{
	return (_has_registered);
}

void	Client::set_registered(bool has_registered)
{
	_has_registered = has_registered;
}
