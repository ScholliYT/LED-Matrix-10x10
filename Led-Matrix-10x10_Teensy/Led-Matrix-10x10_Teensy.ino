/*
   A Software to control a 10x10 RGB LED Matrix with 4 Bit BAM (Brightness).
   @author Tom Stein & Ferenc Stockbrink
   @version 0.2.2
   @date 16.march.2017
*/
/*
   =====Classes=====
*/
class LED {
  public:
    uint32_t rgb;
    LED(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0) {
      setRGB(_r, _g, _b);
    }
    void setRGB(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0) {
      rgb = (_r << 16) | (_g << 8) | _b;
    }
    bool getBit(uint32_t mask) {
      return rgb & mask;
    }
};
/*
   =====Methods=====
*/
constexpr uint8_t COLS = 10;
constexpr uint8_t ROWS = COLS;
#define latchPin 2
#define dataPin 3
#define clockPin 4
#define speedPinAni A9
#define speedPinMultiplex A8
LED leds[100];
unsigned long lngMsLoopstart;
unsigned long lngWhileUptime = 42000;

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
  Serial.begin(256000);
}

void BAM() {
  uint8_t timeMicros = 0;
  uint32_t bitmask_r = 0;
  uint32_t bitmask_g = 0;
  uint32_t bitmask_b = 0;
  for (uint8_t mag = 1; mag < 255; mag++) {
    long startMicros = micros();
    for (byte row = 0; row < ROWS; ++row) {
      
      if ((mag & (mag - 1)) == 0) { // Is it power of two? Change bitmask
        bitmask_r = mag;
        bitmask_g = bitmask_r << 8;
        bitmask_b = bitmask_g << 8;
      }
      GPIOA_PCOR = (1 << 12); // Datapin low
      GPIOA_PCOR = (1 << 13); // Clockpin low
      GPIOD_PSOR = (1);       // latch low
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
      
      GPIOA_PCOR = (1 << 12); // Datapin low
      GPIOA_PCOR = (1 << 13); // Clockpin low
      GPIOD_PCOR = (1);       // latch high
      delayMicroseconds(analogRead(speedPinMultiplex) * 100);
    }
    timeMicros += micros() - startMicros;
  }
  //timeMicros /= 15;
  Serial.println("########");
  Serial.print("BAM: ");
  Serial.println(timeMicros);
  Serial.println("########");
}

void shift1bit (bool b) {
  if (b) {
    GPIOA_PSOR = (1 << 12); // Datapin high
  } else {
    GPIOA_PCOR = (1 << 12); // Datapin low
  }

  // clock pulse
  GPIOA_PSOR = (1 << 13); // Clockpin high
  __asm__ __volatile__ ("nop\n\t"); // sleep one clock cycle
  __asm__ __volatile__ ("nop\n\t"); // sleep one clock cycle
  GPIOA_PCOR = (1 << 13); // Clockpin low
}

