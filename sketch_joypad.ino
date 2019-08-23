#define USE_LovyanLauncher              // When commented out, "LovyanLauncher" is not used.

#include <M5Stack.h>
#ifdef USE_LovyanLauncher
#include <M5StackUpdater.h>
#endif

double old_time, next_game_tick, current_time;

#define KEYBOARD_I2C_ADDR     0x08
#define KEYBOARD_INT          5

void checkI2cKeyboard();

// the setup routine runs once when M5Stack starts up
void setup() {
  // Initialize the M5Stack object
  M5.begin();
  Wire.begin();
#ifdef USE_LovyanLauncher
  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println(F("Will Load menu binary"));
    updateFromFS(SD);
    ESP.restart();
  }
#endif
  
  M5.Lcd.setTextFont(2);
  
  // LCD display
  M5.Lcd.print(F("Hello World!\n"));
  M5.Lcd.print(F("M5Stack is running successfully!\n"));

  next_game_tick = current_time = millis();
  pinMode(KEYBOARD_INT, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:

  current_time = millis();
  if (current_time >= next_game_tick) {
    old_time = current_time;
    next_game_tick = current_time + 16.66666667;

    LogicUpdate();
    Draw();

    M5.update();
  }
}

// Logic Update
void LogicUpdate()
{
  // Keyboard
    if(digitalRead(KEYBOARD_INT) == LOW) {
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  // request 1 byte from keyboard
    while (Wire.available()) { 
      uint8_t key_val = Wire.read();                  // receive a byte as character
      if(key_val != 0) {
        if(key_val >= 0x20 && key_val < 0x7F) { // ASCII String
          Serial.print(F((char)key_val));
          M5.Lcd.print(F((char)key_val));
        } else {
          Serial.printf("0x%02X ",key_val);
          M5.Lcd.printf("0x%02X ",key_val);
        }
      }
    }
  }

  // A=0xEF(0x10)
  // B=0xDF(0x20)
  // UP=0xFE(0xs01)
  // DOWN=0xFD(0x02)
  // LEFT=0xFB(0x04)
  // RIGHT=0xF7(0x08)
  // SELECT=0xBF(0x40)
  // START=0x7F(0x80)
  // KEYOFF=0xFF

}

// Draw
void Draw()
{
  // BtnA
   if (M5.BtnA.wasPressed()) {
    M5.Lcd.print(F("BtnA "));
   } 
   // BtnB
   if (M5.BtnB.wasPressed()) {
    M5.Lcd.print(F("BtnB "));
   } 
   // BtnC
   if (M5.BtnC.wasPressed()) {
    M5.Lcd.print(F("BtnC "));
   } 
}
