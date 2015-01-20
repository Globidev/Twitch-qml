/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChatRoom.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gudepard <gudepard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/11 22:11:41 by gudepard          #+#    #+#             */
/*   Updated: 2014/05/13 03:31:53 by gudepard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChatRoom.h"
#include "ChatBot.h"
#include "IRCSocket.h"
#include "IRCParser.h"

ChatRoom::ChatRoom():
	socket_(new IRCSocket), chatBot_(nullptr), joinOnReady_(false)
{
	auto onPrivMessage = [this](const QString & message)
	{
		auto parsed_message = IRC::parse_message(message);
		if (parsed_message.command == "PRIVMSG")
		{
			Q_EMIT this->message(parsed_message.nickname,
								 parsed_message.arguments[1].trimmed());
		}
	};
	QObject::connect(socket_.get(), &IRCSocket::messageReceived, onPrivMessage);
}

ChatRoom::~ChatRoom()
{
	leave();
}

void ChatRoom::write(const QString & command)
{
	socket_->write(QString("%1\n").arg(command).toLocal8Bit());
}

void ChatRoom::sendMessage(const QString & message)
{
	write(PRIVATE_MESSAGE(channel_name_, message));
}

QString	ChatRoom::channelName() const
{
	return (channel_name_);
}

void	ChatRoom::setChannelName(const QString & channelName)
{
	leave();
	channel_name_ = channelName;
}

bool	ChatRoom::joinOnReady() const
{
	return (joinOnReady_);
}

void	ChatRoom::setJoinOnReady(bool joinOnReady)
{
	joinOnReady_ = joinOnReady;
}

void	ChatRoom::setBot(ChatBot *bot)
{
	chatBot_ = bot;
	Q_EMIT ready();
	if (joinOnReady_)
		join();
}

bool	ChatRoom::join()
{
	leave();
	if (!chatBot_)
		return (false);
	if (channel_name_.isEmpty())
		return (false);
	if (socket_->connect(channel_name_))
	{
		write(AUTHENTICATE(chatBot_->token(), chatBot_->username().toLower()));
		write(JOIN_CHANNEL(channel_name_));
		return (true);
	}
	return (false);
}

void	ChatRoom::leave()
{
	if (socket_->state() == QAbstractSocket::ConnectedState)
	{
		write(LEAVE_CHANNEL(channel_name_));
		socket_->disconnectFromHost();
		socket_->waitForDisconnected();
	}
}
