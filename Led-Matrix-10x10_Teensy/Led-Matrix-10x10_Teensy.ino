/*
   A Software to control a 10x10 RGB LED Matrix with 4 Bit BAM (Brightness).
   Can be controlled via ArtNet with a software like jinx.
   @author Tom Stein & Ferenc Stockbrink
   @version 0.3.0
   @date 13.09.2018
*/
#include "TeensyDMX.h"
// Debug
#define DEBUG 0
#define DEBUGVALUES 0
/*
   =====Classes=====
*/
class LED {
  public:
    uint16_t rgb;
    LED(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0) {
      setRGB(_r, _g, _b);
    }
    void setRGB(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0) {
      _r = _r >> 4; // Shift 8 bit down to 4 bit
      _g = _g >> 4; // Shift 8 bit down to 4 bit 
      _b = _b >> 4; // Shift 8 bit down to 4 bit 
      rgb = (_r << 8) | (_g << 4) | _b;
    }
    bool getBit(uint16_t mask) {
      return rgb & mask;
    }
};
/*
   =====Fields=====
*/
constexpr uint8_t COLS = 10;
constexpr uint8_t ROWS = COLS;
#define latchPin 2
#define latchPinBitBang 1
#define dataPin 3
#define dataPinBitBang 1 << 12
#define clockPin 4
#define clockPinBitBang 1 << 13
#define speedPinAni A9
#define speedPinMultiplex A8
LED leds[100];
unsigned long lngMsLoopstart;
unsigned long lngWhileUptime = 42000;
// DMX Data
uint8_t data[ROWS * COLS * 3];
// DMX Connection
namespace teensydmx =::qindesign::teensydmx;
teensydmx::Receiver dmxRxA
{
  Serial1
};

teensydmx::Receiver dmxRxB
{
  Serial2
};

/*
   =====Methodes=====
*/
void setup() {
  //set pins to output so you can control the shift register
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(speedPinAni, INPUT);
  pinMode(speedPinMultiplex, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(dataPin, LOW);
  digitalWrite(latchPin, LOW);
  digitalWrite(clockPin, LOW);
  digitalWrite(13, HIGH);
  dmxRxA.begin();
  dmxRxB.begin();
  Serial.begin(250000);
}

void BAM() {
  uint8_t timeMicros = 0;
  uint16_t bitmask_r = 0;
  uint16_t bitmask_g = 0;
  uint16_t bitmask_b = 0;
  long startMicros = micros();
  for (uint8_t mag = 1; mag < 16; mag++) {
    for (byte row = 0; row < ROWS; ++row) {
      
      if ((mag & (mag - 1)) == 0) { // Is it power of two? Change bitmask
        bitmask_r = mag;
        bitmask_g = bitmask_r << 4;
        bitmask_b = bitmask_g << 4;
      }
      GPIOA_PCOR = (dataPinBitBang);  // Datapin low
      GPIOA_PCOR = (clockPinBitBang); // Clockpin low
      GPIOD_PSOR = (latchPinBitBang); // latch low
      for (int8_t cnt = ROWS - 1; cnt >= 0; --cnt) { // Shiftout the layer 
        shift1bit(cnt == row);
      }
      for (int8_t col = COLS - 1; col >= 0; --col) { // Shiftout red
        shift1bit(leds[row * ROWS + col].getBit(bitmask_r));
      }
      for (int8_t col = COLS - 1; col >= 0; --col) { // Shiftout green
        shift1bit(leds[row * ROWS + col].getBit(bitmask_g));
      }
      for (int8_t col = COLS - 1; col >= 0; --col) { // Shiftout blue
        shift1bit(leds[row * ROWS + col].getBit(bitmask_b));
      }
      
      GPIOA_PCOR = (dataPinBitBang);  // Datapin low
      GPIOA_PCOR = (clockPinBitBang); // Clockpin low
      GPIOD_PCOR = (latchPinBitBang); // latch high
      //delayMicroseconds(100);
      if(analogRead(speedPinMultiplex) > 5) {
        delayMicroseconds(analogRead(speedPinMultiplex) * 100);
      }
    }
  }
  //timeMicros /= 15;
  Serial.println("########");
  Serial.print("BAM: ");
  Serial.println(micros() - startMicros);
  Serial.println("########");
}

void shift1bit (bool b) {
  if (b) {
    GPIOA_PSOR = (dataPinBitBang); // Datapin high
  } else {
    GPIOA_PCOR = (dataPinBitBang); // Datapin low
  }

  // clock pulse
  GPIOA_PSOR = (clockPinBitBang); // Clockpin high
  __asm__ __volatile__ ("nop\n\t"); // sleep one clock cycle 
  __asm__ __volatile__ ("nop\n\t"); // sleep one clock cycle 
  GPIOA_PCOR = (clockPinBitBang); // Clockpin low
}

int getDMX()
{
  uint8_t tempA[300];
  int readA = dmxRxA.readPacket(tempA, 1, 300);
  if (readA == 300)
  {
    #if DEBUG
    Serial.printf("DMX.A: %d\n", readA);
    #endif

    #if DEBUGVALUES
    for (int row = 0; row < ROWS; ++row)
    {
      for (int column = 0; column < COLS; ++column)
      {
        Serial.printf("[%d %d %d] ", data[row * COLS * 3 + column * 3 + 0], data[row * COLS * 3 + column * 3 + 1], data[row * COLS * 3 + column * 3 + 2]);
      }
      Serial.println("");
    }
    #endif

    memcpy(data, tempA, sizeof(tempA));
    return 1;
  }
  else if (readA == -1)
  {
    #if DEBUG
      Serial.println("There is no DMX package available for port A");
    #endif

    return 0;
  }
  else
  {
    Serial.printf("%s: %d\n", "Error reading all 300 required DMX Channels from DMX.A. Read: ", readA);
    return -1;
  }
}

void loop() { 
    if(getDMX() == 1) { // Check if new DMX values were recieved
        for (int8_t i = 0; i < 100; i++) { // update current data with the new values
            int d = i * 3;
            leds[i].setRGB(data[d], data[d+1], data[d+2]);
        }
    }
    BAM(); // Perfrom one muliplexing cycle with bit angle modulation
} 
