from HardwareDrivers.ArduinoI2CBus.ArduinoI2CBus import ArduinoI2CBus
import time


if __name__ == '__main__':
    Arduino_bus = ArduinoI2CBus()
    encoder = Arduino_bus.create_encoder_dev(18, 24)  # type: HardwareDrivers.ArduinoI2CBus.ArduinoEncoder
    time.sleep(0.5)
    encoder.get_speed()
    is_degrees = True

    while 1:
        for _ in range(500):
            time.sleep(.01)
            print('angle', encoder.get_angle())
        #is_degrees = not is_degrees
        encoder.set_degrees(is_degrees)
