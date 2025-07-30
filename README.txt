The device measures the distance to various solid (non-soft) objects using an ultrasonic sensor.

Components used:
Arduino Nano
OLED Display
Ultrasonic Sensor
Boost Converter (0.9–5V to 5V)
AA 1.5V Battery
Tactile Button (sensor button)
Power Switch

The concept is simple: turn the device on, and it displays distance readings in both centimeters and feet/inches.
According to the sensor’s datasheet, the maximum measuring range is 400 cm, but in practice it’s around 3.5 meters.

Holding the button for more than 1 second will freeze/unfreeze the displayed value, making it easier to read or record.

At short distances (~5–20 cm), the sensor has a ±2 cm inaccuracy due to its limitations. Overall, the device offers an estimated accuracy of about 90%.

If the distance is too small(<2cm) or large(>350cm) then the values get too imprecise, and "OutOfRange" is displayed.