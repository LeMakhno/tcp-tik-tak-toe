import spinner from "../../utils/Spinner.js"
import GameController from "./GameController.js"
import {
    createBoardWithCells,
    createControls,
    createErrFeedMessage,
    createFeed,
    createNicknameRegistrationPanel,
    createOkFeedMessage
} from "./GameViewBuilder.js"

const circle = "url('img/circle.svg')"
const cross = "url('img/cross.svg')"

export default class GameView {

    constructor(webSocket) {
        this.controller = new GameController(this, webSocket)
    }

    render() {
        this.nicknamePanel = createNicknameRegistrationPanel()
        this.nicknamePanel.sendButton.onclick = this.onRegisterNicknameClick.bind(this)
        document.body.prepend(this.nicknamePanel.root)
    }

    setUpControlsPanel() {
        this.controlsPanel = createControls()
    
        this.controlsPanel.joinButton.onclick = this.onJoinClick.bind(this)
        this.controlsPanel.createGameButton.onclick = this.onCreateGameClick.bind(this)
        this.controlsPanel.statusButton.onclick = this.onStatusClick.bind(this)
        this.controlsPanel.clearBoardButton.onclick = this.onClearBoardClick.bind(this)
        this.controlsPanel.leaveButton.onclick = this.onLeaveClick.bind(this)
        this.controlsPanel.disconnectButton.onclick = this.onDisconnectClick.bind(this)

        this.gameBoard = createBoardWithCells()
    
        document.body.appendChild(this.controlsPanel.root)
        document.body.prepend(this.gameBoard.board)
    }

    onJoinClick() {
        this.controller.join(this.controlsPanel.joinInput.value)
        this.controlsPanel.joinInput.value = null
    }
    
    onCreateGameClick() {
        this.controller.createGame()
    }
    
    onStatusClick() {
        this.controller.getStatus()
    }
    
    onClearBoardClick() {
        this.clearBoard()
    }
    
    onLeaveClick() {
        this.controller.leaveGame()
        this.clearBoard()
    }
    
    onDisconnectClick() {
        this.controller.disconnect()
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

    activateCells() {
        this.gameBoard.cells.forEach((cell, idx) => {
            cell.style.cursor = 'pointer'
            cell.onclick = () => {
                this.controller.actOnCell(idx + 1)
            }
        })
    }
    
    deactivateCells() {
        this.gameBoard.cells.forEach((cell) => {
            cell.style.cursor = 'not-allowed'
            cell.onclick = null
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
    
    updateBoard(boardSymbols) {    
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

    remove() {
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
    }
    
    onRegisterNicknameClick() {
        this.controller.registerNickname(this.nicknamePanel.nicknameInput.value)
    }

    showSpinner() {
        spinner.showSpinner()
    }

    hideSpinner() {
        spinner.hideSpinner()
    }

    disableRegisterNicknameButton() {
        this.nicknamePanel.sendButton.disabled = true
    }

    enableRegisterNicknameButton() {
        this.nicknamePanel.sendButton.disabled = false
    }

    setJoinGameInputErrorColor() {
        this.controlsPanel.joinInput.style.backgroundColor = 'red'
        setTimeout(() => {
            this.controlsPanel.joinInput.style.backgroundColor = 'white'
        }, 1000)
    }
}
