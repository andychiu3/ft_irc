/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:39:09 by jmaiolo           #+#    #+#             */
/*   Updated: 2025/03/08 09:17:54 by fiftyblue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <poll.h>
#include <string>
#include <unistd.h>

class Client
{
	private:
		struct pollfd	_poll_info;
		std::string		_username;
		std::string		_nickname;
		std::string		_realname;
		std::string		_recv_buffer;
		std::string		_send_buffer;
		bool			_has_password;
		bool			_has_registered;

	public:
		Client(void);
		Client(int fd, std::string username, std::string nickname, std::string realname);
		~Client(void);

		/* getter */
		const std::string&	get_nickname() const;
		const std::string&	get_username() const;
		const std::string&	get_realname() const;
		std::string	get_prefix() const;
		const struct pollfd	&get_poll_info(void) const;

		/* setter */
		void				set_nickname(std::string nickname);
		void				set_username(std::string username);
		void				set_realname(std::string realname);
		void				set_poll_info(const struct pollfd &poll_info);
		const int			&get_fd(void) const;
		std::string			&get_recv_buffer(void);
		void				set_recv_buffer(std::string recv_buffer);
		std::string			&get_send_buffer(void);
		void				set_send_buffer(std::string send_buffer);
		void				send_message(std::string message);
		bool				has_password(void);
		void				set_password(bool has_password);
		bool				has_registered(void);
		void				set_registered(bool has_registered);
};

#endif