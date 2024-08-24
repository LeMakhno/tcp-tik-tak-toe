export function createRoot() {
    const root = document.createElement('div')
    root.id = 'connect'
    return root
}

export function createTitle() {
    const title = document.createElement('h5')
    title.id = 'connect-title'
    title.textContent = 'Connect to a server ☝'
    return title
}

export function createConnectionInputBlock() {
    const root = document.createElement('div')
    root.id = 'connect-address-block'

    const hostInput = document.createElement('input')
    hostInput.type = 'text'
    hostInput.classList.add('input-field')
    hostInput.classList.add('shadowed')
    hostInput.id = 'connect-host'
    hostInput.placeholder = 'Host'

    const colon = document.createElement('span')
    colon.classList.add('connect-address-colon')
    colon.textContent = ':'

    const portInput = document.createElement('input')
    portInput.type = 'text'
    portInput.classList.add('input-field')
    portInput.classList.add('shadowed')
    portInput.id = 'connect-port'
    portInput.placeholder = 'Port'

    root.appendChild(hostInput)
    root.appendChild(colon)
    root.appendChild(portInput)

    return { root, hostInput, portInput }
}

export function createErrorMessage() {
    const errorMessage = document.createElement('div')
    errorMessage.id = 'connect-error-message'
    errorMessage.classList.add('error-message')
    return errorMessage
}

export function createConnectButton() {
    const connectButton = document.createElement('button')
    connectButton.id = 'server-connect-btn'
    connectButton.textContent = 'Connect'
    connectButton.classList.add('shadowed')
    connectButton.classList.add('btn')
    return connectButton
}
