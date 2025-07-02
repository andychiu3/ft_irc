#include "Server.hpp"

enum	e_sign {
	NONE,
	YES,
	NO,
};

std::vector<std::string>	parse_mode(std::string token) {
	std::vector<std::string>	modes;
	e_sign	sign = NONE;
	
	for (size_t i = 0; i < token.size(); i++) {
		if (token[i] == '+' || token[i] == '-') {
			if (i + 1 >= token.size() || !isalpha(token[i + 1])) {
				modes.clear();
				return modes;
			}
			sign = (token[i] == '+') ? YES : NO;
		} else if (isalpha(token[i])) {
			if (sign == NONE) {
				modes.clear();
				return modes;
			}
			std::string	tmp = (sign == YES ? "+" : "-");
			tmp += token[i];
			modes.push_back(tmp);
		} else {
			modes.clear();
			return modes;
		}
	}
	return modes;
}

void	mode_k_case(std::string mode, Channel *channel, Client *client, const std::vector<std::string> &tokens, size_t &param_i) {
	if (mode[0] == '+') {
		if (param_i >= tokens.size())
			return client->send_message(RPL_CHANNELMODEIS(client->get_nickname(), channel));
		if (!channel->get_key().empty())
			return client->send_message(ERR_KEYSET(client->get_nickname(), channel->get_name()));
		channel->set_key(true, tokens[param_i++]);
	} else
		channel->set_key(false);
}

void	mode_o_case(std::string mode, Channel *channel, Client *client, const std::vector<std::string> &tokens, size_t &param_i) {
	if (param_i >= tokens.size())
		return client->send_message(RPL_CHANNELMODEIS(client->get_nickname(), channel));

	Client	*target = channel->get_client_by_nickname(tokens[param_i]);
	if (!target)
		return client->send_message(ERR_USERNOTINCHANNEL(tokens[param_i], channel->get_name()));
	param_i++;
	if (mode[0] == '+')
		channel->add_operator(target);
	else if (mode[0] == '-' && channel->get_operators().size() > 1)
		channel->rm_operator(target->get_fd());
}

void	mode_l_case(std::string mode, Channel *channel, Client *client, const std::vector<std::string> &tokens, size_t &param_i) {
	if (mode[0] == '+') {
		if (param_i >= tokens.size())
			return client->send_message(RPL_CHANNELMODEIS(client->get_nickname(), channel));
		
		int	nbr = atoi(tokens[param_i++].c_str()); // seems that no specify in RFC 2812 if notANumber
		if (nbr)
			channel->set_user_limit(true, nbr);
	} else
		channel->set_user_limit(false);
}

void	process_mode(std::vector<std::string> modes, Channel *channel, Client *client, const std::vector<std::string> &tokens) {
	size_t	param_i = 3;
	for (size_t	i = 0; i < modes.size(); i++) {
		if (modes[i] == "+i" || modes[i] == "-i")
			channel->set_invite_only(modes[i][0] == '+');
		else if (modes[i] == "+t" || modes[i] == "-t")
			channel->set_topic_restricted(modes[i][0] == '+');
		else if (modes[i] == "+k" || modes[i] == "-k")
			mode_k_case(modes[i], channel, client, tokens, param_i);
		else if (modes[i] == "+o" || modes[i] == "-o")
			mode_o_case(modes[i], channel, client, tokens, param_i);
		else if (modes[i] == "+l" || modes[i] == "-l")
			mode_l_case(modes[i], channel, client, tokens, param_i);
		else
			client->send_message(ERR_UNKNOWNMODE(modes[i].substr(1,1), channel->get_name()));
	}
}

// MODE #channel *(+/-*i) password limit
// 0      1          2      3
// at least 2 but normally needs 3 potentially needs more
void	Server::mode(Client *client, const std::vector<std::string> &tokens)
{
	if (tokens.size() < 2)
		return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), tokens[0]));

	const std::string&	chan_name = tokens[1];
	if (_channels.find(chan_name) == _channels.end())
		return client->send_message(ERR_NOSUCHCHANNEL(chan_name));
	
	Channel	*channel = _channels[chan_name];
	if (tokens.size() < 3)
		return client->send_message(RPL_CHANNELMODEIS(client->get_nickname(), channel));
	if (!channel->is_member(client->get_fd()))
		return client->send_message(ERR_NOTONCHANNEL(chan_name));
	if (!channel->is_operator(client->get_fd()))
		return client->send_message(ERR_CHANOPRIVSNEEDED(client->get_nickname(), chan_name));
	
	std::vector<std::string>	modes = parse_mode(tokens[2]);
	if (modes.empty()) // msg may need to be changed
		return client->send_message(ERR_NEEDMOREPARAMS(client->get_nickname(), tokens[0])); //
	process_mode(modes, channel, client, tokens);
}
