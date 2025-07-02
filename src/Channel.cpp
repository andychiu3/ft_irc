/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:40:55 by jmaiolo           #+#    #+#             */
/*   Updated: 2025/03/20 13:45:36 by fiftyblue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/* constructor */
Channel::Channel()
	: _name("default") {
	_mode.i = false;
	_mode.t = false;
	_mode.k = false;
	_mode.o = false;
	_mode.l = false;
	_mode.password = "";
	_mode.limit = 0;
}

Channel::Channel(const str& name) 
: _name(name) {
	_mode.i = false;
	_mode.t = false;
	_mode.k = false;
	_mode.o = false;
	_mode.l = false;
	_mode.password = "";
	_mode.limit = 0;
}

// seems uesless
Channel::Channel(const str& topic,
				const std::map<int, Client *>& member,
				const std::set<int>& op,
				str password) 
	: _topic(topic), _members(member), _operators(op), _mode() {
		_mode.i = false;
		_mode.t = false;
		_mode.k = false;
		_mode.o = false;
		_mode.l = false;
		_mode.password = password;
		_mode.limit = 0;
}

Channel::~Channel() {}

/* getter */
const str&	Channel::get_name() const {
	return _name;
}
const str&	Channel::get_topic() const {
	return _topic;
}

const std::map<int, Client *>&	Channel::get_member() const {
	return _members;
}

std::set<int>	Channel::get_operators() const {
	return _operators;
}

Client*	Channel::get_client_by_nickname(const std::string& nickname) {
	for (std::map<int, Client *>::iterator it = _members.begin(); it != _members.end(); it++) {
		if (it->second->get_nickname() == nickname)
			return it->second;
	}
	return NULL;
}

const str&	Channel::get_key() {
	return _mode.password;
}

const size_t&	Channel::get_user_limit() {
	return _mode.limit;
}
		
/* is */
bool	Channel::is_member(int fd) const {
	return _members.find(fd) != _members.end();
}

bool	Channel::is_operator(int fd) const {
	return _operators.find(fd) != _operators.end();
}

bool	Channel::check_pass(const str& pass) const {
	return !_mode.k || _mode.password == pass;
}

bool	Channel::is_invited(int fd) const {
	if (!_mode.i)
		return true;
	return _mode.invited.find(fd) != _mode.invited.end();
}

bool	Channel::is_full()const {
	return _mode.l && _members.size() >= _mode.limit;
}

bool	Channel::can_join(int fd, const str& pass) const {
	if (_mode.i && !is_invited(fd))
		return false;
	if (_mode.k && pass != _mode.password)
		return false;
	if (_mode.l && _members.size() >= _mode.limit)
		return false;
	return true;
}

bool	Channel::is_valid_channel_name(const str& name) {
	if (name.empty() || name.length() > 50)
		return false;
	if (name[0] != '#' && name[0] != '&')
		return false;
	for (size_t i = 0; i < name.size(); i++) {
		if (name[i] <= 31 || name[i] >= 127)
			return false;
		if (name[i] == ' ' || name[i] == ',' || name[i] == ':' ||name[i] == '!'
			|| name[i] == '@' || name[i] == '*' || name[i] == '?')
			return false;
	}
	return true;
}

bool	Channel::is_invite_only() const {
	return _mode.i;
}

bool	Channel::is_topic_restricted() const {
	return _mode.t;
}

bool	Channel::has_key() const {
	return _mode.k;
}

bool	Channel::has_user_limit() const {
	return _mode.l;
}

/* setter */
void	Channel::set_topic(const str& topic) {
	_topic = topic;
}

void	Channel::set_invite_only(bool value) {
	_mode.i = value;
	if (value == false)
		_mode.invited.clear();
}

void	Channel::add_invite(int fd) {
	_mode.invited.insert(fd);
}

void	Channel::rm_invite(int fd) {
	_mode.invited.erase(fd);
}

void	Channel::set_topic_restricted(bool value) {
	_mode.t = value;
}

void	Channel::set_key(bool value, const str& password) {
	_mode.k = value;
	if (value)
		_mode.password = password;
	else
		_mode.password.clear();
}

void	Channel::set_user_limit(bool value, size_t limit) {
	if (value && limit > 0) {
		_mode.l = true;
		_mode.limit = limit;
	} else {
		_mode.l = false;
		_mode.limit = 0;
	}
}

bool	Channel::add_member(Client *client) {
	if (!client)
		return true;
		
	int	fd = client->get_fd();
	if (is_member(fd))
		return false;
	_members[fd] = client;
	_mode.invited.erase(fd);
	return true;
}

bool	Channel::rm_member(Client *client) {
	int	fd = client->get_fd();
	if (!is_member(fd))
		return false;
	_members.erase(fd);
	rm_operator(fd);
	return _members.empty();
}

void	Channel::add_operator(Client *client) {
	int	fd = client->get_fd();
	if (!is_member(fd))
		return ;
	if (is_operator(fd))
		return ;
	_operators.insert(fd);
}

void	Channel::rm_operator(int fd) {
	if (!is_operator(fd))
		return ;
	_operators.erase(fd);
}

void	Channel::broadcast(const str& msg) {
	std::map<int, Client *>::iterator	it;
	for (it = _members.begin(); it != _members.end(); it++) {
		it->second->send_message(msg);
	}
}

void	Channel::broadcast_but(Client *client, const str& msg) {
	std::map<int, Client *>::iterator	it;
	for (it = _members.begin(); it != _members.end(); it++) {
		if (it->second != client)
			it->second->send_message(msg);
	}
}
