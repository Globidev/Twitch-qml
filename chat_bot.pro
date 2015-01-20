TEMPLATE		=	app
TARGET			=	chat_bot
INCLUDEPATH		+=	.

QMAKE_CXXFLAGS	+=	-Wall -Wextra -Werror
CONFIG			+=	c++11
CONFIG			-=	app_bundle

QT				+=	widgets qml quick network

HEADERS			+=	ChatBot.h         \
					ChatRoom.h        \
					IRCSocket.h       \
					IRCParser.h       \
					LocalHTTPServer.h \

SOURCES			+=	main.cpp            \
					ChatBot.cpp         \
					ChatRoom.cpp        \
					IRCSocket.cpp       \
					IRCParser.cpp       \
					LocalHTTPServer.cpp \
