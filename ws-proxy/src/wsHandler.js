import { isAscii } from 'buffer'
import { randomUUID } from 'crypto'
import { connectToTarget } from './target.js'

class EventType {
    static MESSAGE = 'message'
    static BOARD = 'board'
}

class EventStatus {
    static OK = 'ok'
    static CONNECTED = 'connected'
    static ERROR = 'err'
}

async function onConnection(webSocket, request) {
    console.log(`Connected: ${request.socket.remoteAddress}`)

    const tunnelId = randomUUID()

    try {
        const targetConnection = await connectToTarget(tunnelId, webSocket)
        webSocket.tunnelId = tunnelId
        webSocket.targetConnection = targetConnection
        webSocket.isActive = true

        console.log(`Created tunnel ${tunnelId}`)

        webSocket.on('message', onMessage.bind(webSocket))
        webSocket.on('close', onClose.bind(webSocket))
        webSocket.on('error', onError.bind(webSocket))
        webSocket.on('closedByTarget', onCloseByTarget.bind(webSocket))
        webSocket.on('write', onWrite.bind(webSocket))

        webSocket.send(JSON.stringify({
            type: EventType.MESSAGE,
            status: EventStatus.CONNECTED,
            data: tunnelId
        }))
    } catch (err) {
        console.log(`${err} ${tunnelId}`)
        webSocket.close(1011, JSON.stringify({
            type: EventType.MESSAGE,
            status: EventStatus.ERROR,
            data: err.message ?? 'error'
        }))
    }
}

function onMessage(data, isBinary) {
    if (isBinary || !isAscii(data)) {
        this.send(JSON.stringify({
            type: EventType.MESSAGE,
            status: EventStatus.ERROR,
            data: 'Only ASCII characters allowed'
        }))
        return
    }

    const asciiData = data.toString('ascii')

    console.log(`Message: ${asciiData} ${this.tunnelId}`)
    this.targetConnection.emit('write', asciiData)
}

function onClose(code, reason) {
    console.log(`Client connection closed: ${code} ${clientDisconnectionReason(reason)} ${this.tunnelId}`)
    this.isActive = false
    if (this.targetConnection.isActive) {
        this.targetConnection.emit('closedByClient')
    }
}

function onError(err) {
    console.log(`Client connection error: ${err} ${this.tunnelId}`)
}

function onCloseByTarget() {
    this.close(1011, JSON.stringify({
        type: EventType.MESSAGE,
        data: 'Closed by target'
    }))
}

function onWrite(data) {
    console.log(`Writing to WebSocket: ${this.tunnelId}`)
    this.send(data)
}

function onServerListening() {
    console.log('Server started')
}

function clientDisconnectionReason(reason) {
    return reason.length
        ? reason.toString('utf-8')
        : 'Silent'
}

export {
    EventType,
    onConnection,
    onServerListening
}
