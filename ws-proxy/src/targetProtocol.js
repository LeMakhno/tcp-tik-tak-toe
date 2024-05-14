class ProtocolConstants {
    static ELEMENT_SEPARATOR = '@'
    static END_OF_MESSAGE = ';'
}

function hasMessage(buffer) {
    return buffer.indexOf(ProtocolConstants.END_OF_MESSAGE) !== -1
}

function consumeMessage(buffer) {
    const endOfMessage = buffer.indexOf(ProtocolConstants.END_OF_MESSAGE)
    const message = buffer.subarray(0, endOfMessage).toString('ascii')

    const remainingDataLen = buffer.byteLength - endOfMessage - 1

    const remainder = remainingDataLen
        ? buffer.subarray(endOfMessage + 1)
        : Buffer.alloc(0)

    return { message, remainder }
}

function targetMessageToJson(targetMessage) {
    const elemets = targetMessage.split(ProtocolConstants.ELEMENT_SEPARATOR)

    return JSON.stringify({
        type: elemets[0],
        status: elemets[1],
        data: elemets[2]
    })
}

function jsonToTargetMessage(jsonMessage) {
    if (!jsonMessage.command || !jsonMessage.args) {
        throw new Error('Bad request: missing command or args array')
    }

    if (!jsonMessage.args.length) {
        return jsonMessage.command + ProtocolConstants.END_OF_MESSAGE
    }

    let targetMessage = jsonMessage.command + ProtocolConstants.ELEMENT_SEPARATOR
        + jsonMessage.args.join(ProtocolConstants.ELEMENT_SEPARATOR)
        + ProtocolConstants.END_OF_MESSAGE

    return targetMessage
}

export { consumeMessage, hasMessage, jsonToTargetMessage, targetMessageToJson }
