FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++ cmake git curl \
    libboost-all-dev  # Установка библиотеки Boost

RUN git clone https://github.com/ipkn/crow.git /crow

WORKDIR /app

COPY src/ src/
COPY CMakeLists.txt .

RUN cmake -DCROW_INCLUDE_DIR=/crow/include . && make

COPY web/ web/

CMD ["./web_server"]