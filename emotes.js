WorkerScript.onMessage = function(message) {
    var reply = new Object
    var emotes = message['emotes']
    var str = message['message']

    for (var key in emotes) {
        var re = new RegExp('\\b' + key + '\\b', 'g');
        var count = str.match(re)
        if (count)
            reply[emotes[key]] = count.length
    }
    WorkerScript.sendMessage(reply)
}
