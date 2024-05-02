#!/bin/sh

# create fake serial devices
sudo socat -v \
	PTY,raw,echo=0,link=/dev/ttyUSB0 \
	PTY,raw,echo=0,link=/dev/ttyUSB1
