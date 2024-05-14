import * as connectionViewBuilder from './connectionViewBuilder.js'
import { hideSpinner, showSpinner } from '../../spinner.js'
import { validateConnectionInput } from '../../validations.js'
import { wsConnect } from '../../wsConnector.js'
import { setUpGameView } from '../game/gameView.js'

let root
let connectionHostInput
let connectionPortInput
let connectErrorMessage
let connectButton

function setUpConnectionView() {
    root = connectionViewBuilder.createRoot()

    const {
        inputBlock,
        hostInput,
        portInput
    } = connectionViewBuilder.createConnectionAddressBlock()

    connectionHostInput = hostInput
    connectionPortInput = portInput

    connectErrorMessage = connectionViewBuilder.createErrorMessage()

    connectButton = connectionViewBuilder.createConnectButton()
    connectButton.onclick = onConnectClick

    root.appendChild(connectionViewBuilder.createTitle())
    root.appendChild(inputBlock)
    root.appendChild(connectErrorMessage)
    root.appendChild(connectButton)

    document.body.prepend(root)
}

function removeConnectionView() {
    root.remove()
}

function disableConnectionButton() {
    connectButton.disabled = true
}

function enableConnectionButton() {
    connectButton.disabled = false
}

async function onConnectClick() {
    const host = connectionHostInput.value
    const port = connectionPortInput.value

    console.log(`Connecting to ${host}:${port}`)

    const { isValid, message } = validateConnectionInput(host, port)
    
    if (!isValid) {
        showConnectionErrorMessage(message, 2000)
        return
    }

    disableConnectionButton()
    showSpinner()

    let webSocket
    try {
        webSocket = await wsConnect(host, port)
    } catch (err) {
        showConnectionErrorMessage(err.message ?? 'Unexpected error', 5000)
        return
    } finally {
        hideSpinner()
        enableConnectionButton()
    }

    removeConnectionView()
    setUpGameView(webSocket)
}

function showConnectionErrorMessage(message, visibilityMs) {
    connectErrorMessage.textContent = message
    connectErrorMessage.style.opacity = 1
    if (visibilityMs) {
        setTimeout(() => {
            connectErrorMessage.style.opacity = 0
        }, visibilityMs)
    }
}

export {
    enableConnectionButton,
    setUpConnectionView,
    showConnectionErrorMessage
}
