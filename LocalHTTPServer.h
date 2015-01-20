/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocalHTTPServer.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gudepard <gudepard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/13 01:45:30 by gudepard          #+#    #+#             */
/*   Updated: 2014/05/13 02:02:54 by gudepard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCALHTTPSERVER_H
# define LOCALHTTPSERVER_H

# include <QTcpServer>

class LocalHTTPServer : public QTcpServer
{
	Q_OBJECT

	public:
		LocalHTTPServer(quint16, QObject * = nullptr);

		Q_SIGNAL void newQuery(const QList<QPair<QString, QString>> &);

	protected:
		virtual void incomingConnection(qintptr);

	private:
		void readClient(QTcpSocket *);
		inline void respondToClient(QTcpSocket *);
};

static const auto REQUEST_REG_EXP = QRegExp("[ \r\n][ \r\n]*");
static const auto GET_METHOD = "GET";

#endif
