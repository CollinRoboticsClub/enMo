#!/bin/sh

# Run `sudo modprobe v4l2loopback exclusive_caps=1` before running this script.
# You should only need to do it once until you reboot unless you `modprobe -r` it.

# create fake camera device and stream video to it
VIDEO_FILE_PATH="${1:-$HOME/Videos/numbered-frames.mkv}"
ffmpeg -re -i "$VIDEO_FILE_PATH" -s 1280x720 -vcodec rawvideo -pix_fmt yuv420p -f v4l2 /dev/video0
