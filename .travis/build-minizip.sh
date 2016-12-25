#!/usr/bin/env sh

set -e
set -x

rm -rf /tmp/minizip
git clone https://github.com/nmoinvaz/minizip.git /tmp/minizip
cd /tmp/minizip
cmake .
sudo make install
