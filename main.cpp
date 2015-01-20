/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gudepard <gudepard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/11 21:42:43 by gudepard          #+#    #+#             */
/*   Updated: 2014/05/13 02:41:45 by gudepard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <QApplication>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>

#include "ChatBot.h"
#include "ChatRoom.h"

static const auto QML_FILE_PATH = "chat.qml";

int		main(int argc, char **argv)
{
	QApplication		app { argc, argv };
	QFileSystemWatcher	watcher;
	QQuickView			view;

	qmlRegisterType<ChatBot>("GlobiTwitch", 1, 0, "ChatBot");
	qmlRegisterType<ChatRoom>("GlobiTwitch", 1, 0, "ChatRoom");

	auto reload_file = [&]()
	{
		view.engine()->clearComponentCache();
		view.setSource(QUrl::fromLocalFile(QML_FILE_PATH));
		watcher.addPath(QML_FILE_PATH);
	};

	QObject::connect(&watcher, &QFileSystemWatcher::fileChanged, reload_file);
	QObject::connect(view.engine(), &QQmlEngine::quit,
					 &app, &QApplication::quit);
	reload_file();
	view.show();
	return (app.exec());
}
