import time

from adafruit_servokit import ServoKit


class Arm:
    def __init__(self):
        # PINOUT
        # FIXME: As of writing, this is wrong lol -- fix on Friday!
        # I know we have 5 servos...
        kit = ServoKit(channels=16)

        # optionally can use these vars for named servo access:
        self.base = kit.servo[0]
        self.shoulder = kit.servo[1]
        self.elbow = kit.servo[2]
        self.wrist = kit.servo[3]
        self.gripper = kit.servo[4]

        self.servo_list = kit.servo


# Simple test function
def main():
    arm = Arm()
    servos = arm.servo_list
    for i in range(5):
        # Testing
        print(f"Setting servo {i} to 90 degrees")
        servos[i].angle = 90

        time.sleep(1)


def sweeping_test():
    # WEEPING test bruh
    pass


# If script is ran interactively, run through some basic tests
if __name__ == "__main__":
    main()
