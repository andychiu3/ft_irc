/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 10:54:32 by fiftyblue         #+#    #+#             */
/*   Updated: 2025/03/20 15:56:52 by fiftyblue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include "Replies.hpp"

/* ERROR */
std::string	RPL_CHANNELMODEIS(const std::string& nick, Channel *channel) {
	std::string	reply = ":ft_irc 324 " + nick + " " + channel->get_name();
	std::string	tmp;
	std::string	param;
	if (channel->is_invite_only())
		tmp += "i";
	if (channel->is_topic_restricted())
		tmp += "t";
	if (channel->has_key()) {
		tmp += "k";
		param += " " + channel->get_key();
	}
	if (channel->has_user_limit()) {
		tmp += "l";
		std::ostringstream	oss;
		oss << channel->get_user_limit();
		param += " " + oss.str();
	}
	if (!tmp.empty())
		reply += " +" + tmp;
	return reply + "\r\n";
}

// RFC 1459
std::string	ERR_TOOMANYTARGETS(const std::string& nick) {
	return ":ft_irc 407 " + nick + " :Duplicate recipients. No message delivered\r\n";
}

std::string	ERR_NORECIPIENT(const std::string& nick, const std::string& cmd) {
	return ":ft_irc 411 " + nick + " :No recipient given " + cmd + "\r\n";
}

std::string	ERR_NOTEXTTOSEND(const std::string& nick) {
	return ":ft_irc 412 " + nick + " :No text to send\r\n";
}

std::string	ERR_UNKNOWNCOMMAND(const std::string &nick, const std::string& cmd)
{
	return ":ft_irc 421 " + nick + " " + cmd + " :Unknown command\r\n";
}

std::string	ERR_ERRONEUSNICKNAME(const std::string &nick)
{
	return ":ft_irc 432 " + nick + " :Erroneous nickname\r\n";
}

std::string	ERR_NICKNAMEINUSE(const std::string &nick)
{
	return ":ft_irc 433 " + nick + " :Nickname is already in use\r\n";
}

std::string	ERR_NOTREGISTERED()
{
	return ":ft_irc 451 * :You have not registered\r\n";
}

std::string	ERR_ALREADYREGISTERED(const std::string &nick)
{
	return ":ft_irc 462 " + nick + " :You may not reregister\r\n";
}

std::string	ERR_PASSWDMISMATCH()
{
	return ":ft_irc 464 * :Password is incorrect\r\n";
}

std::string	ERR_NOSUCHNICK(const std::string& nick, const std::string& target_nick) {
	std::string	reply = ":ft_irc 401 " + nick;
	if (!target_nick.empty())
		reply += " " + target_nick;
	reply += " :No such nick/channel\r\n";
	return reply;
}

std::string	ERR_NOSUCHCHANNEL(const std::string& channel) {
	return ":ft_irc 403 " + channel + " :No such channel\r\n";
}

std::string	ERR_NEEDMOREPARAMS(const std::string& nick, const std::string& cmd) {
	return ":ft_irc 461 " + nick + " " + cmd + " :Not enough parameters\r\n";
}

std::string	ERR_USERNOTINCHANNEL(const std::string& nick, const std::string& channel) {
	return ":ft_irc 441 " + nick + " " + channel + " :They aren't on that channel\r\n";
}

std::string	ERR_NOTONCHANNEL(const std::string& channel) {
	return ":ft_irc 442 " + channel + " :You're not on that channel\r\n";
}

std::string	ERR_USERONCHANNEL(const std::string& nick, const std::string& channel) {
	return ":ft_irc 443 " + nick + " " + channel + " :is already on channel\r\n";
}

std::string	ERR_KEYSET(const std::string& nick, const std::string& channel) {
	return ":ft_irc 467 " + nick + " " + channel + " :Channel key already set\r\n";
}

std::string	ERR_CHANNELISFULL(const std::string& nick, const std::string& channel) {
	return ":ft_irc 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n";
}

std::string	ERR_UNKNOWNMODE(const std::string& c, const std::string& channel) {
	return ":ft_irc 472 " + c + " :is unknown mode char to me for " + channel + "\r\n";
}

std::string	ERR_INVITEONLYCHAN(const std::string& nick, const std::string& channel) {
	return ":ft_irc 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n";
}

std::string	ERR_BADCHANNELKEY(const std::string& nick, const std::string& channel) {
	return ":ft_irc 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n";
}

std::string ERR_BADCHANMASK(const std::string& channel) {
	return ":ft_irc 476 " + channel + " :Bad Channel Mask\r\n";
}

