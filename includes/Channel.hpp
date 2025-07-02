/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:40:49 by jmaiolo           #+#    #+#             */
/*   Updated: 2025/03/20 13:42:57 by fiftyblue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <map>
#include <set>

#include "Client.hpp"
#include "utils.hpp"

typedef	std::string str;

class Channel {
	private:
		str	_name;
		str	_topic;
		std::map<int, Client *>	_members;
		std::set<int>	_operators;

		struct channel_mode {
			bool	i;
			bool	t;
			bool	k;
			bool	o;
			bool	l;
			std::set<int>	invited;
			str	password;
			size_t	limit;
		}	_mode;

	public:
		Channel();
		Channel(const str& _name);
		Channel(const str&, const std::map<int, Client *>&, const std::set<int>&, str);
		~Channel();

		// getter
		const str&	get_name() const;
		const str&	get_topic() const;
		const std::map<int, Client *>&	get_member() const;
		std::set<int>	get_operators() const;
		Client	*get_client_by_nickname(const std::string& nickname);
		const str&	get_key();
		const size_t&	get_user_limit();

		// is && checker
		bool	is_member(int fd) const;
		bool	is_operator(int fd) const;
		bool	is_invited(int) const;
		bool	check_pass(const str& pass) const;
		bool	is_full()const;
		bool	can_join(int fd, const str& pass) const;
		static bool	is_valid_channel_name(const str&);
		bool	is_invite_only() const;
		bool	is_topic_restricted() const;
		bool	has_key() const;
		bool	has_user_limit() const;
		
		// setter
		void	set_topic(const str&);
		bool	add_member(Client *client);
		bool	rm_member(Client *client);
		void	add_operator(Client *client);
		void	rm_operator(int fd);
		void	set_invite_only(bool value);
		void	add_invite(int fd);
		void	rm_invite(int fd);
		void	set_topic_restricted(bool value);
		void	set_key(bool value, const str& password = "");
		void	set_user_limit(bool value, size_t limit = 0);

		// functions
		void	broadcast(const str& msg);
		void	broadcast_but(Client *client, const str& msg);
}	;

#endif
