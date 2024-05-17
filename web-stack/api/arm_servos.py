import time

from adafruit_motor.servo import Servo
from adafruit_servokit import ServoKit


class SafeServo:
    def __init__(self, servo: Servo, initial_angle: int, min_angle: int, max_angle: int):
        self.servo = servo
        self.servo.angle = initial_angle
        self.min_angle = min_angle
        self.max_angle = max_angle

    def set_angle(self, absolute_angle):
        # Clamp values to valid range
        if absolute_angle > self.max_angle or absolute_angle < self.min_angle:
            # print(f"provided angle is out of range, ignoring it")
            return

        # Don't do anything if servo angle is already correct
        if absolute_angle == self.servo.angle:
            # print(f"servo is already at this angle, ignoring it")
            return

        self.servo.angle = absolute_angle

    def move(self, relative_angle):
        absolute_angle = self.servo.angle + relative_angle

        return self.set_angle(absolute_angle)


class Arm:
    def __init__(self):
        kit = ServoKit(channels=16)

        # PINOUT
        self.base = SafeServo(kit.servo[0], initial_angle=90, min_angle=70, max_angle=110)
        self.shoulder = SafeServo(kit.servo[1], initial_angle=90, min_angle=40, max_angle=180)
        self.elbow = SafeServo(kit.servo[2], initial_angle=90, min_angle=20, max_angle=135)
        self.wrist = SafeServo(kit.servo[3], initial_angle=90, min_angle=70, max_angle=110)
        self.gripper = SafeServo(kit.servo[4], initial_angle=90, min_angle=70, max_angle=150)

        self.servo_list = kit.servo

        # Define absolute movement methods for all servos
        self.set_base_angle = self.base.set_angle
        self.set_shoulder_angle = self.shoulder.set_angle
        self.set_elbow_angle = self.elbow.set_angle
        self.set_wrist_angle = self.wrist.set_angle
        self.set_gripper_angle = self.gripper.set_angle

        # Define relative movement methods for all servos
        self.move_base = self.base.move
        self.move_shoulder = self.shoulder.move
        self.move_elbow = self.elbow.move
        self.move_wrist = self.wrist.move
        self.move_gripper = self.gripper.move


def test():
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
    test()
    sweeping_test()
