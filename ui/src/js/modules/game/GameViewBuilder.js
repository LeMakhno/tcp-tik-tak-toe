const cellsAmount = 9

export function createBoardWithCells() {
    const board = document.createElement('div')
    board.id = 'board'

    const cells = []
    for (let i = 0; i < cellsAmount; i++) {
        const cell = document.createElement('div')
        cell.classList.add('cell')
        cells.push(cell)
        board.appendChild(cell)
    }

    return {
        board,
        cells
    }
}

export function createBoard() {
    const board = document.createElement('div')
    board.id = 'board'
    return board
}

export function createCells() {
    const cells = []
    for (let i = 0; i < 9; i++) {
        const cell = document.createElement('div')
        cell.classList.add('cell')
        cells.push(cell)
    }
    return cells
}

export function createFeed() {
    const feed = document.createElement('div')
    feed.id = 'feed'
    return feed
}

export function createOkFeedMessage(text) {
    return createFeedMessage('message-ok', text)
}

export function createErrFeedMessage(text) {
    return createFeedMessage('message-err', text)
}

function createFeedMessage(messageClass, text) {
    const message = document.createElement('div')
    message.classList.add('message')
    message.classList.add(messageClass)
    message.textContent = text
    return message
}

export function createNicknameRegistrationPanel() {
    const root = document.createElement('div')
    root.id = 'nickname-panel'
    root.classList.add('shadowed')
    
    const title = document.createElement('h5')
    title.id = 'nickname-title'
    title.textContent = 'Enter a nickname to use during this session'

    const nicknameInput = document.createElement('input')
    nicknameInput.id = 'nickname-input'
    nicknameInput.type = 'text'
    nicknameInput.classList.add('shadowed')
    nicknameInput.classList.add('input-field')
    nicknameInput.placeholder = 'Nickname'

    const errorMessage = document.createElement('div')
    errorMessage.id = 'nickname-error-message'
    errorMessage.classList.add('error-message')

    const sendButton = document.createElement('button')
    sendButton.id = 'nickname-btn'
    sendButton.textContent = 'Register'
    sendButton.classList.add('shadowed')
    sendButton.classList.add('btn')

    root.appendChild(title)
    root.appendChild(nicknameInput)
    root.appendChild(errorMessage)
    root.appendChild(sendButton)

    return {
        root,
        nicknameInput,
        errorMessage,
        sendButton
    }
}

export function createControls() {
    const root = document.createElement('div')
    root.id = 'controls'

    const joinBlock = document.createElement('div')
    joinBlock.id = 'join'

    const joinButton = document.createElement('button')
    joinButton.id = 'join-btn'
    joinButton.classList.add('shadowed')
    joinButton.classList.add('btn')
    joinButton.innerText = 'Join 🤝'

    const joinInput = document.createElement('input')
    joinInput.id = 'join-input'
    joinInput.placeholder = 'Game ID'
    joinInput.type = 'text'
    joinInput.classList.add('input-field')
    joinInput.classList.add('shadowed')

    joinBlock.appendChild(joinButton)
    joinBlock.appendChild(joinInput)

    const createGameButton = document.createElement('button')
    createGameButton.classList.add('shadowed')
    createGameButton.classList.add('btn')
    createGameButton.innerText = 'Create game 🎮🎲'

    const statusButton = document.createElement('button')
    statusButton.classList.add('shadowed')
    statusButton.classList.add('btn')
    statusButton.innerText = 'Status 🤔'

    const clearBoardButton = document.createElement('button')
    clearBoardButton.classList.add('shadowed')
    clearBoardButton.classList.add('btn')
    clearBoardButton.innerText = 'Clear board 🗑'

    const leaveButton = document.createElement('button')
    leaveButton.classList.add('shadowed')
    leaveButton.classList.add('btn')
    leaveButton.innerText = 'Leave ❌'

    const disconnectButton = document.createElement('button')
    disconnectButton.classList.add('shadowed')
    disconnectButton.classList.add('btn')
    disconnectButton.innerText = 'Disconnect 🔴'

    root.appendChild(joinBlock)
    root.appendChild(createGameButton)
    root.appendChild(statusButton)
    root.appendChild(clearBoardButton)
    root.appendChild(leaveButton)
    root.appendChild(disconnectButton)
    
    return {
        root,
        joinButton,
        joinInput,
        createGameButton,
        statusButton,
        clearBoardButton,
        leaveButton,
        disconnectButton
    }
}
