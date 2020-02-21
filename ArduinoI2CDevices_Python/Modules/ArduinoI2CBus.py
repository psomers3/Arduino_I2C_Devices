import ctypes
import pylibi2c
import struct
from .ArduinoEncoder import ArduinoEncoder
from .ArduinoServo import ArduinoServo

first_byte_cmd = {
    'NEW_SERVO_DEVICE': 0x00,
    'NEW_ENCODER_DEVICE': 0x01,
    'CLEAR_DEVICES': 0x02,
    'FWD_TO_DEVICES': 0x03,
    'SET_ENCODER_UPDATE_FREQ': 0x04
}


class ArduinoI2CBus(object):
    def __init__(self, i2c_bus: str = r'/dev/i2c-1', arduino_addr=4):
        self.i2c_dev = pylibi2c.I2CDevice(i2c_bus, arduino_addr, iaddr_bytes=4)
        self.arduino_addr = struct.pack('b', arduino_addr)
        self.devices = []
        self.clear_all_devices()

    def clear_all_devices(self):
        message = struct.pack('bbbb', first_byte_cmd['CLEAR_DEVICES'], 0x00, 0x00, 0x00)
        message = int.from_bytes(message, byteorder='big')  # convert to number for i2c library
        self.i2c_dev.read(message, 1)

    def create_encoder_dev(self, pin_A: int, pin_B: int) -> ArduinoEncoder:
        """

        :param pin_A: encoder A wire
        :param pin_B: encoder B wire
        :return:
        """
        message = struct.pack('bbbb', first_byte_cmd['NEW_ENCODER_DEVICE'], pin_A, pin_B, 0x00)
        message = int.from_bytes(message, byteorder='big')  # convert to number for i2c library
        device_id = int.from_bytes(self.i2c_dev.read(message, 1), byteorder='big')

        new_encoder = ArduinoEncoder(self.i2c_dev, self.arduino_addr, device_id)
        self.devices.append(new_encoder)
        return new_encoder  # type: ArduinoEncoder

    def set_encoder_update_freq(self, freq: int):
        """
        Will change update rate for all encoder devices on the arduino
        :param freq: global update frequency in Hz
        """
        short = struct.pack('H', freq)
        message = struct.pack('b',first_byte_cmd['SET_ENCODER_UPDATE_FREQ'])
        new_message = bytearray(4)
        new_message[0] = message
        new_message[1:2] = short
        new_message[-1] = bytes(0x00)
        final_message = bytes(new_message)
        message = int.from_bytes(final_message, byteorder='big')  # convert to number for i2c library
        response = self.i2c_dev.read(message, 1)

    def create_servo_dev(self, pin: int, min_period=1000, max_period=2000) -> ArduinoServo:
        """
        :param pin_A: pwm output pin
        :return: a handle to the new servo object
        """
        message = struct.pack('bbbb', first_byte_cmd['NEW_SERVO_DEVICE'], pin, 0x00, 0x00)
        message = int.from_bytes(message, byteorder='big')  # convert to number for i2c library
        device_id = int.from_bytes(self.i2c_dev.read(message, 1), byteorder='big')

        new_servo = ArduinoServo(self.i2c_dev, self.arduino_addr, device_id, min_period=min_period, max_period=max_period)
        self.devices.append(new_servo)
        return new_servo  # type: ArduinoServo
