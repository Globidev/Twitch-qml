/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChatBot.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gudepard <gudepard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/11 21:54:52 by gudepard          #+#    #+#             */
/*   Updated: 2014/05/13 02:28:38 by gudepard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <QDesktopServices>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include "ChatBot.h"
#include "ChatRoom.h"
#include "LocalHTTPServer.h"

ChatBot::ChatBot():
	localServer_(new LocalHTTPServer(6969))
{
	QObject::connect(&accessManager_, &QNetworkAccessManager::finished,
	[&](QNetworkReply *reply)
	{
		auto	json = QJsonDocument::fromJson(reply->readAll()).object();
		access_token_ = json.value("access_token").toString();
		ready();
	});

	QObject::connect(localServer_.get(), &LocalHTTPServer::newQuery,
	[&](const QList<QPair<QString, QString>> & query)
	{
		for (const auto & pair : query)
		{
			if (pair.first == "code")
				return (fetchToken(pair.second));
		}
		qDebug() << "error" << query;
	});
}

void				ChatBot::authenticate(bool fromCache)
{
	if (fromCache)
	{
		QFile	cacheFile { CACHE_FILE };

		if (cacheFile.open(QIODevice::ReadOnly))
		{
			access_token_ = cacheFile.readAll();
			ready();
		}
		else
			fromCache = false;
	}
	if (!fromCache)
		QDesktopServices::openUrl(AUTH_URL);
}

void				ChatBot::fetchToken(const QString & code)
{
	QNetworkRequest			request { POST_URL };

	request.setHeader(QNetworkRequest::ContentTypeHeader,
					  "application/x-www-form-urlencoded");
	accessManager_.post(request, AUTH_PAYLOAD(code));
}

void				ChatBot::ready()
{
	QFile	cacheFile { CACHE_FILE };

	Q_EMIT authenticated();
	if (cacheFile.open(QIODevice::WriteOnly))
		cacheFile.write(access_token_.toUtf8());
	foreach(ChatRoom *room, chatrooms_)
		room->setBot(this);
}

QString				ChatBot::token() const
{
	return (access_token_);
}

QString				ChatBot::username() const
{
	return (username_);
}

void				ChatBot::setUsername(const QString & username)
{
	username_ = username;
}

ChatBot::ChatRooms	ChatBot::chatrooms()
{
	return (ChatRooms(this, chatrooms_));
}
