import QtQuick 2.2
import QtQuick.Controls 1.1
import GlobiTwitch 1.0

Item {
    id: container
    width: 1920
    height: 1080
    property var emotes: new Object;
    property int kpmCount: 0;

    Rectangle {
        id: stateRect
        anchors.fill: parent
        color: 'firebrick'
    }

    WorkerScript {
        id: emoteFinder
        source: 'emotes.js'

        onMessage: {
            for (var key in messageObject) {
                for (var i = 0; i < messageObject[key]; ++i) {
                    var component = Qt.createComponent('emote.qml');
                    if (component.status == Component.Ready) {
                        var emote = component.createObject(container, {
                            'source' : Qt.resolvedUrl(key)
                        });
                    }
                }
            }
        }
    }

    ChatBot {
        id: bot

        ChatRoom {
            id: chatRoom

            onMessage: {
                if (sender && sender != 'jtv') {
                    chatModel.append({
                        'who'  : sender,
                        'what' : message
                    })
                    chatView.positionViewAtEnd()
                    var kappas = countWord(message, 'Kappa')
                    kappaCount.count += kappas
                    emoteFinder.sendMessage({
                        'message' : message,
                        'emotes'  : container.emotes
                    })
                }
                if (message == '!kpm') {
                    ++kpmCount;
                    print('kpm called by ' + sender + ' (' + kpmCount + ')')
                    var minutes = ((new Date - kappaCount.start) / 60000)
                    var kpm = kappaCount.count / minutes
                    sendMessage(sender + ' --> ' + 'current KPM : ' + kpm +
                                ' (' + kappaCount.count + ' Kappa s in ' +
                                minutes + ' minutes)')
                }
            }

            function countWord(str, word) {
                var re = new RegExp('\\b' + word + '\\b', 'g');
                var count = str.match(re)
                count = count? count.length : 0
                return (count)
            }
        }

        onAuthenticated : {
            stateRect.color = 'LightGreen'
            channelInput.enabled = true
            channelButton.enabled = true
        }
    }

    Text {
        id: kappaCount
        property int count: 0
        property var start: new Date
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 50
        font.pointSize: 50

        onCountChanged: text = count

        function countKappa(str) {
            var count = str.match(/\bKappa\b/g)
            count = count? count.length : 0
            return (count)
        }
    }

    Rectangle {
        id: chatRect
        width: 300
        height: parent.height
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 50
        border.width: 2
        border.color: 'red'

        ListView {
            id: chatView
            anchors.fill: parent
            anchors.margins: 10

            model: ListModel {
                id: chatModel
            }

            delegate: Text {
                width: parent.width
                text: who + ' : ' + what
                wrapMode: Text.WordWrap
            }
        }
    }

    TextField {
        id: channelInput
        enabled: false
        width: 150
        anchors.horizontalCenter: chatRect.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        placeholderText: 'choose a channel'
    }

    Button {
        id: channelButton
        enabled: false
        anchors.verticalCenter: channelInput.verticalCenter
        anchors.left: channelInput.right
        anchors.leftMargin: 10
        text: 'Go'
        onClicked: {
            chatRoom.channel = channelInput.text
            chatRoom.join()
            chatModel.clear()
        }
    }

    TextField {
        id: botNameInput
        anchors.centerIn: parent
        placeholderText: 'bot nickname'
    }

    Button {
        id: botNameButton
        anchors.verticalCenter: botNameInput.verticalCenter
        anchors.left: botNameInput.right
        anchors.leftMargin: 10
        text: 'Go'
        onClicked: {
            bot.username = botNameInput.text
            bot.authenticate()
        }
    }

    Action {
        shortcut: 'Ctrl+W'
        onTriggered: Qt.quit()
    }

    Component.onCompleted : {
        var request = new XMLHttpRequest()
        request.onreadystatechange = function() {
            if (request.readyState == XMLHttpRequest.DONE) {
                var em = eval(request.responseText)['emotes']
                for (var key in em) {
                    emotes[key] = 'http://static-cdn.jtvnw.net/emoticons/v1/' + em[key]['image_id'] + '/2.0'
                }
            }
        }
        request.open("GET", "http://twitchemotes.com/api_cache/v2/global.json")
        request.send()
    }
}
