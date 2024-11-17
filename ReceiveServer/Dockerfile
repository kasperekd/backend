FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    wget \
    # libboost-all-dev \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

RUN wget https://github.com/CrowCpp/Crow/releases/download/v1.2.0/Crow-1.2.0-Linux.deb \
    && dpkg -i Crow-1.2.0-Linux.deb \
    && rm Crow-1.2.0-Linux.deb

WORKDIR /app

COPY . .

RUN mkdir build && cd build && cmake .. && make

CMD ["./build/receive_server"]
