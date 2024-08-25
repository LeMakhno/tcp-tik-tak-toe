import ConnectionView from "../connection/ConnectionView.js"

const nicknameMinLen = 3
const nicknameMaxLen = 30
const possibleBoardSymbols = 'xo '

export default class GameController {

    constructor(gameView, webSocket) {
        this.gameView = gameView
        this.webSocket = webSocket
        this.webSocket.onclose = this.onWebSocketClosed.bind(this)
    }

    registerNickname(nickname) {
        const { isValid, message } = this.validateNicknameInput(nickname)

        if (!isValid) {
            this.gameView.showNicknameErrorMessage(message, 5000)
            return
        }

        this.gameView.disableRegisterNicknameButton()
        this.gameView.showSpinner()

        this.webSocket.send(JSON.stringify({
            command: 'reg',
            args: [nickname]
        }))

        this.webSocket.onmessage = this.onNicknameRegistrationResponse.bind(this)
    }

    onNicknameRegistrationResponse(message) {
        const payload = JSON.parse(message.data)

        try {
            if (payload.status !== 'ok') {
                this.gameView.showNicknameErrorMessage(payload.data, 5000)
                return
            }
        } finally {
            this.gameView.hideSpinner()
            this.gameView.enableRegisterNicknameButton()
        }

        this.onSuccessfulNicknameRegistration(payload)
    }

    onSuccessfulNicknameRegistration(payload) {
        this.gameView.removeNicknamePanel()
        this.gameView.setUpControlsPanel()
        this.gameView.activateCells()
        this.gameView.addFeed()
        this.gameView.addOkFeedMessage(payload.data)
        this.webSocket.onmessage = this.onIncomingMessage.bind(this)
    }

    onIncomingMessage(message) {
        const payload = JSON.parse(message.data)

        if (payload.type === 'board') {
            // this.gameView.updateBoard(payload.data)
            this.gameView.updateBoard(this.parseBoard(payload.data))
            return
        }

        if (payload.status === 'ok') {
            this.gameView.addOkFeedMessage(payload.data)
        } else {
            this.gameView.addErrFeedMessage(payload.data)
        }
    }

    parseBoard(data) {
        let boardSymbols = []

        for (let char of data) {
            if (possibleBoardSymbols.includes(char)) {
                boardSymbols.push(char)
            }
        }

        return boardSymbols
    }

    join(gameId) {
        gameId = gameId.trim()

        if (!gameId || !gameId.length) {
            this.gameView.setJoinGameInputErrorColor()
            return
        }

        this.webSocket.send(JSON.stringify({
            command: 'join',
            args: [gameId]
        }))
    }

    createGame() {
        this.webSocket.send(JSON.stringify({
            command: 'init',
            args: []
        }))
    }

    getStatus() {
        this.webSocket.send(JSON.stringify({
            command: 'status',
            args: []
        }))
    }

    leaveGame() {
        this.webSocket.send(JSON.stringify({
            command: 'leave',
            args: []
        }))
    }

    actOnCell(idx) {
        this.webSocket.send(JSON.stringify({
            command: 'put',
            args: [idx]
        }))
    }

    disconnect() {
        this.webSocket.close(1000)
    }

    onWebSocketClosed() {
       this.gameView.remove()
        const connectionView = new ConnectionView();
        connectionView.render()
        connectionView.showConnectionErrorMessage('Disconnected', 5000)
    }

    validateNicknameInput(nickname) {
        if (!nickname) {
            return {
                isValid: false,
                message: 'Nickname is not specified'
            }
        }
        nickname = nickname.trim()
        if (!nickname) {
            return {
                isValid: false,
                message: 'Nickname is empty'
            }
        }
        if (nickname.length < nicknameMinLen || nickname.length > nicknameMaxLen) {
            return {
                isValid: false,
                message: `Nickname length must be between ${nicknameMinLen} and ${nicknameMaxLen}`
            }
        }
        return { isValid: true }
    }
}
