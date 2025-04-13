FROM alpine:latest AS builder

WORKDIR /app

COPY . .

RUN apk update && apk add cmake g++ make && \
    cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --config Release -j $(nproc)

ENTRYPOINT ["./build/0_root/api"]
