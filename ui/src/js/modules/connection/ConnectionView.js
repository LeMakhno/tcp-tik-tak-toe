import spinner from '../../utils/Spinner.js'
import ConnectionController from './ConnectionController.js'
import {
    createConnectButton,
    createConnectionInputBlock,
    createErrorMessage,
    createRoot,
    createTitle
} from './ConnectionViewBuilder.js'

export default class ConnectionView {

    constructor() {
        this.controller = new ConnectionController(this)
    }

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

    showSpinner() {
        spinner.showSpinner()
    }

    hideSpinner() {
        spinner.hideSpinner()
    }

    getHostInputValue() {
        return this.connectionInputBlock.hostInput.value
    }

    getPortInputValue() {
        return this.connectionInputBlock.portInput.value
    }

    onConnectClick() {
        this.controller.openWebSocketConnection()
    }
}
