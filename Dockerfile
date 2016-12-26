FROM ubuntu:latest

RUN apt-get update
RUN apt-get install -y cmake libjansson-dev libminizip-dev check clang-3.8 build-essential

ADD . /src
WORKDIR /src
RUN cmake .
RUN make