std::string	ERR_CHANOPRIVSNEEDED(const std::string& nick, const std::string& channel) {
	return ":ft_irc 482 " + nick + " " + channel + " :You're not channel operator\r\n";
}

/* REPLY */
std::string	RPL_WELCOME(const std::string &nick, const std::string &username)
{
	return ":ft_irc 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "! " + username + "@ft_irc\r\n";
}

std::string	RPL_YOURHOST(const std::string &nick)
{
	return ":ft_irc 002 " + nick + " :Your host is ft_irc, running version 1.0\r\n";
}

std::string	RPL_CREATED(const std::string &nick)
{
	return ":ft_irc 003 " + nick + " :This server was created on 01/27/2025\r\n";
}

std::string	RPL_MYINFO(const std::string &nick)
{
	return ":ft_irc 004 " + nick + " ft_irc 1.0 itkol\r\n";
}

std::string RPL_LISTSTART(const std::string& nick) {
	return ":ft_irc 321 " + nick + " Channel :Users Name\r\n";
}

std::string RPL_LIST(const std::string& nick, Channel *channel) {
	std::ostringstream	oss;

	oss << ":ft_irc 322 " << nick << " " << channel->get_name() << " " << channel->get_member().size() << " ";
	if (channel->get_topic().empty())
		oss << ":";
	else
		oss << channel->get_topic();
	return oss.str() + "\r\n";
}

std::string RPL_LISTEND(const std::string& nick) {
	return ":ft_irc 323 " + nick + " :End of /LIST\r\n";
}

std::string RPL_NOTOPIC(const std::string& nick, const std::string& channel) {
	return ":ft_irc 331 " + nick + " " + channel + " :No topic is set\r\n";
}

std::string RPL_TOPIC(const std::string& nick, const std::string& channel, const std::string& topic) {
	return ":ft_irc 332 " + nick + " " + channel + " :" + topic + "\r\n";
}

std::string	RPL_INVITING(const std::string& channel_name, const std::string& nick) {
	return ":ft_irc 341 " + channel_name + " " + nick + "\r\n";
}

std::string	RPL_NAMREPLY(const std::string& nick, Channel* channel) {
	std::string	reply = ":ft_irc 353 " + nick + " = " + channel->get_name() + " :";

	const std::map<int, Client *>&	members = channel->get_member();
	for (std::map<int, Client *>::const_iterator	it = members.begin(); it != members.end(); it++) {
		if (it != members.begin())
			reply += " ";
		if (channel->is_operator(it->first))
			reply += "@";
		reply += it->second->get_nickname();
	}
	return reply + "\r\n";
}

std::string RPL_ENDOFNAMES(const std::string& nick, const std::string& channel) {
	return ":ft_irc 366 " + nick + " " + channel + " :End of /NAMES list\r\n";
}

/* BROADCAST */
std::string	RPL_JOIN(Client *client, const std::string& channel_name) {
	return ":" + client->get_prefix() + " JOIN " + channel_name + "\r\n";
}

std::string	RPL_LEAVE(Client *client, const std::string& channel_name, const std::string& msg) {
	std::string	reply = ":" + client->get_prefix() + " PART " + channel_name;
	if (!msg.empty())
		reply += " " + msg;
	return reply + "\r\n";
}

std::string	RPL_INVITATION(Client *client, const std::string& target, const std::string& channel_name) {
	return ":" + client->get_prefix() + " INVITE " + target + " " + channel_name + "\r\n";
}

std::string	RPL_KICK(Client *client, const std::string& channel_name, const std::string& target, const std::string& msg) {
	std::string	reply = ":" + client->get_prefix() + " KICK " + channel_name + " " + target;
	if (!msg.empty())
		reply += " :" + msg;
	return reply + "\r\n";
}

std::string	PRIVMSG_CHANNEL(Client *client, const std::string& channelname, const std::string& msg) {
	return ":" + client->get_prefix() + " PRIVMSG " + channelname + " :" + msg + "\r\n";
}

std::string	PRIVMSG_CLIENT(Client *client, Client *target, const std::string& msg) {
	return ":" + client->get_nickname() + " PRIVMSG " + target->get_nickname() + " :" + msg + "\r\n";
}

std::string	NOTICE_CHANNEL(Client *client, const std::string& channelname, const std::string& msg) {
	return ":" + client->get_prefix() + " NOTICE " + channelname + " :" + msg + "\r\n";
}

std::string	NOTICE_CLIENT(Client *client, Client *target, const std::string& msg) {
	return ":" + client->get_nickname() + " NOTICE " + target->get_nickname() + " :" + msg + "\r\n";
}