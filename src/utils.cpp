/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fiftyblue <fiftyblue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:54:32 by jmaiolo           #+#    #+#             */
/*   Updated: 2025/02/23 11:05:44 by fiftyblue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

/*
	this is used to print any errors, and if needed exit or print the errno
	prints out to std::cerr and has the two ints as flag,
	these both are set to = 1 by default in the .hpp file
*/
void	print_error(std::string prefix, int errno_flag, int exit_flag)
{
	if (errno_flag)
		std::cerr << prefix << ": " << errno << std::endl;
	else
		std::cerr << prefix << std::endl;
	if (exit_flag)
		exit(1);
}

/*
	this is just a util function for tokenise_message()
	it gets passed 2 references to a token and a vector of tokens,
	if the token isnt empty then it pushes it to the vector
	then it clears it for reuse
*/
void	push_token(std::string &token, std::vector<std::string> &tokens)
{
	if (!token.empty())
	{
		tokens.push_back(token);
		token.clear();
	}
}

std::vector<std::string>	split(const std::string& str, char delimiter) {
	std::vector<std::string>	tokens;
	std::string	tmp;

	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] == delimiter) {
			if (!tmp.empty()) {
				tokens.push_back(tmp);
				tmp.clear();
			}
		} else {
			tmp += str[i];
		}
	}
	if (!tmp.empty())
		tokens.push_back(tmp);
	return tokens;
}

std::string	join_with_deli(const std::vector<std::string>& vec, const std::string& delimiter) {
	std::string	result;
	for (size_t i = 0; i < vec.size(); i++) {
		if (i != 0)
			result += delimiter;
		result += vec[i];
	}
	return result;
}

bool	is_valid_nickname(const std::string& nickname)
{
	if (nickname.empty() || nickname.length() > 9)
		return false;
	
	std::string invalid_chars = " ,:!\\#%?";
	for (size_t i = 0; i < invalid_chars.length(); i++) {
		if (nickname.find(invalid_chars[i]) != std::string::npos)
			return false;
	}
	return true;
}
