import wsConnect from "../../utils/WebSocketConnector.js"
import GameView from "../game/GameView.js"

const ipV4Pattern = /\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}/
const digitsPattern = /\d{1,5}/
const maxPortValue = 65535

export default class ConnectionController {

    constructor(connectionView) {
        this.connectionView = connectionView
    }

    async openWebSocketConnection() {
        const host = this.connectionView.getHostInputValue()
        const port = this.connectionView.getPortInputValue()
    
        const { isValid, message } = this.validateConnectionInput(host, port)
        
        if (!isValid) {
            this.connectionView.showConnectionErrorMessage(message, 2000)
            return
        }
    
        this.connectionView.disableConnectionButton()
        this.connectionView.showSpinner()
    
        let webSocket
        try {
            webSocket = await wsConnect(host, port)
        } catch (err) {
            this.connectionView.showConnectionErrorMessage(err.message, 5000)
            return
        } finally {
            this.connectionView.hideSpinner()
            this.connectionView.enableConnectionButton()
        }
    
        this.connectionView.remove()
        new GameView(webSocket).render()
    }

    validateConnectionInput(host, port) {
        if (!host || !port) {
            return {
                isValid: false,
                message: 'Host or port is not specified'
            }
        }
        if (!ipV4Pattern.test(host) && host !== 'localhost') {
            return {
                isValid: false,
                message: 'Invalid host'
            }
        }
        if (!digitsPattern.test(port) || port < 0 || port > maxPortValue) {
            return {
                isValid: false,
                message: 'Invalid port'
            }
        }
    
        return { isValid: true }
    }
}
