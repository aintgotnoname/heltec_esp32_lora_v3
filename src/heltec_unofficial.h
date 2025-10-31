/**
 * @file heltec_unofficial.h
 * @brief Header file for the Heltec library.
 *
 * This file contains the definitions and declarations for the Heltec library.
 * The library provides functions for controlling the Heltec ESP32 LoRa V3
 * board, including LED brightness control, voltage measurement, deep sleep
 * mode, and more.
 */

#ifndef heltec_unofficial_h
#define heltec_unofficial_h

// 'PRG' Button
#ifndef BUTTON
#define BUTTON    GPIO_NUM_0
#endif
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

#ifdef HELTEC_WIRELESS_STICK_LITE
  #define HELTEC_NO_DISPLAY
#endif

#ifdef HELTEC_NO_RADIOLIB
  #define HELTEC_NO_RADIO_INSTANCE
#else
  #include "RadioLib.h"
  // make sure the power off button works when using RADIOLIB_OR_HALT
  // (See RadioLib_convenience.h)
  #define RADIOLIB_DO_DURING_HALT heltec_delay(10)
  #include "RadioLib_convenience.h"
#endif

#ifdef HELTEC_NO_DISPLAY
  #define HELTEC_NO_DISPLAY_INSTANCE
#else
  #include "SSD1306Wire.h"
  #include "OLEDDisplayUi.h"
#endif

#include "HotButton.h"

#ifndef HELTEC_NO_RADIO_INSTANCE
  #ifndef ARDUINO_heltec_wifi_32_lora_V3
    // Assume MISO and MOSI being wrong when not using Heltec's board definition
    // and use hspi to make it work anyway. See heltec_setup() for the actual SPI setup.
    #include <SPI.h>
    extern SPIClass* hspi;
    extern SX1262 radio;
  #else
    // Default SPI on pins from pins_arduino.h
    extern SX1262 radio;
  #endif
#endif

#ifndef HELTEC_NO_DISPLAY_INSTANCE
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

  #ifdef ARDUINO_heltec_wireless_stick_v3
    #define DISPLAY_GEOMETRY GEOMETRY_64_32
  #else
    #define DISPLAY_GEOMETRY GEOMETRY_128_64
  #endif
  extern SSD1306Wire display;
  extern PrintSplitter both;
#else
  Print &both = Serial;
#endif

extern HotButton button;

void heltec_led(int percent);
void heltec_ve(bool state);
float heltec_vbat();
void heltec_deep_sleep_prepare();
void heltec_high_res_deep_sleep(int64_t time_in_microseconds);
void heltec_deep_sleep(int seconds) ;
void heltec_deep_sleep(int seconds = 0);
int heltec_battery_percent(float vbat = -1);
bool heltec_wakeup_was_button();
bool heltec_wakeup_was_timer();
float heltec_temperature();
void heltec_display_power(bool on);
void heltec_setup();
void heltec_loop();
void heltec_delay(int ms);

#endif  // heltec_unofficial_h
