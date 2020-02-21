from ..Modules import ArduinoI2CBus
import time


if __name__ == '__main__':
    # create the fake "Arduino Bus" of potential devices
    Arduino_bus = ArduinoI2CBus()

    # create an encoder device on the Arduino
    servo = Arduino_bus.create_servo_dev(pin=9, min_period=1000, max_period=1900)
    encoder = Arduino_bus.create_encoder_dev(pin_A=19, pin_B=32)

    servo.attach()

    for i in range(100):
        servo.write_percentage(i/100)
        print(i, '% -- servo position:', encoder.get_angle())
        time.sleep(0.1)

    servo.detach()
    time.sleep(1)
    servo.detach()
