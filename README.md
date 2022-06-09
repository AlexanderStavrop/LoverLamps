# LoverLamps

The perfect gift for you other half. 

If you want your other half to know when you are thinking about them, then you are in the right place.

I created a lamp, but its no ordinary lamp. Its magic, you can put your hand close to it and both lamps will change to new random color, at the same time,
so you loved one can know you are thinking about them. 

For each lamp, the following components are required
- 1 esp8266 or esp32 
- 1 ultra sonic sensor
- as many leds as you want.

Also, 3 different feeds need to be created on adafruit, one for each color.

## How it works
The esp connects to wifi and then connects to adafruit. It retrieves the corresponding rgb values and changes to that color. If an object is close to the 
lamp's ultrasonic sensor, then a new random value for each color is calulated and both lamps change at the same time.

## Lamps and PCB
will post pcb and wood work at some point.
