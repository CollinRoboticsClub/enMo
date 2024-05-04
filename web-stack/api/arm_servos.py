import functools
import time

from adafruit_servokit import ServoKit

# Global debugging constant
MOCK_SERVOS_FOR_DEBUGGING = True

# TODO: Verify what the actual max/min values are
SERVO_MAX_ANGLE = 180
SERVO_MIN_ANGLE = -180


# Generic servo movement functions
def set_servo_angle(servo, absolute_angle):
    # Clamp values to valid range
    if absolute_angle > SERVO_MAX_ANGLE:
        print(f"provided angle is out of range, ignoring it")
        return
    if absolute_angle < SERVO_MIN_ANGLE:
        print(f"provided angle is out of range, ignoring it")
        return

    # Don't do anything if servo angle is already correct
    if absolute_angle == servo.angle:
        print(f"servo is already at this angle, ignoring it")
        return

    servo.angle = absolute_angle

    if MOCK_SERVOS_FOR_DEBUGGING:
        print(f"set {servo.name} to angle {absolute_angle}")
    else:
        print(f"set servo to angle {absolute_angle}")


def move_servo(servo, relative_angle):
    absolute_angle = servo.angle + relative_angle

    return set_servo_angle(servo, absolute_angle)


# These classes are just for easy debugging without needing to have the actual hardware attached
class MockServo:
    def __init__(self, name):
        self.name = name
        self.angle = 0


class MockServoKit:
    def __init__(self):
        servo_name_list = ["base", "shoulder", "elbow", "wrist", "gripper"]
        servo_instance_list = []
        for servo in servo_name_list:
            servo_instance_list.append(MockServo(servo))

        self.servo = servo_instance_list


class Arm:
    def __init__(self):
        if MOCK_SERVOS_FOR_DEBUGGING:
            kit = MockServoKit()
        else:
            kit = ServoKit(channels=16)

        # PINOUT
        # FIXME: As of writing, this is wrong lol -- fix on Friday!
        # I know we have 5 servos...
        self.base = kit.servo[0]
        self.shoulder = kit.servo[1]
        self.elbow = kit.servo[2]
        self.wrist = kit.servo[3]
        self.gripper = kit.servo[4]
        self.servo_list = kit.servo

        # Define absolute movement methods for all servos
        self.set_base_angle = functools.partial(set_servo_angle, self.base)
        self.set_shoulder_angle = functools.partial(set_servo_angle, self.shoulder)
        self.set_elbow_angle = functools.partial(set_servo_angle, self.elbow)
        self.set_wrist_angle = functools.partial(set_servo_angle, self.wrist)
        self.set_gripper_angle = functools.partial(set_servo_angle, self.gripper)

        # Define relative movement methods for all servos
        self.move_base = functools.partial(move_servo, self.base)
        self.move_shoulder = functools.partial(move_servo, self.shoulder)
        self.move_elbow = functools.partial(move_servo, self.elbow)
        self.move_wrist = functools.partial(move_servo, self.wrist)
        self.move_gripper = functools.partial(move_servo, self.gripper)


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
