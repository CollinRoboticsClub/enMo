import serial


class ArduinoSerialConnection:
    def __init__(self, device, baudrate=9600, timeout=0.5):
        self.serial_connection = serial.Serial(port=device, baudrate=baudrate, timeout=timeout)

    def send(self, input_str: str):
        self.serial_connection.write(f"{input_str}\n".encode())

    def send_array(self, input_arr: list[str]):
        for input_str in input_arr:
            self.send(input_str)
