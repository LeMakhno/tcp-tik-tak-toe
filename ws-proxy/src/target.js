import { connect } from 'net'
import config from './config.js'
import {
    consumeMessage,
    hasMessage,
    jsonToTargetMessage,
    targetMessageToJson
} from './targetProtocol.js'

const connectionTimeoutMs = 5000

const targetHostAndPort = `${config.targetHost}:${config.targetPort}`

function connectToTarget(tunnelId, webSocket) {
    return new Promise((resolve, reject) => {
        const sock = connect({
            host: config.targetHost,
            port: config.targetPort,
            family: 4
        })

        sock.tunnelId = tunnelId
        sock.webSocket = webSocket
        sock.isActive = true

        sock.tempBuffer = Buffer.alloc(0)

        const connectionTimeout = setTimeout(() => {
            sock.destroy()
            console.log(`Timed out connecting to target: ${targetHostAndPort}`)
            reject(new Error('Timed out connecting to target'))
        }, connectionTimeoutMs)

        sock.once('error', err => {
            clearTimeout(connectionTimeout)
            console.log(`Error connecting to target: ${targetHostAndPort}`)
            reject(err)
        })

        sock.on('connect', () => {
            clearTimeout(connectionTimeout)
            sock.removeAllListeners('error')
            sock.on('error', onError.bind(sock))
            sock.on('data', onRead.bind(sock))
            sock.on('close', onClose.bind(sock))
            sock.on('write', onWrite.bind(sock))
            sock.on('closedByClient', onCloseByClient.bind(sock))
            resolve(sock)
        })
    })
}

function onRead(data) {
    let buffer = Buffer.concat([this.tempBuffer, data])

    while (hasMessage(buffer)) {
        const { message, remainder } = consumeMessage(buffer)
        const messageJson = targetMessageToJson(message)

        console.log(`Received from target: ${messageJson} ${this.tunnelId}`)

        this.webSocket.emit('write', messageJson)
        buffer = remainder
    }

    this.tempBuffer = buffer
}

function onError(err) {
    console.log(`Socket error: ${err} ${this.tunnelId}`)
}

function onClose(hadError) {
    console.log(`Closing ${this.tunnelId}`)
    this.isActive = false
    if (this.webSocket.isActive) {
        this.webSocket.emit('closedByTarget')
    }
}

function onCloseByClient() {
    console.log(`Closing target connection due to client disconnection ${this.tunnelId}`)
    this.destroy()
}

function onWrite(data) {
    let targetMessage
    try {
        targetMessage = jsonToTargetMessage(JSON.parse(data))
    } catch (err) {
        console.log(`${err} ${this.tunnelId}`)
        return
    }
    console.log(`Proxying to target: ${targetMessage} ${this.tunnelId}`)
    this.write(targetMessage)
}

export { connectToTarget }
