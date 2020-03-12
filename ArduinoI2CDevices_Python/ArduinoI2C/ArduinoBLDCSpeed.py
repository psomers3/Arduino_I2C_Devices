from smbus2 import SMBus, i2c_msg
import struct

commands = {
    'GET_SPEED': 0x01,
}

FWD_TO_DEV = 0x03


class ArduinoEncoder(object):
    def __init__(self, i2c_bus: SMBus, arduino_addr, dev_id):
        self.i2c_bus = i2c_bus
        self.arduino_addr = arduino_addr
        self.dev_id = dev_id

    def get_speed(self):
        write_msg = i2c_msg.write(self.arduino_addr, [FWD_TO_DEV, self.dev_id, commands['GET_SPEED']])
        read_msg = i2c_msg.read(self.arduino_addr, 4)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)
        return struct.unpack('<f', read_msg.buf[0:4])[0]
