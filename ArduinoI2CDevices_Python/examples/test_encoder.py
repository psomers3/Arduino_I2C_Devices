from ArduinoI2CDevices_Python import ArduinoI2CBus
import time


if __name__ == '__main__':
    # create the fake "Arduino Bus" of potential devices
    Arduino_bus = ArduinoI2CBus()

    # create an encoder device on the Arduino
    encoder = Arduino_bus.create_encoder_dev(pin_A=18, pin_B=24)

    while 1:
        time.sleep(.1)  # print every 0.1 seconds
        print('angle', encoder.get_angle())
