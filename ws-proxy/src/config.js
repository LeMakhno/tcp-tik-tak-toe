const config = {
    port: Number.parseInt(process.env.PORT || '2828'),
    targetHost: process.env.TARGET_HOST || 'localhost',
    targetPort: Number.parseInt(process.env.TARGET_PORT || '6677')
}

console.log(`Config: ${JSON.stringify(config)}`)

export default config
