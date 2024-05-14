import { WebSocketServer } from 'ws'
import { onConnection, onServerListening } from './wsHandler.js'
import config from './config.js'

const server = new WebSocketServer({
    port: config.port
})

server.on('connection', onConnection)
server.on('listening', onServerListening)
