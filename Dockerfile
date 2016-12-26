FROM ubuntu:latest

RUN apt-get update
RUN apt-get install -y cmake libjansson-dev libminizip-dev clang-3.8 build-essential libcurl4-openssl-dev
RUN apt-get install zlib1g-dev 
ADD . /src
WORKDIR /src
RUN cmake .
RUN make
