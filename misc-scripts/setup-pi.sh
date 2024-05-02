#!/usr/bin/env bash

# Basic setup script for the robot's Raspberry Pi 4
#
# It's not perfect because honestly it's more of a setup reference
# than something we actually run regularly, but it should be good enough.

# TODO: enable USB boot in Pi's firmware just in case some sd card shenanigans happen

# Constants
readonly CONTAINER_DIR=/opt/chessbot

# make sure we're running as root
if [ "$EUID" -ne 0 ]; then
	echo "Please run as root"
	return 1
fi

# Update and upgrade
apt update
apt upgrade

# Apparently required for the arm servos
apt install i2c-tools
raspi-config nonint do_i2c 0 # enable i2c

# Install docker
apt install docker.io docker-compose-plugin # might wanna add Docker's apt repo for Compose V2???

# Make docker-compose file
mkdir -p "$CONTAINER_DIR" || return 2
cat <<-'EOF' >"$CONTAINER_DIR/docker-compose.yml"
	services:
	  web-stack:
	    container_name: chessbot
	    image: ghcr.io/collinroboticsclub/chessbot-pi:latest
	    privileged: true
	    volumes:
	      - type: bind
	        source: /dev/ttyUSB0 # might need to change this
	        target: /dev/ttyUSB0 # DO NOT CHANGE THIS ONE
	    ports:
	      - 80:8000
	    restart: unless-stopped
EOF

# Run container
cd "$CONTAINER_DIR" || {
	echo "failed to cd to dir: $CONTAINER_DIR"
	return 3
}
docker compose down # hacky way to make sure possibly previously running container is removed fully
docker compose up --detach --force-recreate --pull=always
