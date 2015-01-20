import QtQuick 2.2

Image {
    id: img
    x: Math.random() * parent.width
    y: Math.random() * parent.height
    property var dx: 3 * (Math.random() > 0.5 ? -1 : 1)
    property var dy: 3 * (Math.random() > 0.5 ? -1 : 1)

    Timer {
        running: true
        repeat: true
        interval: 20
        onTriggered: {
            var newX = img.x + img.dx
            var newY = img.y + img.dy

            if (newX < 0)
                img.dx = -img.dx
            if (newY < 0)
                img.dy = -img.dy
            if (newX > img.parent.width - img.width)
                img.dx = -img.dx
            if (newY > img.parent.height - img.height)
                img.dy = -img.dy
            img.x = Math.max(0, Math.min(newX, img.parent.width - img.width))
            img.y = Math.max(0, Math.min(newY, img.parent.height - img.height))
            img.rotation += 10
        }
    }
}
