# What is this ?
- A tik-tak-toe multiplayer game server written in c
- An adapter that translates custom server protocol to json encoded messages over WebSocket
- Web UI to play

# How to play ?
Deploy the whole application using:
```shell
docker compose -f docker-compose-all.yaml up
```
Access port 8888 on host where the Web UI is  
When prompted to connect to server, specify host and port of ws-proxy

To deploy backend only, use:
```shell
docker compose -f docker-compose-backend.yaml up
```
