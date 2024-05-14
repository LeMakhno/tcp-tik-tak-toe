FROM ubuntu:22.04

ENV PORT=6677

RUN apt update
RUN apt install make -y
RUN apt install gcc -y

WORKDIR /app
COPY . .
RUN make

ENTRYPOINT build/main ${PORT}
