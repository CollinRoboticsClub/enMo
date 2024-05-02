#!/usr/bin/env bash

# TODO:
# - enable USB boot in Pi's firmware just in case some sd card shenanigans happen
# - test this script

# Constants
readonly CONTAINER_DIR=/opt/chessbot

# make sure we're running as root
if [ "$EUID" -ne 0 ]; then
	echo "Please run as root"
	return 1
fi

# Install docker
apt update
apt upgrade
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
