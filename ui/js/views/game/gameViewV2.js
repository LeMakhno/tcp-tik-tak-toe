import { validateNicknameInput } from "../../validations.js"
import {
    createBoardWithCells,
    createControls,
    createErrFeedMessage,
    createFeed,
    createNicknamePanel,
    createOkFeedMessage
} from "./gameViewBuilder.js"

export default class GameView {

    constructor(webSocket) {
        this.webSocket = webSocket
    }

    render() {
        this.gameBoard = createBoardWithCells()
        this.nicknamePanel = createNicknamePanel()

        this.nicknamePanel.sendButton.onclick = this.onRegisterNicknameClick.bind(this)
        this.webSocket.onmessage = this.onNicknameRegistrationResponse.bind(this)

        document.body.prepend(this.gameBoard.board)
        document.body.prepend(this.nicknamePanel.root)

        this.webSocket.onclose = this.onWebSocketClosed.bind(this)
    }

    setUpControlsPanel() {
        this.controlsPanel = createControls()
    
        this.controlsPanel.joinButton.onclick = this.onJoinClick.bind(this)
        this.controlsPanel.createGameButton.onclick = this.onCreateGameClick.bind(this)
        this.controlsPanel.statusButton.onclick = this.onStatusClick.bind(this)
        this.controlsPanel.clearBoardButton.onclick = this.onClearBoardClick.bind(this)
        this.controlsPanel.leaveButton.onclick = this.onLeaveClick.bind(this)
        this.controlsPanel.disconnectButton.onclick = this.onDisconnectClick.bind(this)
    
        document.body.appendChild(this.controlsPanel.root)
    }
    
    onJoinClick() {
        const { joinInput } = this.controlsPanel
    
        const gameId = joinInput.value?.trim()
    
        if (!gameId || !gameId.length) {
            joinInput.style.backgroundColor = 'red'
            setTimeout(() => {
                joinInput.style.backgroundColor = 'white'
            }, 1000)
            return
        }
    
        joinInput.value = null
    
        this.webSocket.send(JSON.stringify({
            command: 'join',
            args: [gameId]
        }))
    }
    
    onCreateGameClick() {
        this.webSocket.send(JSON.stringify({
            command: 'init',
            args: []
        }))
    }
    
    onStatusClick() {
        this.webSocket.send(JSON.stringify({
            command: 'status',
            args: []
        }))
    }
    
    onClearBoardClick() {
        this.clearBoard()
    }
    
    onLeaveClick() {
        this.webSocket.send(JSON.stringify({
            command: 'leave',
            args: []
        }))
    }
    
    onDisconnectClick() {
        this.webSocket.close(1000)
    }
    
    createCellClickHandler(idx) {
        return () => {
            this.webSocket.send(JSON.stringify({
                command: 'put',
                args: [idx]
            }))
        }
    }
    
    addFeed() {
        this.feed = createFeed()
        this.gameBoard.board.after(this.feed)
    }
    
    addOkFeedMessage(data) {
        this.feed.prepend(createOkFeedMessage(data))
    }
    
    addErrFeedMessage(data) {
        this.feed.prepend(createErrFeedMessage(data))
    }
    
    activeteCells() {
        this.gameBoard.cells.forEach((cell, idx) => {
            cell.style.cursor = 'pointer'
            cell.onclick = this.createCellClickHandler(idx + 1)
        })
    }
    
    deactivateCells() {
        this.gameBoard.cells.forEach((cell, idx) => {
            cell.style.cursor = 'not-allowed'
            cell.onclick = undefined
        })
    }
    
    clearBoard() {
        this.gameBoard.cells.forEach(cell => {
            cell.style.backgroundImage = 'none'
        })
    }
    
    removeNicknamePanel() {
        this.nicknamePanel.root.remove()
        this.nicknamePanel = null
    }
    
    removeControlsPanel() {
        this.controlsPanel.root.remove()
        this.controlsPanel = null
    }
    
    removeFeed() {
        this.feed.remove()
        this.feed = null
    }
    
    removeBoard() {
        this.gameBoard.board.remove()
        this.gameBoard = null
    }
    
    showNicknameErrorMessage(message, visibilityMs) {
        this.nicknamePanel.errorMessage.textContent = message
        this.nicknamePanel.errorMessage.style.opacity = 1
        if (visibilityMs) {
            setTimeout(() => {
                this.nicknamePanel.errorMessage.style.opacity = 0
            }, visibilityMs)
        }
    }
    
    onNicknameRegistrationResponse(message) {
        const payload = JSON.parse(message.data)
    
        if (payload.status !== 'ok') {
            this.showNicknameErrorMessage(payload.data, 5000)
            return
        }
    
        this.removeNicknamePanel()
        this.setUpControlsPanel()
        this.activeteCells()
        this.addFeed()
        this.addOkFeedMessage(payload.data)
        this.webSocket.onmessage = this.onIncomingMessage.bind(this)
    }
    
    onIncomingMessage(message) {
        const payload = JSON.parse(message.data)
    
        if (payload.type === 'board') {
            this.updateBoard(payload.data)
            return
        }
    
        if (payload.status === 'ok') {
            this.addOkFeedMessage(payload.data)
        } else {
            this.addErrFeedMessage(payload.data)
        }
    }
    
    updateBoard(payload) {
        const possibleBoardSymbols = 'xo '
        let boardSymbols = []
    
        for (let i = 0; i < payload.length; i++) {
            if (possibleBoardSymbols.includes(payload.charAt(i))) {
                boardSymbols.push(payload.charAt(i))
            }
        }
    
        this.gameBoard.cells.forEach((cell, idx) => {
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
    
    onWebSocketClosed() {
        this.deactivateCells()
        if (this.controlsPanel) {
            this.removeControlsPanel()
        }
        if (this.nicknamePanel) {
            this.removeNicknamePanel()
        }
        if (this.feed) {
            this.removeFeed()
        }
        if (this.gameBoard) {
            this.removeBoard()
        }
        this.setUpConnectionView()
        this.showConnectionErrorMessage('Disconnected', 5000)
    }
    
    onRegisterNicknameClick() {
        const nickname = this.nicknamePanel.nicknameInput.value
    
        const { isValid, message } = validateNicknameInput(nickname)
    
        if (!isValid) {
            this.showNicknameErrorMessage(message, 5000)
            return
        }
    
        this.webSocket.send(JSON.stringify({
            command: 'reg',
            args: [nickname]
        }))
    }
}
