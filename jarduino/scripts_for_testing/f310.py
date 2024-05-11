import pygame


class F310:
    def __init__(self):
        pygame.init()
        pygame.joystick.init()
        joystick_count = pygame.joystick.get_count()
        if joystick_count == 0:
            print("No joysticks found")
            return None
        self.joystick = pygame.joystick.Joystick(0)
        self.joystick.init()

        self.deadzone = 0.07

    def update(self):
        """
        Call this once per loop
        """
        pygame.event.pump()

    def apply_deadzone(self, value):
        if abs(value) < self.deadzone:
            return 0
        else:
            return value

    # region axes
    def get_hat(self):
        return self.joystick.get_hat(0)

    def left_x(self):
        return self.apply_deadzone(self.joystick.get_axis(0))

    def left_y(self):
        """
        Returns positive UP
        """
        return self.apply_deadzone(-self.joystick.get_axis(1))

    def right_x(self):
        return self.apply_deadzone(self.joystick.get_axis(2))

    def right_y(self):
        """
        Returns positive UP
        """
        return self.apply_deadzone(-self.joystick.get_axis(3))

    def left_trigger(self):
        return self.joystick.get_axis(4)

    def right_trigger(self):
        return self.joystick.get_axis(5)

    # endregion axes

    # region buttons
    def a(self):
        return self.joystick.get_button(0)

    def b(self):
        return self.joystick.get_button(1)

    def x(self):
        return self.joystick.get_button(2)

    def y(self):
        return self.joystick.get_button(3)

    def left_bumper(self):
        return self.joystick.get_button(4)

    def right_bumper(self):
        return self.joystick.get_button(5)

    def back(self):
        return self.joystick.get_button(6)

    def start(self):
        return self.joystick.get_button(7)

    def left_stick_press(self):
        return self.joystick.get_button(8)

    def right_stick_press(self):
        return self.joystick.get_button(9)

    # endregion buttons


if __name__ == "__main__":
    import colorama

    controller = F310()

    def make_green_if_true(b):
        return (
            colorama.Fore.GREEN + str(b) + colorama.Fore.RESET if b else str(b)
        )

    def format_ax(x):
        sx = f"{x: 1.2f}"

        if x > 0:
            return colorama.Fore.GREEN + sx + colorama.Fore.RESET
        elif x < 0:
            return colorama.Fore.RED + sx + colorama.Fore.RESET
        else:
            return colorama.Fore.BLUE + sx + colorama.Fore.RESET

    ms = 0
    while True:
        controller.update()

        output = "| "

        output += f"{format_ax(controller.left_x())}, {format_ax(controller.left_y())}  | "
        output += f"{format_ax(controller.right_x())}, {format_ax(controller.right_y())}  | "
        output += f"{format_ax(controller.left_trigger())} "
        output += f"{format_ax(controller.right_trigger())} "
        output += "| "

        output += f"{make_green_if_true(controller.a())} "
        output += f"{make_green_if_true(controller.b())} "
        output += f"{make_green_if_true(controller.x())} "
        output += f"{make_green_if_true(controller.y())} "
        output += " "
        output += f"{make_green_if_true(controller.left_bumper())} "
        output += f"{make_green_if_true(controller.right_bumper())} "
        output += " "
        output += f"{make_green_if_true(controller.back())} "
        output += f"{make_green_if_true(controller.start())} "
        output += " "
        output += f"{make_green_if_true(controller.left_stick_press())} "
        output += f"{make_green_if_true(controller.right_stick_press())} "
        output += "| "

        hatx, haty = controller.get_hat()
        output += f"{format_ax(hatx)}, {format_ax(haty)} |"

        if ms > 75:
            print(output)
            ms = 0

        ms += pygame.time.delay(10)
