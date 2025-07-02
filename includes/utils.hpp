/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:54:24 by jmaiolo           #+#    #+#             */
/*   Updated: 2025/02/23 11:01:07 by fiftyblue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Config.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <vector>

void						print_error(std::string prefix, int errno_flag = 1, int exit_flag = 1);
void						push_token(std::string &token, std::vector<std::string> &tokens);
std::vector<std::string>	split(const std::string& str, char delimiter);
std::string					join_with_deli(const std::vector<std::string>& vec, const std::string& delimiter);
bool						is_valid_nickname(const std::string& nickname);

#endif