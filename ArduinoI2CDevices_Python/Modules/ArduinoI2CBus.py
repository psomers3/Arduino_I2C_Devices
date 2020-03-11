import smbus2
import struct
from .ArduinoEncoder import ArduinoEncoder
from .ArduinoServo import ArduinoServo

first_byte_cmd = {
    'NEW_SERVO_DEVICE': 0x00,
    'NEW_ENCODER_DEVICE': 0x01,
    'CLEAR_DEVICES': 0x02,
    'FWD_TO_DEVICES': 0x03,
    'SET_ENCODER_UPDATE_FREQ': 0x04,
    'NEW_BLDC_ENCODER_DEVICE': 0x05
}


class ArduinoI2CBus(object):
    def __init__(self, i2c_bus: smbus2.SMBus, arduino_addr=4):
        if isinstance(i2c_bus, int):
            self.i2c_bus = smbus2.SMBus(bus=i2c_bus)
        else:
            self.i2c_bus = i2c_bus
        self.arduino_addr = arduino_addr
        self.devices = []
        self.clear_all_devices()

    def clear_all_devices(self):
        write_msg = smbus2.i2c_msg.write(self.arduino_addr, [first_byte_cmd['CLEAR_DEVICES']])
        read_msg = smbus2.i2c_msg.read(self.arduino_addr, 1)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)

    def create_BLDC_encoder_dev(self, pin_A: int, pin_B: int, pin_C: int) -> ArduinoEncoder:
        """
        :param pin_A: Hall Effect A wire
        :param pin_B: Hall Effect B wire
        :param pin_C: Hall Effect C wire
        :return: a handle to new encoder device
        """
        write_msg = smbus2.i2c_msg.write(self.arduino_addr, [first_byte_cmd['NEW_BLDC_ENCODER_DEVICE'], pin_A, pin_B, pin_C])
        read_msg = smbus2.i2c_msg.read(self.arduino_addr, 1)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)
        new_encoder = ArduinoEncoder(self.i2c_bus, self.arduino_addr, ord(read_msg.buf[0]))
        self.devices.append(new_encoder)
        return new_encoder  # type: ArduinoEncoder

    def create_encoder_dev(self, pin_A: int, pin_B: int) -> ArduinoEncoder:
        """
        :param pin_A: encoder A wire
        :param pin_B: encoder B wire
        :return: a handle to new encoder device
        """
        write_msg = smbus2.i2c_msg.write(self.arduino_addr, [first_byte_cmd['NEW_ENCODER_DEVICE'], pin_A, pin_B])
        read_msg = smbus2.i2c_msg.read(self.arduino_addr, 1)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)
        new_encoder = ArduinoEncoder(self.i2c_bus, self.arduino_addr, ord(read_msg.buf[0]))
        self.devices.append(new_encoder)
        return new_encoder  # type: ArduinoEncoder

    def set_encoder_update_freq(self, freq: int):
        """
        Will change update rate for all encoder devices on the arduino
        :param freq: global update frequency in Hz
        """
        short = struct.unpack('2B', struct.pack('H', freq))
        write_msg = smbus2.i2c_msg.write(self.arduino_addr, [first_byte_cmd['SET_ENCODER_UPDATE_FREQ'], short[0], short[1]])
        read_msg = smbus2.i2c_msg.read(self.arduino_addr, 1)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)

    def create_servo_dev(self, pin: int, min_period: int = 1000, max_period: int = 2000) -> ArduinoServo:
        """
        :param pin: pwm output pin
        :param min_period: pulse length in milliseconds of minimum servo movement.
        :param max_period: pulse length in milliseconds of maximum servo movement.
        :return: a handle to the new servo object
        """
        write_msg = smbus2.i2c_msg.write(self.arduino_addr, [first_byte_cmd['NEW_SERVO_DEVICE'], pin])
        read_msg = smbus2.i2c_msg.read(self.arduino_addr, 1)
        self.i2c_bus.i2c_rdwr(write_msg, read_msg)
        new_servo = ArduinoServo(self.i2c_bus, self.arduino_addr, ord(read_msg.buf[0]), min_period=min_period, max_period=max_period)
        self.devices.append(new_servo)
        return new_servo  # type: ArduinoServo
