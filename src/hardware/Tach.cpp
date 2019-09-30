#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
#include <Math.h>

// namespace {
//   const int OLED_RESET = 4;
//   const int TEXT_SIZE_SMALL = 1;
//   const int TEXT_SIZE_LARGE = 2;
//   const int ONE_K = 1000;
  
//   const int OLED_HEIGHT = 64;
//   const int OLED_WIDTH = 128;
//   const int YELLOW_SEGMENT_HEIGHT = 16;
//   const int DISPLAY_FULL_BRIGHTNESS = 255;
//   const int DISPLAY_DIM_BRIGHTNESS = 0;
  
//   const int IR_LED_PIN_3 = 3;
//   const int PHOTODIODE_PIN_2 = 2;
//   const int INTERRUPT_ZERO_ON_PIN_2 = 0;
  
//   const uint16_t DIAL_CENTER_X = OLED_WIDTH / 2;
//   const uint16_t DIAL_RADIUS = (OLED_HEIGHT - YELLOW_SEGMENT_HEIGHT) - 1;
//   const uint16_t DIAL_CENTER_Y = OLED_HEIGHT - 1;
//   const uint16_t INDICATOR_LENGTH = DIAL_RADIUS - 5;
//   const uint16_t INDICATOR_WIDTH = 5;
//   const uint16_t LABEL_RADIUS = DIAL_RADIUS - 18;
//   const int DIAL_LABEL_Y_OFFSET = 6;
//   const int DIAL_LABEL_X_OFFSET = 4;
  
//   const long MAJOR_TICKS[] = { 0, 10000, 20000, 30000 };
//   const int MAJOR_TICK_COUNT = sizeof(MAJOR_TICKS) / sizeof(MAJOR_TICKS[0]);
//   const int  MAJOR_TICK_LENGTH = 7;
//   const long MINOR_TICKS[] = {5000, 15000, 25000};
//   const int MINOR_TICK_COUNT = sizeof(MINOR_TICKS) / sizeof(MINOR_TICKS[0]);
//   const int MINOR_TICK_LENGTH = 3;
  
//   const uint16_t DIAL_MAX_RPM = MAJOR_TICKS[MAJOR_TICK_COUNT-1];
  
//   const int HALF_CIRCLE_DEGREES = 180;
//   const float PI_RADIANS = PI/HALF_CIRCLE_DEGREES;
  
//   const double MILLIS_PER_SECOND = 1000.0;
//   const double SECONDS_PER_MINUTE = 60.0;
//   const long DISPLAY_TIMEOUT_INTERVAL = 120 * MILLIS_PER_SECOND;
//   const long DISPLAY_DIM_INTERVAL = DISPLAY_TIMEOUT_INTERVAL/2;
//   const long DISPLAY_UPDATE_INTERVAL = 250;
//   const int  DISPLAY_AVERAGE_INTERVALS = 4;
  
//   volatile unsigned long revolutions;
  
//   unsigned long previous_revolutions = 0;
//   unsigned long revolution_count[DISPLAY_AVERAGE_INTERVALS]; 
//   unsigned long interval_millis[DISPLAY_AVERAGE_INTERVALS]; 
//   unsigned int interval_index = 0;
//   unsigned long previous_millis = 0;
//   unsigned long last_sensor_time = 0;
//   bool is_oled_display_on = false;
//   bool is_oled_display_dim = false;
// }

// void setup() {
//   Serial.begin(9600);
//   initOledDisplayWithI2CAddress(0x3C);
//   display.setTextColor(WHITE);
//   initArrays();
	
//   turnOnIrLED();
//   attachPhotodiodeToInterrruptZero();
//   last_sensor_time = millis();
//   turnOnDisplay();
// }

// void initArrays() {
//   memset(revolution_count,0,sizeof(revolution_count));
//   memset(interval_millis,0,sizeof(interval_millis));
// }

// void loop() {
//   unsigned long current_millis = millis();
//   if (current_millis - last_sensor_time >= DISPLAY_TIMEOUT_INTERVAL) {
//     turnOffDisplay();
//   } else if (current_millis - last_sensor_time >= DISPLAY_DIM_INTERVAL) {
//     dimDisplay();
//   }  
  
//   if (current_millis - previous_millis >= DISPLAY_UPDATE_INTERVAL) {
//     previous_millis = current_millis;
//     updateDisplay();
// 	}
// }

// void turnOnIrLED() {
//   pinMode(IR_LED_PIN_3, OUTPUT);
//   digitalWrite(IR_LED_PIN_3, HIGH);
// }

// void attachPhotodiodeToInterrruptZero() {
//   pinMode(PHOTODIODE_PIN_2, INPUT_PULLUP);
//   attachInterrupt(INTERRUPT_ZERO_ON_PIN_2, incrementRevolution, FALLING);
// }

// void incrementRevolution() {
//   revolutions++;
// }

// long calculateRpm() {
//   unsigned long current_millis = millis();
//   unsigned long current_revolutions = revolutions;
//   unsigned long previous_display_millis;
//   unsigned long previous_revolutions;
    
//   queueIntervalRevolution(current_revolutions, current_millis);
//   previous_display_millis = getIntervalMillis();
//   previous_revolutions = getIntervalRevolutions();

//   unsigned long elapsed_millis =  current_millis - previous_display_millis;
//   float elapsed_seconds = ((elapsed_millis * 1.0) / MILLIS_PER_SECOND);
//   float delta_revolutions = (current_revolutions - previous_revolutions) * 1.0;

//   long rpm = (long) ((delta_revolutions / elapsed_seconds) * SECONDS_PER_MINUTE);
//   return rpm;
// }

// void queueIntervalRevolution(unsigned long revolution_value, unsigned long milliseconds) {
//   interval_index++;
//   int queue_index = (int)(interval_index % DISPLAY_AVERAGE_INTERVALS);
//   revolution_count[queue_index] = revolution_value; 
//   interval_millis[queue_index] = milliseconds;
// }

// unsigned long getIntervalMillis() {
//   int index_front_of_queue = (int)((interval_index + 1)  % DISPLAY_AVERAGE_INTERVALS);
//   return interval_millis[index_front_of_queue];
// }

// unsigned long getIntervalRevolutions() {
//   int index_front_of_queue = (int)((interval_index + 1)  % DISPLAY_AVERAGE_INTERVALS);
//   return revolution_count[index_front_of_queue];
// }
