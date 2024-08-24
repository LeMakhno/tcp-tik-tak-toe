const ipV4Pattern = /\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}/
const digitsPattern = /\d{1,5}/
const maxPortValue = 65535

const nicknameMinLen = 3
const nicknameMaxLen = 30

function validateConnectionInput(host, port) {
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

function validateNicknameInput(nickname) {
    if (!nickname || !nickname.length) {
        return {
            isValid: false,
            message: 'Nickname is not specified'
        }
    }
    nickname = nickname.trim()
    if (!nickname.length) {
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

export { validateConnectionInput, validateNicknameInput }
