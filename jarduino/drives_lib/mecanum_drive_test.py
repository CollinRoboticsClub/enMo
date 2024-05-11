# Just a simple algorithmic test.
# I've poorly just copied MecanumDrive's alg
# and returned the set'd speeds instead.


def drive(x: float, y: float, rotation: float):
    fl = y + x + rotation
    fr = y - x - rotation
    bl = y - x + rotation
    br = y + x - rotation

    return fl, fr, bl, br


def print_test(x, y, z):
    fl, fr, bl, br = [f"{x: .1f}" for x in drive(x, y, z)]
    # fmt: off
    output = \
    f"""
    {fl} === {fr}
      |        |
      |        |
      |        |
    {bl} === {br}
    """
    # fmt: on

    print(f"Testing with x={x:.1f}, y={y:.1f}, z={z:.1f}:")
    print(output)


from scripts_for_testing.f310 import F310
import time
import serial

controller = F310()

# with serial.Serial(port="COM4", baudrate=9600, timeout=0.5) as bot:

while True:
    controller.update()

    x = controller.left_x()
    y = controller.left_y()
    z = controller.right_x()

    print_test(x, y, z)

    # message = f"{x} {y} {z}\n"
    # print(message)
    # bot.write(message.encode())

    # time.sleep(0.01)
    time.sleep(1)
