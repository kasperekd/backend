FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    wget \
    build-essential \
    libasio-dev \
    libpq-dev \
    libpqxx-dev \
    && rm -rf /var/lib/apt/lists/*

# RUN wget https://github.com/jtv/libpqxx/archive/7.7.5.tar.gz \
#     && tar -xzf 7.7.5.tar.gz \
#     && cd libpqxx-7.7.5 \
#     && cmake . && make && make install

WORKDIR /app

COPY . .

RUN mkdir build && cd build && cmake .. && make

CMD ["./build/calculation_module"]
