#include "config.h"

// RGB VALUES
#define MIN 0                                           // Minimum Rgb Value
#define MAX 255                                         // Maximum Value
#define TRANSITION_SPEED 5                              // Setting the color change transition speed

// IO settings
#define trigPin 0                                       // Ultrasonic sensor - Trigger pine
#define echoPin 2                                       // Ultrasonic sensor - Echo pine
#define Red_led 16                                      // Rgb led - Red pin
#define Blue_led 4                                      // Rgb led - Blue pin
#define Green_led 5                                     // Rgb led - Green pin

// Needed variables
int RGB[3];                                             // Array containing the RGB values

// Adafruit feeds
AdafruitIO_Feed *red_feed = io.feed("red-value");       // Adafruit feed for the red color
AdafruitIO_Feed *blue_feed = io.feed("blue-value");     // Adafruit feed for the blue color
AdafruitIO_Feed *green_feed = io.feed("green-value");   // Adafruit feed for the green color


// _________________________________________________________ Message Handlers _________________________________________________________
// Method for handling a change in red value
void handleMessage_red(AdafruitIO_Data *data) {
  RGB[0] = data->toInt();                               // Converting the value to integer
}

// Method for handling a change in blue value
void handleMessage_blue(AdafruitIO_Data *data) {
  RGB[2] = data->toInt();                               // Converting the value to integer
}

// Method for handling a change in green value
void handleMessage_green(AdafruitIO_Data *data) {
  RGB[1] = data->toInt();                               // Converting the value to integer
  faid_in();                                            // Faiding in the colors
  delay(800);                                           // Waiting for 800ms
  faid_out();                                           // Faiding out the colors
}


// ___________________________________________________________ RGB Methods _____________________________________________________________
// Method for creating and sending the rgb values to the other lamp.
void create_and_send_rgb() {
  RGB[0] = random(MIN, MAX);                            // Creating a new value for the Red color
  RGB[1] = random(MIN, MAX);                            // Creating a new value for the Blue color
  RGB[2] = random(MIN, MAX);                            // Creating a new value for the Green color

  red_feed->save(RGB[0]);                               // Updating the red value on adafruit
  blue_feed->save(RGB[2]);                              // Updating the blue value on adafruit
  green_feed->save(RGB[1]);                             // Updating the green value on adafruit
}


// Method updating the rgb values in each loop
void update_rgb() {
  analogWrite(Red_led, RGB[0]);                         // Updating the red led value
  analogWrite(Blue_led, RGB[2]);                        // Updating the blue led value
  analogWrite(Green_led, RGB[1]);                       // Updating the green led value
}

// Method for slowly terning off the leds
void faid_in() {
  for (int i = RGB[0]; i >= 0; i--) {                   // Decreasing the value of red
    analogWrite(Red_led,i);
    delay(TRANSITION_SPEED);
  }
  for (int i = RGB[2]; i >= 0; i--) {                   // Decreasing the value of green 
    analogWrite(Green_led,i);
    delay(TRANSITION_SPEED);
  }
  for (int i = RGB[1]; i >= 0; i--) {                   // Decreasing the value of blue
    analogWrite(Blue_led,i);
    delay(TRANSITION_SPEED);
  }
}

// Method for slowly terning on the leds
void faid_out() {
  for (int i = 0; i <= RGB[0]; i++) {                   // Increasing the value of red
    analogWrite(Red_led, i);
    delay(TRANSITION_SPEED);
  }
  for (int i = 0; i <= RGB[1]; i++) {                   // Increasing the value of green
    analogWrite(Green_led, i);
    delay(TRANSITION_SPEED);
  }
  for (int i = 0; i <= RGB[2]; i++) {                   // Increasing the value of blue
    analogWrite(Blue_led, i);
    delay(TRANSITION_SPEED);
  }
}

// __________________________________________________________ Input Methods _____________________________________________________________
// Method for getting the distance of the hand
int get_distance() {
  long duration;                                        // Creating a variable for the duration
  int distance;                                         // Creating a variable for the distance

  digitalWrite(trigPin, LOW);                           // Setting the triggerpin off
  delayMicroseconds(2);                                 // Waiting for 2 microseconds

  digitalWrite(trigPin, HIGH);                          // Setting the triggerpin on
  delayMicroseconds(10);                                // Waiting for 10 microseconds
  digitalWrite(trigPin, LOW);                           // Setting the triggerpin off

  duration = pulseIn(echoPin, HIGH);                    // Calculating the duration through pulseIn
  distance = duration * 0.034 / 2;                      // Calculating the distance
  return distance;                                      // Returning the distance
}


// Method for checking if someone is close to the lamp (aka thinking about you)
void Am_i_thinking_about_you() {
  int dist = get_distance();                            // Getting the distance of the hand

  if (dist >= 3 && dist <= 5) {                         // Checking if the distance is between to 3 and 5 cm
    delay(800);                                         // Waiting for 0.8 seconds
    if (dist >= 3 && dist <= 5)                         // Checking again if the distance is between 3 to 5 cm
      create_and_send_rgb();                            // If true we create and send the new rgb values
  }
}


// ___________________________________________________________ Setup Method ______________________________________________________________
// Setup method
void setup() {
  Serial.begin(115200);                                 // Setting the baud rate (For esp8266)

  // ADAFRUIT SETUP
  io.connect();                                         // Connecting to the adafruit

  red_feed->onMessage(handleMessage_red);               // Setting the message handler for red color
  blue_feed->onMessage(handleMessage_blue);             // Setting the message handler for blue color
  green_feed->onMessage(handleMessage_green);           // Setting the message handler for green color

  while (io.status() < AIO_CONNECTED) {                 // Looping until we are connected to the adafruit
    Serial.println(".");
    delay(500);
  }

  // ADAFRUIT FEEDS
  red_feed->get();                                      // Setting the red feed get method
  blue_feed->get();                                     // Setting the blue feed get method
  green_feed->get();                                    // Setting the green feed get method

  // NEEDED VARIABLES
  RGB[0] = 0;                                           // Setting the current value red value
  RGB[1] = 0;                                           // Setting the current value blue value
  RGB[2] = 0;                                           // Setting the current value green value

  // HARDWARE SETUP
  pinMode(Red_led, OUTPUT);                             // Setting the red led pin as output
  pinMode(Blue_led, OUTPUT);                            // Setting the blue pin as output
  pinMode(Green_led, OUTPUT);                           // Setting the green led pin as output

  pinMode(trigPin, OUTPUT);                             // Setting the trigPin as an Output
  pinMode(echoPin, INPUT);                              // Setting the echoPin as an Input
}

// ___________________________________________________________ Loop Method ______________________________________________________________
void loop() {
  io.run();                                             // Keeping the program running.
  Am_i_thinking_about_you();                            // Checking if something is close to the lamp.
  update_rgb();                                         // Updating the colors
}
