/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gudepard <gudepard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/11 22:18:22 by gudepard          #+#    #+#             */
/*   Updated: 2014/05/12 19:35:04 by gudepard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCPARSER_H
# define IRCPARSER_H

# include <deque>
# include <QRegExp>
# include <QStringList>

namespace IRC
{
	static auto	NICKNAME_REGEX = QRegExp("^:(.*)!.*$");

	struct	Message
	{
		Message();

		QString nickname;
		QString command;
		std::deque<QString> arguments;
		bool isValid;
	};

	Message	parse_message(const QString &);
};

#endif
