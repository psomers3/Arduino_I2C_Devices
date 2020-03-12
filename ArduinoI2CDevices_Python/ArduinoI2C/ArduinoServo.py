from smbus2 import SMBus, i2c_msg
import struct

commands = {
    'WRITE': 0x00,
    'WRITE_MS': 0x01,
    'ATTACH': 0x02,
    'DETACH': 0x03
}

FWD_TO_DEV = 0x03


class ArduinoServo(object):
    def __init__(self,
                 i2c_bus: SMBus,
                 arduino_addr,
                 dev_id,
                 min_period=1000,
                 max_period=2000):
        self.min_period = min_period
        self.max_period = max_period
        self.range = self.max_period - self.min_period
        self.i2c_bus = i2c_bus
        self.arduino_addr = arduino_addr
        self.dev_id = dev_id

    def write(self, angle: float):  # from 0-180
        self.write_percentage(angle/180)

    def write_micro_seconds(self, micro_seconds: int):
        short = struct.pack('H', micro_seconds)
        write_msg = i2c_msg.write(self.arduino_addr, [FWD_TO_DEV, self.dev_id, commands['WRITE_MS'], short[0], short[1]])
        read_msg = i2c_msg.read(self.arduino_addr, 1)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)

    def write_percentage(self, percent: float):  # 0-100
        self.write_micro_seconds(int(self.range*percent + self.min_period))

    def attach(self):
        write_msg = i2c_msg.write(self.arduino_addr, [FWD_TO_DEV, self.dev_id, commands['ATTACH']])
        read_msg = i2c_msg.read(self.arduino_addr, 1)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)

    def detach(self):
        write_msg = i2c_msg.write(self.arduino_addr, [FWD_TO_DEV, self.dev_id, commands['DETACH']])
        read_msg = i2c_msg.read(self.arduino_addr, 1)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)
