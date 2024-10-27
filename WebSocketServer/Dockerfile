FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    g++ cmake

WORKDIR /app

COPY src/ src/
COPY CMakeLists.txt .

RUN cmake . && make

CMD ["./websocket_server"]
