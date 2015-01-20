/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gudepard <gudepard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/11 22:18:24 by gudepard          #+#    #+#             */
/*   Updated: 2014/05/12 19:35:10 by gudepard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "IRCParser.h"

IRC::Message::Message() : isValid(true)
{

}

IRC::Message IRC::parse_message(const QString & message)
{
	QString	str(message);
	Message	parsed_message;

	parsed_message.isValid = !str.isEmpty();
	if(str[0] == ':')
	{
		auto	splitted = str.split(' ');
		NICKNAME_REGEX.indexIn(splitted[0]);
		parsed_message.nickname = NICKNAME_REGEX.cap(1);
		splitted.removeFirst();
		str = splitted.join(' ');
	}
	if(str.indexOf(" :") != -1)
	{
		auto splitted = str.split(" :");
		for(const auto & s : splitted[0].split(' '))
			parsed_message.arguments.push_back(s);
		parsed_message.arguments.push_back(splitted[1]);
	}
	else
	{
		for(const auto & s : str.split(' '))
			parsed_message.arguments.push_back(s);
	}
	parsed_message.command = parsed_message.arguments.front();
	parsed_message.arguments.pop_front();
	return (parsed_message);
}
