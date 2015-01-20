/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCSocket.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gudepard <gudepard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/11 22:02:58 by gudepard          #+#    #+#             */
/*   Updated: 2014/05/13 01:59:23 by gudepard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSOCKET_H
# define IRCSOCKET_H

# include <QTcpSocket>

static const auto CONNECT_TIMEOUT = 10000;
static const auto TWITCH_IRC_PORT = quint16(6667);
static const auto TWITCH_CHANNEL_IRC = []()
{
	return QString("irc.twitch.tv");
};

class IRCSocket : public QTcpSocket
{
	Q_OBJECT

	public:
		IRCSocket();
		virtual	~IRCSocket();

		bool			connect(const QString &);

		Q_SIGNAL void	messageReceived(const QString &);

	private:
		Q_SLOT void	onReadyRead();

		QString	buffer_;
};

#endif
