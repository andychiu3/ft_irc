/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaiolo <jmaiolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:42:53 by jmaiolo           #+#    #+#             */
/*   Updated: 2025/02/26 16:04:44 by jmaiolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

class Config
{
	private:
		std::string	_port;
		std::string	_password;
	public:
		Config(void);
		Config(std::string port, std::string password);
		~Config(void);

		std::string	get_port(void);
		std::string	get_password(void);
};

#endif