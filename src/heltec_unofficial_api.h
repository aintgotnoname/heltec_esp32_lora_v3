/**
 * @file heltec_api.h
 * @brief Header file for the Heltec library.
 *
 * This file contains the definitions and declarations for the Heltec library.
 * The library provides functions for controlling the Heltec ESP32 LoRa V3
 * board, including LED brightness control, voltage measurement, deep sleep
 * mode, and more.
 */

#ifndef heltec_api_h
#define heltec_api_h

// 'PRG' Button
#define BUTTON    GPIO_NUM_0
// LED pin & PWM parameters
#define LED_PIN   GPIO_NUM_35
#define LED_FREQ  5000
#define LED_CHAN  0
#define LED_RES   8
// External power control
#define VEXT      GPIO_NUM_36
// Battery voltage measurement
#define VBAT_CTRL GPIO_NUM_37
#define VBAT_ADC  GPIO_NUM_1
// SPI pins
//#define SS        GPIO_NUM_8
//#define MOSI      GPIO_NUM_10
//#define MISO      GPIO_NUM_11
//#define SCK       GPIO_NUM_9
// Radio pins
#define DIO1      GPIO_NUM_14
//#define RST_LoRa  GPIO_NUM_12
//#define BUSY_LoRa GPIO_NUM_13
// Display pins
//#define SDA_OLED  GPIO_NUM_17
//#define SCL_OLED  GPIO_NUM_18
//#define RST_OLED  GPIO_NUM_21

#ifndef HELTEC_NO_RADIOLIB
  #include "RadioLib.h"
  // make sure the power off button works when using RADIOLIB_OR_HALT
  // (See RadioLib_convenience.h)
  #define RADIOLIB_DO_DURING_HALT heltec_delay(10)
  #include "RadioLib_convenience.h"
  String radiolib_result_string(const int16_t result);
#endif

#ifdef HELTEC_NO_DISPLAY
  #define HELTEC_NO_DISPLAY_INSTANCE
#else
  #include "SSD1306Wire.h"
// removed  #include "OLEDDisplayUi.h"
#endif

//#include "HotButton.h"

#ifndef HELTEC_NO_RADIO_INSTANCE
  #ifndef HELTEC_NO_RADIOLIB
    extern SX1262 radio;
  #endif
#endif

// Don't you just hate it when battery percentages are wrong?
//
// I measured the actual voltage drop on a LiPo battery and these are the
// average voltages, expressed in 1/256'th steps between min_voltage and
// max_voltage for each 1/100 of the time it took to discharge the battery. The
// code for a telnet server that outputs battery voltage as CSV data is in
// examples, and a python script that outputs the constants below is in
// src/tools.
extern const float min_voltage;
extern const float max_voltage;
extern const uint8_t scaled_voltage[100];

/**
 * @class PrintSplitter
 * @brief A class that splits the output of the Print class to two different
 *        Print objects.
 *
 * The PrintSplitter class is used to split the output of the Print class to two
 * different Print objects. It overrides the write() function to write the data
 * to both Print objects.
 */
class PrintSplitter : public Print {
  public:
    PrintSplitter(Print &_a, Print &_b) : a(_a), b(_b) {}
    size_t write(uint8_t c) {
      a.write(c);
      return b.write(c);
    }
    size_t write(const char* str) {
      a.write(str);
      return b.write(str);
    }
  private:
    Print &a;
    Print &b;
};


  extern SSD1306Wire display;
  // removed extern OLEDDisplayUi ui;
  extern PrintSplitter both;


  // extern HotButton button;

/**
 * @brief Controls the LED brightness based on the given percentage.
 *
 * This function sets up the LED channel, frequency, and resolution, and then
 * adjusts the LED brightness based on the given percentage. If the percentage
 * is 0 or less, the LED pin is set as an input pin.
 *
 * @param percent The brightness percentage of the LED (0-100).
 */
void heltec_led(int percent);

/**
 * @brief Controls the VEXT pin to enable or disable external power.
 *
 * This function sets the VEXT pin as an output pin and sets its state based on
 * the given parameter. If the state is true, the VEXT pin is set to LOW to
 * enable external power. If the state is false, the VEXT pin is set to INPUT to
 * disable external power.
 *
 * @param state The state of the VEXT pin (true = enable, false = disable).
 */
void heltec_ve(bool state);

/**
 * @brief Measures the battery voltage.
 *
 * This function measures the battery voltage by controlling the VBAT_CTRL pin
 * and reading the analog value from the VBAT_ADC pin. The measured voltage is
 * then converted to a float value and returned.
 *
 * @return The battery voltage in volts.
 */
float heltec_vbat();

/**
 * @brief Prepares the device into deep sleep mode.
 *
 * This function prepares the device for deep sleep mode by disconnecting from
 * WiFi, turning off the display, disabling external power, and turning off the
 * LED. 
 */
void heltec_deep_sleep_prepare();

/**
 * @brief Puts the device into deep sleep mode.
 *
 * This function puts the device for deep sleep mode. It can also be configured 
 * to wake up after a certain number of microseconds using the optional parameter.
 *
 * @param seconds The number of micro seconds to sleep before waking up (default = 0).
 */
void heltec_high_res_deep_sleep(int64_t microseconds = 0);

/**
 * @brief Puts the device into deep sleep mode.
 *
 * This function prepares the device for deep sleep mode by disconnecting from
 * WiFi, turning off the display, disabling external power, and turning off the
 * LED. It can also be configured to wake up after a certain number of seconds
 * using the optional parameter.
 *
 * @param seconds The number of seconds to sleep before waking up (default = 0).
 */
void heltec_deep_sleep(int seconds = 0);

/**
 * @brief Calculates the battery percentage based on the measured battery
 * voltage.
 *
 * This function calculates the battery percentage based on the measured battery
 * voltage. If the battery voltage is not provided as a parameter, it will be
 * measured using the heltec_vbat() function. The battery percentage is then
 * returned as an integer value.
 *
 * @param vbat The battery voltage in volts (default = -1).
 * @return The battery percentage (0-100).
 */
int heltec_battery_percent(float vbat = -1);

/**
 * @brief Checks if the device woke up from deep sleep due to button press.
 * 
 * @return True if the wake-up cause is a button press, false otherwise.
 */
bool heltec_wakeup_was_button();

/**
 * @brief Checks if the device woke up from deep sleep due to a timer.
 * 
 * This function checks if the device woke up from deep sleep due to a timer.
 * 
 * @return True if the wake-up cause is a timer interrupt, false otherwise.
 */
bool heltec_wakeup_was_timer();

/**
 * @brief Measures esp32 chip temperature
 * 
 * @return float with temperature in degrees celsius.
*/
float heltec_temperature();

/**
 * @brief Initializes the Heltec library.
 *
 * This function should be the first thing in setup() of your sketch. It
 * initializes the Heltec library by setting up serial port and display.
 */
void heltec_setup();

/**
 * @brief The main loop function for the Heltec library.
 *
 * This function should be called in loop() of the Arduino sketch. It updates
 * the state of the power button and implements long-press power off if used.
 */
void heltec_loop();

/**
 * @brief Delays the execution of the program for the specified number of
 *        milliseconds.
 *
 * This function delays the execution of the program for the specified number of
 * milliseconds. During the delay, it also calls the heltec_loop() function to
 * allow for the power off button to be checked.
 *
 * @param ms The number of milliseconds to delay.
 */
void heltec_delay(int ms);

#endif  // heltec_api_h
