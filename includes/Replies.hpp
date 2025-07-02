/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 10:54:32 by fiftyblue         #+#    #+#             */
/*   Updated: 2025/03/20 14:36:17 by fiftyblue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	REPLIES
# define REPLIES

#include <string>
#include <sstream>

#include "Channel.hpp"

/* ERROR */
std::string	ERR_TOOMANYTARGETS(const std::string& nick);
std::string	ERR_NORECIPIENT(const std::string& nick, const std::string& cmd);
std::string	ERR_NOTEXTTOSEND(const std::string& nick);
std::string	ERR_NEEDMOREPARAMS(const std::string& nick, const std::string& cmd);
std::string	ERR_USERNOTINCHANNEL(const std::string& nick, const std::string& channel);
std::string	ERR_NOTONCHANNEL(const std::string& channel);
std::string	ERR_USERONCHANNEL(const std::string& nick, const std::string& channel);
std::string	ERR_KEYSET(const std::string& nick, const std::string& channel);
std::string	ERR_NOSUCHNICK(const std::string& nick, const std::string& target_nick);
std::string	ERR_NOSUCHCHANNEL(const std::string& channel);
std::string	ERR_CHANNELISFULL(const std::string& nick, const std::string& channel);
std::string	ERR_UNKNOWNMODE(const std::string& c, const std::string& channel);
std::string	ERR_INVITEONLYCHAN(const std::string& nick, const std::string& channel);
std::string	ERR_BADCHANNELKEY(const std::string& nick, const std::string& channel);
std::string	ERR_BADCHANMASK(const std::string& channel);
std::string	ERR_CHANOPRIVSNEEDED(const std::string& nick, const std::string& channel);
std::string	ERR_UNKNOWNCOMMAND(const std::string &nick, const std::string& cmd);
std::string	ERR_ERRONEUSNICKNAME(const std::string &nick);
std::string	ERR_NICKNAMEINUSE(const std::string &nick);
std::string	ERR_NOTREGISTERED();
std::string	ERR_ALREADYREGISTERED(const std::string &nick);
std::string	ERR_PASSWDMISMATCH();

/* REPLY */
std::string	RPL_CHANNELMODEIS(const std::string& nick, Channel *channel);
std::string	RPL_WELCOME(const std::string &nick, const std::string &username);
std::string	RPL_YOURHOST(const std::string &nick);
std::string	RPL_CREATED(const std::string &nick);
std::string	RPL_MYINFO(const std::string &nick);
std::string	RPL_LISTSTART(const std::string& nick);
std::string	RPL_LIST(const std::string& nick, Channel *channel);
std::string	RPL_LISTEND(const std::string& nick);
std::string	RPL_NOTOPIC(const std::string& nick, const std::string& channel);
std::string	RPL_TOPIC(const std::string& nick, const std::string& channel, const std::string& topic);
std::string	RPL_INVITING(const std::string& channel_name, const std::string& nick);
std::string	RPL_NAMREPLY(const std::string& nick, Channel* channel);
std::string	RPL_ENDOFNAMES(const std::string& nick, const std::string& channel);

/* BROADCAST */
std::string	RPL_JOIN(Client *client, const std::string& channel_name);
std::string	RPL_LEAVE(Client *client, const std::string& channel_name, const std::string& msg);
std::string	RPL_INVITATION(Client *client, const std::string& target, const std::string& channel_name);
std::string	RPL_KICK(Client *client, const std::string& channel_name, const std::string& target, const std::string& msg);
std::string	PRIVMSG_CHANNEL(Client *client, const std::string& channelname, const std::string& msg);
std::string	PRIVMSG_CLIENT(Client *client, Client *target, const std::string& msg);
std::string	NOTICE_CHANNEL(Client *client, const std::string& channelname, const std::string& msg);
std::string	NOTICE_CLIENT(Client *client, Client *target, const std::string& msg);

#endif
