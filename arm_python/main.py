# https://learn.adafruit.com/adafruit-16-channel-servo-driver-with-raspberry-pi/overview

# Downloaded the PDF :) it's in here!

# sudo apt-get install python-smbus
# sudo apt-get install i2c-tools # not strictly required!

import time
from adafruit_servokit import ServoKit

# PINOUT
# FIXME As of writing, this is wrong lol -- fix on Friday!
# I know we have 5 servos...
kit = ServoKit(channels=16)


def main():
    for i in range(6):
        # Testing
        print(f"Setting servo {i} to 90 degrees")
        kit.servo[i].angle = 90

        time.sleep(1)

    pass


if __name__ == "__main__":
    main()
