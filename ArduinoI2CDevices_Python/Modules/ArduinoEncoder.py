from smbus2 import SMBus, i2c_msg
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

FWD_TO_DEV = 0x03


class ArduinoEncoder(object):
    def __init__(self, i2c_bus: SMBus, arduino_addr, dev_id):
        self.i2c_bus = i2c_bus
        self.arduino_addr = arduino_addr
        self.dev_id = dev_id

    def get_position(self):
        write_msg = i2c_msg.write(self.arduino_addr, [FWD_TO_DEV, self.dev_id, commands['GET_POSITION']])
        read_msg = i2c_msg.read(self.arduino_addr, 4)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)
        return struct.unpack('I', read_msg.buf[0:4])[0]

    def get_speed(self):
        write_msg = i2c_msg.write(self.arduino_addr, [FWD_TO_DEV, self.dev_id, commands['GET_SPEED']])
        read_msg = i2c_msg.read(self.arduino_addr, 4)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)
        return struct.unpack('<f', read_msg.buf[0:4])[0]

    def get_angle(self):
        write_msg = i2c_msg.write(self.arduino_addr, [FWD_TO_DEV, self.dev_id, commands['GET_ANGLE']])
        read_msg = i2c_msg.read(self.arduino_addr, 4)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)
        return struct.unpack('<f', read_msg.buf[0:4])[0]

    def set_degrees(self, use_degrees: bool):
        write_msg = i2c_msg.write(self.arduino_addr, [FWD_TO_DEV, self.dev_id, commands['SET_DEGREES'], use_degrees])
        read_msg = i2c_msg.read(self.arduino_addr, 1)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)

    def set_resolution(self, pulses_per_rev: int):
        as_bytes = struct.unpack('2B', struct.pack('H', pulses_per_rev))
        write_msg = i2c_msg.write(self.arduino_addr,
                                  [FWD_TO_DEV,
                                   self.dev_id,
                                   commands['SET_RESOLUTION'],
                                   as_bytes[0],
                                   as_bytes[1]])
        read_msg = i2c_msg.read(self.arduino_addr, 1)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)

    def zero(self):
        write_msg = i2c_msg.write(self.arduino_addr, [FWD_TO_DEV, self.dev_id, commands['ZERO']])
        read_msg = i2c_msg.read(self.arduino_addr, 1)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)


