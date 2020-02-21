import pylibi2c
import struct

commands = {
    'WRITE': 0x00,
    'WRITE_MS': 0x01,
    'ATTACH': 0x02,
    'DETACH': 0x03
}


class ArduinoServo(object):
    def __init__(self,
                 i2c_dev: pylibi2c.I2CDevice,
                 arduino_addr,
                 dev_id,
                 min_period=1000,
                 max_period=2000):
        self.min_period = min_period
        self.max_period = max_period
        self.range = self.max_period - self.min_period
        self.i2c_dev = i2c_dev
        self.arduino_addr = arduino_addr
        self.dev_id = (dev_id << 4) | 4

    def write(self, angle: float):  # from 0-180
        self.write_percentage(angle/180)
        # as_int = int(angle*100)
        # short = struct.pack('H', as_int)
        # message = struct.pack('bb', self.dev_id, commands['WRITE'])
        # new_message = bytearray(4)
        # new_message[0:1] = message
        # new_message[2:] = short
        # final_message = bytes(new_message)
        # message = int.from_bytes(final_message, byteorder='big')  # convert to number for i2c library
        # self.i2c_dev.read(message, 1)

    def write_micro_seconds(self, micro_seconds: int):
        short = struct.pack('H', micro_seconds)
        message = struct.pack('bb', self.dev_id, commands['WRITE_MS'])
        new_message = bytearray(4)
        new_message[0:1] = message
        new_message[2:] = short
        final_message = bytes(new_message)
        message = int.from_bytes(final_message, byteorder='big')  # convert to number for i2c library
        self.i2c_dev.read(message, 1)

    def write_percentage(self, percent: float):  # 0-100
        self.write_micro_seconds(int(self.range*percent + self.min_period))

    def attach(self):
        message = struct.pack('bbbb', self.dev_id, commands['ATTACH'], 0x00, 0x00)
        message = int.from_bytes(message, byteorder='big')  # convert to number for i2c library
        self.i2c_dev.read(message, 1)

    def detach(self):
        message = struct.pack('bbbb', self.dev_id, commands['DETACH'], 0x00, 0x00)
        message = int.from_bytes(message, byteorder='big')  # convert to number for i2c library
        self.i2c_dev.read(message, 1)
