/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCSocket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gudepard <gudepard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/11 22:03:00 by gudepard          #+#    #+#             */
/*   Updated: 2014/05/13 07:00:46 by gudepard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCSocket.h"

IRCSocket::IRCSocket() : QTcpSocket()
{
	QObject::connect(this, &QTcpSocket::readyRead,
					 this, &IRCSocket::onReadyRead);
}

IRCSocket::~IRCSocket()
{

}

bool IRCSocket::connect(const QString & channel_name)
{
	(void)channel_name;
	connectToHost(TWITCH_CHANNEL_IRC(), TWITCH_IRC_PORT);
	return (waitForConnected(CONNECT_TIMEOUT));
}

void IRCSocket::onReadyRead()
{
	int		endl_index = 0;
	QString	line;

	buffer_ += readAll();
	while ((endl_index = buffer_.indexOf("\n", 0)) != -1)
	{
		line = buffer_.left(endl_index);
		if (line.startsWith("PING "))
			write(QString("PONG " + line.right(5) + "\r\n").toLocal8Bit());
		else
			Q_EMIT messageReceived(line);
		buffer_.remove(0, endl_index + 1);
	}
}
