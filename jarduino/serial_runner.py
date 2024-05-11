from f310 import F310
import pygame, sys, serial

# pip install pygame pyserial # if you need it lol

"""
A quick script to take in Logitech F310 or similar inputs
    from your local system, then send some translated values
    to a connected Arduino over Serial in the format of (x, y, rotation).
"""

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("CLI ARG EXPECTED: serial_runner.py [serial port name]")
        print("\tlooks like COM4 (Windows) or /dev/ttyusb0 (Linux)")
        exit(-1)

    controller = F310()

    # I use a baudrate of 9600 by convention - Josh
    # but you would match this with whatever the Arduino is reading at!!!
    with serial.Serial(port=sys.argv[1], baudrate=9600) as arduino_ser:

        def write_duino(x: float, y: float, rot: float):
            msg = bytes(f"{x:.2f} {y:.2f} {rot:.2f}", "utf-8")
            arduino_ser.write(msg)

        # arduino_ser.write()
        while True:
            controller.update()
            x = controller.left_x()  # left/right strafe
            y = controller.left_y()  # forward/backward strafe
            rot = controller.right_y()  # turning in-place
            # (of course, you can and should be combining all those motions when driving)
            write_duino(x, y, rot)

            pygame.time.delay(10)
