import { wsConnect } from '../../utils/wsConnector.js'
import { validateConnectionInput } from '../../utils/validations.js'
import GameView from '../game/gameView.js'
import { hideSpinner, showSpinner } from '../spinner.js'
import {
    createConnectButton,
    createConnectionInputBlock,
    createErrorMessage,
    createRoot,
    createTitle
} from './connectionViewBuilder.js'

export default class ConnectionView {

    render() {
        this.root = createRoot()
        this.title = createTitle()
        this.connectionInputBlock = createConnectionInputBlock()
        this.connectionErrorMessage = createErrorMessage()
        this.connectionButton = createConnectButton()

        this.root.appendChild(this.title)
        this.root.appendChild(this.connectionInputBlock.root)
        this.root.appendChild(this.connectionErrorMessage)
        this.root.appendChild(this.connectionButton)

        this.connectionButton.onclick = this.onConnectClick.bind(this)
    
        document.body.prepend(this.root)
    }

    remove() {
        this.root.remove()
    }

    showConnectionErrorMessage(message, visibilityMs) {
        this.connectionErrorMessage.textContent = message
        this.connectionErrorMessage.style.opacity = 1
        if (visibilityMs) {
            setTimeout(() => {
                this.connectionErrorMessage.style.opacity = 0
            }, visibilityMs)
        }
    }

    disableConnectionButton() {
        this.connectionButton.disabled = true
    }

    enableConnectionButton() {
        this.connectionButton.disabled = false
    }

    async onConnectClick() {
        const host = this.connectionInputBlock.hostInput.value
        const port = this.connectionInputBlock.portInput.value
    
        const { isValid, message } = validateConnectionInput(host, port)
        
        if (!isValid) {
            this.showConnectionErrorMessage(message, 2000)
            return
        }
    
        this.disableConnectionButton()
        showSpinner()
    
        let webSocket
        try {
            webSocket = await wsConnect(host, port)
        } catch (err) {
            this.showConnectionErrorMessage(err.message, 5000)
            return
        } finally {
            hideSpinner()
            this.enableConnectionButton()
        }
    
        this.remove()
        new GameView(webSocket).render()
    }
}
