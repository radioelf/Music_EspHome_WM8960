#include "esphome.h"
#include "esphome/core/log.h"
#include <Wire.h>
// El WM8960 tiene registros de 9 bits.
// Para escribir en un registro, debemos hacer lo siguiente:
// Enviar 3 bytes
// Byte0 = dirección del dispositivo + bit de lectura/escritura
// Control_byte_1 = dirección del registro a escribir (7 bits) más el 9º bit de datos
// Control_byte_2 = los 8 bits restantes de datos del registro

// Address 0x1A  register 0x01  Config 0x17 (23)
//  00011010      0000001-0      00010111

#define I2C_SDA 4
#define I2C_SCL 5

const uint16_t I2C_ADDRESS = 0x1A;
const uint8_t register_bit[29] = {0, 2, 4,  5,  6,  7,  8, 10, 14, 17, 18, 51, 52, 53,  65, 67, 68, 69, 74, 75, 90, 92, 94, 104, 106, 108, 110, 1, 3};
const uint8_t value[29] =        {23,23,121,121,121,121,5, 0,  2,  196,0,  252,1,  227, 8,  8,  128,128,128,128,128,128,60, 55,  134, 194, 38,  23,23};

static const char *const TAG = "WM8960";

class Wm8960 : public Component, public BinarySensor{
 public:
  // constructor
    void setup() override {
        bool state = false; 

        Wire.begin(I2C_SDA, I2C_SCL, 100000);
        Wire.beginTransmission(I2C_ADDRESS);
        if (Wire.endTransmission() != 0) {
          ESP_LOGE(TAG, "not found at address 0x1A");
          publish_state(state);
        return;
       }
       state = true;      
       for (uint8_t x = 0; x < 29; x++) {
          Wire.beginTransmission(I2C_ADDRESS);       
          Wire.write(register_bit[x]);
          Wire.write(value[x]);
          Wire.endTransmission();
          if (x == 26) delay (10);
        } 

        ESP_LOGI(TAG, "Codec 1 Setup complete. Listen to left/right INPUT1 on Speaker outputs.");     
        publish_state(state);
  }
};
