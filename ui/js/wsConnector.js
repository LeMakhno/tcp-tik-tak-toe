const connectionTimeoutMs = 5000

function wsConnect(host, port) {
    let connectionTimedOut = false

    return new Promise((resolve, reject) => {
        const webSocket = new WebSocket(`ws://${host}:${port}`)

        const connectionTimeoutTimer = setTimeout(() => {
            connectionTimedOut = true
            webSocket.close()
        }, connectionTimeoutMs)

        webSocket.onclose = closeEvent => {
            clearTimeout(connectionTimeoutTimer)
            if (connectionTimedOut) {
                reject(new Error('Connection timeout'))
                return
            }
            if (closeEvent.code === 1011) {
                reject(new Error('Service not available'))
                return
            }
            const reason = closeEvent.reason?.length
                ? closeEvent.reason
                : 'Connection refused'
            reject(new Error(reason))
        }

        webSocket.onmessage = message => {
            clearTimeout(connectionTimeoutTimer)
            const payload = JSON.parse(message.data)
            if (payload.status !== 'connected') {
                webSocket.close(1000, 'Unexpected error')
                return
            }
            resolve(webSocket)
        }
    })
}

export { wsConnect }
