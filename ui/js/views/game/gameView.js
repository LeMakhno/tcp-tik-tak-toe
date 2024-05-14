import { validateNicknameInput } from '../../validations.js'
import {
    setUpConnectionView,
    showConnectionErrorMessage
} from '../connection/connectionView.js'
import * as gameViewBuilder from './gameViewBuilder.js'

const circle = "url('../img/circle.svg')"
const cross = "url('../img/cross.svg')"

let webSocket

let gameBoard
let nicknamePanel
let controlsPanel
let feed

function setUpGameView(ws) {
    webSocket = ws

    gameBoard = gameViewBuilder.createBoardWithCells()
    
    nicknamePanel = gameViewBuilder.createNicknamePanel()
    nicknamePanel.sendButton.onclick = onRegisterNicknameClick
    webSocket.onmessage = onNicknameRegistrationMessage

    document.body.appendChild(gameBoard.board)
    document.body.appendChild(nicknamePanel.root)

    webSocket.onclose = onWebSocketClosed
}

export function setUpControlsPanel() {
    controlsPanel = gameViewBuilder.createControls()

    controlsPanel.joinButton.onclick = onJoinClick
    controlsPanel.createGameButton.onclick = onCreateGameClick
    controlsPanel.statusButton.onclick = onStatusClick
    controlsPanel.clearBoardButton.onclick = onClearBoardClick
    controlsPanel.leaveButton.onclick = onLeaveClick
    controlsPanel.disconnectButton.onclick = onDisconnectClick

    document.body.appendChild(controlsPanel.root)
}

function onJoinClick() {
    const { joinInput } = controlsPanel

    const gameId = joinInput.value?.trim()

    if (!gameId || !gameId.length) {
        joinInput.style.backgroundColor = 'red'
        setTimeout(() => {
            joinInput.style.backgroundColor = 'white'
        }, 1000)
        return
    }

    joinInput.value = null

    webSocket.send(JSON.stringify({
        command: 'join',
        args: [gameId]
    }))
}

function onCreateGameClick() {
    webSocket.send(JSON.stringify({
        command: 'init',
        args: []
    }))
}

function onStatusClick() {
    webSocket.send(JSON.stringify({
        command: 'status',
        args: []
    }))
}

function onClearBoardClick() {
    clearBoard()
}

function onLeaveClick() {
    webSocket.send(JSON.stringify({
        command: 'leave',
        args: []
    }))
}

function onDisconnectClick() {
    webSocket.close(1000)
}

function createCellClickHandler(idx) {
    return () => {
        webSocket.send(JSON.stringify({
            command: 'put',
            args: [idx]
        }))
    }
}

function addFeed() {
    feed = gameViewBuilder.createFeed()
    gameBoard.board.after(feed)
}

function addOkFeedMessage(data) {
    feed.prepend(gameViewBuilder.createOkFeedMessage(data))
}

function addErrFeedMessage(data) {
    feed.prepend(gameViewBuilder.createErrFeedMessage(data))
}

function activeteCells() {
    gameBoard.cells.forEach((cell, idx) => {
        cell.style.cursor = 'pointer'
        cell.onclick = createCellClickHandler(idx + 1)
    })
}

function deactivateCells() {
    gameBoard.cells.forEach((cell, idx) => {
        cell.style.cursor = 'not-allowed'
        cell.onclick = undefined
    })
}

function clearBoard() {
    gameBoard.cells.forEach(cell => {
        cell.style.backgroundImage = 'none'
    })
}

function removeNicknamePanel() {
    nicknamePanel.root.remove()
    nicknamePanel = undefined
}

function removeControlsPanel() {
    controlsPanel.root.remove()
    controlsPanel = undefined
}

function removeFeed() {
    feed.remove()
    feed = undefined
}

function removeBoard() {
    gameBoard.board.remove()
    gameBoard = undefined
}

function showNicknameErrorMessage(message, visibilityMs) {
    nicknamePanel.errorMessage.textContent = message
    nicknamePanel.errorMessage.style.opacity = 1
    if (visibilityMs) {
        setTimeout(() => {
            nicknamePanel.errorMessage.style.opacity = 0
        }, visibilityMs)
    }
}

function onNicknameRegistrationMessage(message) {
    const payload = JSON.parse(message.data)

    if (payload.status !== 'ok') {
        showNicknameErrorMessage(payload.data, 5000)
        return
    }

    removeNicknamePanel()
    setUpControlsPanel()
    activeteCells()
    addFeed()
    addOkFeedMessage(payload.data)
    webSocket.onmessage = onIncomingMessage
}

function onIncomingMessage(message) {
    const payload = JSON.parse(message.data)

    if (payload.type === 'board') {
        updateBoard(payload.data)
        return
    }

    if (payload.status === 'ok') {
        addOkFeedMessage(payload.data)
    } else {
        addErrFeedMessage(payload.data)
    }
}

function updateBoard(payload) {
    const possibleBoardSymbols = 'xo '
    let boardSymbols = []

    for (let i = 0; i < payload.length; i++) {
        if (possibleBoardSymbols.includes(payload.charAt(i))) {
            boardSymbols.push(payload.charAt(i))
        }
    }

    gameBoard.cells.forEach((cell, idx) => {
        const boardSymbol = boardSymbols[idx]
        if (boardSymbol === 'x') {
            cell.style.backgroundImage = cross
        } else if (boardSymbol === 'o') {
            cell.style.backgroundImage = circle
        } else {
            cell.style.backgroundImage = 'none'
        }
    })
}

function onWebSocketClosed() {
    deactivateCells()
    if (controlsPanel) {
        removeControlsPanel()
    }
    if (nicknamePanel) {
        removeNicknamePanel()
    }
    if (feed) {
        removeFeed()
    }
    if (gameBoard) {
        removeBoard()
    }
    setUpConnectionView()
    showConnectionErrorMessage('Disconnected', 5000)
}

function onRegisterNicknameClick() {
    const nickname = nicknamePanel.nicknameInput.value

    const { isValid, message } = validateNicknameInput(nickname)

    if (!isValid) {
        showNicknameErrorMessage(message, 5000)
        return
    }

    webSocket.send(JSON.stringify({
        command: 'reg',
        args: [nickname]
    }))
}

export { setUpGameView }
