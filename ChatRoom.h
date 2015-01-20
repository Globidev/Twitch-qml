/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChatRoom.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gudepard <gudepard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/11 22:11:39 by gudepard          #+#    #+#             */
/*   Updated: 2014/05/13 02:29:05 by gudepard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHATROOM_H
# define CHATROOM_H

# include <QString>
# include <QQmlComponent>

class ChatBot;
class IRCSocket;

static const auto AUTHENTICATE = [](const QString & token,
									const QString & nick)
{
	return QString("PASS oauth:%1\nNICK %2").arg(token, nick);
};
static const auto JOIN_CHANNEL = [](const QString & channel)
{
	return QString("JOIN #%1").arg(channel);
};
static const auto LEAVE_CHANNEL = [](const QString & channel)
{
	return QString("PART #%1").arg(channel);
};
static const auto PRIVATE_MESSAGE = [](const QString & channel,
									   const QString & message)
{
	return QString("PRIVMSG #%1 :%2").arg(channel, message);
};

class ChatRoom: public QQmlComponent
{
	Q_OBJECT

	Q_PROPERTY(QString channel READ channelName WRITE setChannelName)
	Q_PROPERTY(bool joinOnReady READ joinOnReady WRITE setJoinOnReady)

	public:
		ChatRoom();
		~ChatRoom();

		ChatRoom(const ChatRoom &) = delete;
		ChatRoom & operator=(const ChatRoom &) = delete;

		void				write(const QString &);
		Q_INVOKABLE void	sendMessage(const QString &);
		Q_SIGNAL void		message(const QString & sender,
									const QString & message);

		QString				channelName() const;
		void				setChannelName(const QString & channelName);
		bool				joinOnReady() const;
		void				setJoinOnReady(bool joinOnReady);
		void				setBot(ChatBot *bot);

		Q_SIGNAL void		ready();
		Q_INVOKABLE bool	join();
		Q_INVOKABLE void	leave();

	private:
		std::unique_ptr<IRCSocket>	socket_;
		QString						channel_name_;
		ChatBot						*chatBot_;
		bool						joinOnReady_;
};

#endif
