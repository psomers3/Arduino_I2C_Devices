import pylibi2c
import struct

commands = {
    'GET_POSITION': 0x00,
    'GET_SPEED': 0x01,
    'GET_ANGLE': 0x02,
    'ZERO': 0x03,
    'SET_UPDATE_FREQ': 0x04,
    'SET_DEGREES': 0x05,
    'SET_RESOLUTION': 0x06
}


class ArduinoEncoder(object):
    def __init__(self, i2c_dev: pylibi2c.I2CDevice, arduino_addr, dev_id):
        self.i2c_dev = i2c_dev
        self.arduino_addr = arduino_addr
        self.dev_id = (dev_id << 4) | 4

    def get_position(self):
        message = struct.pack('bbbb',self.dev_id, commands['GET_POSITION'], 0x00, 0x00)
        message = int.from_bytes(message, byteorder='big')  # convert to number for i2c library
        position = struct.unpack('I', self.i2c_dev.read(message, 4))
        return position

    def get_speed(self):
        message = struct.pack('bbbb', self.dev_id, commands['GET_SPEED'], 0x00, 0x00)
        message = int.from_bytes(message, byteorder='big')  # convert to number for i2c library
        speed = struct.unpack('<f', self.i2c_dev.read(message, 4))
        return speed

    def get_angle(self):
        message = struct.pack('bbbb', self.dev_id, commands['GET_ANGLE'], 0x00, 0x00)
        message = int.from_bytes(message, byteorder='big')  # convert to number for i2c library
        angle = struct.unpack('<f', self.i2c_dev.read(message, 4))
        return angle

    def set_degrees(self, use_degrees: bool):
        message = struct.pack('bbbb', self.dev_id, commands['SET_DEGREES'], use_degrees, 0x00)
        message = int.from_bytes(message, byteorder='big')  # convert to number for i2c library
        response = struct.unpack('<f', self.i2c_dev.read(message, 4))

    def set_resolution(self, pulses_per_rev: int):
        message = struct.pack('bbbb', self.dev_id, commands['SET_RESOLUTION'], pulses_per_rev)
        message = int.from_bytes(message, byteorder='big')  # convert to number for i2c library
        response = struct.unpack('<f', self.i2c_dev.read(message, 4))