void loop() {
  //1
  lngMsLoopstart = micros();
  lngWhileUptime = analogRead(speedPinAni) * 100; //lngWhileUptime = 42000;
  while (micros() < lngMsLoopstart + lngWhileUptime) {
    leds[0].setRGB(0, 0, 0);
    leds[1].setRGB(0, 0, 0);
    leds[2].setRGB(0, 0, 0);
    leds[3].setRGB(0, 0, 0);
    leds[4].setRGB(0, 0, 0);
    leds[5].setRGB(0, 0, 0);
    leds[6].setRGB(0, 0, 0);
    leds[7].setRGB(0, 0, 0);
    leds[8].setRGB(0, 0, 0);
    leds[9].setRGB(0, 0, 0);
    leds[10].setRGB(0, 0, 0);
    leds[11].setRGB(0, 0, 0);
    leds[12].setRGB(0, 0, 0);
    leds[13].setRGB(0, 0, 0);
    leds[14].setRGB(0, 0, 0);
    leds[15].setRGB(0, 0, 0);
    leds[16].setRGB(0, 0, 0);
    leds[17].setRGB(0, 0, 0);
    leds[18].setRGB(0, 0, 0);
    leds[19].setRGB(0, 0, 0);
    leds[20].setRGB(0, 0, 0);
    leds[21].setRGB(0, 0, 0);
    leds[22].setRGB(0, 0, 0);
    leds[23].setRGB(0, 0, 0);
    leds[24].setRGB(0, 0, 0);
    leds[25].setRGB(0, 0, 0);
    leds[26].setRGB(0, 0, 0);
    leds[27].setRGB(0, 0, 0);
    leds[28].setRGB(0, 0, 0);
    leds[29].setRGB(0, 0, 0);
    leds[30].setRGB(0, 0, 0);
    leds[31].setRGB(0, 0, 0);
    leds[32].setRGB(0, 0, 0);
    leds[33].setRGB(0, 0, 0);
    leds[34].setRGB(0, 0, 0);
    leds[35].setRGB(0, 0, 0);
    leds[36].setRGB(0, 0, 0);
    leds[37].setRGB(0, 0, 0);
    leds[38].setRGB(0, 0, 0);
    leds[39].setRGB(0, 0, 0);
    leds[40].setRGB(0, 0, 0);
    leds[41].setRGB(0, 0, 0);
    leds[42].setRGB(0, 0, 0);
    leds[43].setRGB(0, 0, 0);
    leds[44].setRGB(255, 255, 255);
    leds[45].setRGB(200, 200, 200);
    leds[46].setRGB(0, 0, 0);
    leds[47].setRGB(0, 0, 0);
    leds[48].setRGB(0, 0, 0);
    leds[49].setRGB(0, 0, 0);
    leds[50].setRGB(0, 0, 0);
    leds[51].setRGB(0, 0, 0);
    leds[52].setRGB(0, 0, 0);
    leds[53].setRGB(0, 0, 0);
    leds[54].setRGB(100, 100, 100);
    leds[55].setRGB(50, 50, 50);
    leds[56].setRGB(20, 20, 20);
    leds[57].setRGB(10, 10, 10);
    leds[58].setRGB(1, 1, 1);
    leds[59].setRGB(0, 0, 0);
    leds[60].setRGB(0, 0, 0);
    leds[61].setRGB(0, 0, 0);
    leds[62].setRGB(0, 0, 0);
    leds[63].setRGB(0, 0, 0);
    leds[64].setRGB(0, 0, 0);
    leds[65].setRGB(0, 0, 0);
    leds[66].setRGB(0, 0, 0);
    leds[67].setRGB(0, 0, 0);
    leds[68].setRGB(0, 0, 0);
    leds[69].setRGB(0, 0, 0);
    leds[70].setRGB(0, 0, 0);
    leds[71].setRGB(0, 0, 0);
    leds[72].setRGB(0, 0, 0);
    leds[73].setRGB(0, 0, 0);
    leds[74].setRGB(0, 0, 0);
    leds[75].setRGB(0, 0, 0);
    leds[76].setRGB(0, 0, 0);
    leds[77].setRGB(0, 0, 0);
    leds[78].setRGB(0, 0, 0);
    leds[79].setRGB(0, 0, 0);
    leds[80].setRGB(0, 0, 0);
    leds[81].setRGB(0, 0, 0);
    leds[82].setRGB(0, 0, 0);
    leds[83].setRGB(0, 0, 0);
    leds[84].setRGB(0, 0, 0);
    leds[85].setRGB(0, 0, 0);
    leds[86].setRGB(0, 0, 0);
    leds[87].setRGB(0, 0, 0);
    leds[88].setRGB(0, 0, 0);
    leds[89].setRGB(0, 0, 0);
    leds[90].setRGB(0, 0, 0);
    leds[91].setRGB(0, 0, 0);
    leds[92].setRGB(0, 0, 0);
    leds[93].setRGB(0, 0, 0);
    leds[94].setRGB(0, 0, 0);
    leds[95].setRGB(0, 0, 0);
    leds[96].setRGB(0, 0, 0);
    leds[97].setRGB(0, 0, 0);
    leds[98].setRGB(0, 0, 0);
    leds[99].setRGB(0, 0, 0);
    BAM();
  }
}


