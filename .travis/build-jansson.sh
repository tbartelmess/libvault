#!/usr/bin/env bash

cd /tmp
wget http://www.digip.org/jansson/releases/jansson-2.9.tar.gz
tar xfvz jansson-2.9.tar.gz
cd jansson-2.9
make install
