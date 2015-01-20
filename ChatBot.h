/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChatBot.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gudepard <gudepard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/11 21:47:34 by gudepard          #+#    #+#             */
/*   Updated: 2014/05/13 02:33:53 by gudepard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHATBOT_H
# define CHATBOT_H

# include <QString>
# include <QUrl>
# include <QUrlQuery>
# include <QQmlComponent>
# include <QNetworkAccessManager>

# include "LocalHTTPServer.h"

class ChatRoom;

static const auto CACHE_FILE = "token.cache";
static const auto CLIENT_ID = "asxw0n4xye4j6i847867zr640d7q0e8";
static const auto CLIENT_SECRET = "kjge7m2qm4wh5fi2j2ellkor5aihbyh";
static const auto REDIRECT_URI = "http://localhost:6969";
static const auto AUTH_URL = QUrl(QString(
								"https://api.twitch.tv/kraken/oauth2/authorize?"
								"response_type=code&"
								"client_id=%1&"
								"redirect_uri=%2&"
								"scope=chat_login")
								.arg(CLIENT_ID, REDIRECT_URI));
static const auto POST_URL = QUrl("https://api.twitch.tv/kraken/oauth2/token");
static const auto AUTH_PAYLOAD = [](const QString & code)
{
	QUrlQuery	params;

	params.addQueryItem("client_id", CLIENT_ID);
	params.addQueryItem("client_secret", CLIENT_SECRET);
	params.addQueryItem("grant_type", "authorization_code");
	params.addQueryItem("redirect_uri", REDIRECT_URI);
	params.addQueryItem("code", code.toUtf8());

	return params.toString(QUrl::FullyEncoded).toUtf8();
};

class ChatBot: public QQmlComponent
{
	Q_OBJECT

	typedef QQmlListProperty<ChatRoom>	ChatRooms;

	Q_PROPERTY(QString username READ username WRITE setUsername)
	Q_PROPERTY(QQmlListProperty<ChatRoom> chatrooms READ chatrooms)

	Q_CLASSINFO("DefaultProperty", "chatrooms")

	public:
		ChatBot();

		QString				token() const;

		QString				username() const;
		void				setUsername(const QString &);

		ChatRooms			chatrooms();

		Q_SIGNAL void		authenticated();
		Q_INVOKABLE void	authenticate(bool fromCache = true);

	private:
		void				fetchToken(const QString & code);
		void				ready();

		QString	access_token_;
		QString	username_;

		QList<ChatRoom *>	chatrooms_;

		QNetworkAccessManager	accessManager_;

		std::unique_ptr<LocalHTTPServer>	localServer_;
};

#endif
