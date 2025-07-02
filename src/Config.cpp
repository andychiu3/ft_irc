/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaiolo <jmaiolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:42:59 by jmaiolo           #+#    #+#             */
/*   Updated: 2025/02/28 00:08:41 by jmaiolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(void)
{
	_port = "6667";
	_password = "password";
}

Config::Config(std::string port, std::string password)
{
	_port = port;
	_password = password;
}

Config::~Config(void)
{
}

std::string	Config::get_port(void)
{
	return (_port);
}

std::string	Config::get_password(void)
{
	return (_password);
}
