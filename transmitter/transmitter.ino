#include <LiquidCrystal.h>
#include <Wire.h>

// Transmit rate in bps
#define TX_RATE 24
#define DELAY 500 / TX_RATE

#define TX_CLOCK 2
#define TX_DATA 3

#define LCD_D4 6
#define LCD_D5 7
#define LCD_D6 8
#define LCD_D7 9
#define LCD_RS 4
#define LCD_EN 5

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
  pinMode(TX_DATA, OUTPUT);
  pinMode(TX_CLOCK, OUTPUT);

  // Initialize the LCD screen
  lcd.begin(16, 2);
  sendASCII("Hi, I'm Michael. How are you?");
  uint16_t ham = hamming(65);
}


// receives a message with 11 least-significant bits containing the data
uint16_t hamming(uint16_t message) {
  // BIT # 0 1 2 3 4 5 6 7 8 9 a b c d e f
  // DATA  1 0 0 0 0 0 1 1 0 1 1
  // HAMM  x x x x x x x x x x x p p p p P
  uint16_t result = 0;

  return result;
}

void loop() {}

void sendASCII(const char* message) {

  for (int i = 0; i < strlen(message); i++) {
    lcd.setCursor(0, 0);
    lcd.print(message[i]);
    byte tx_byte = message[i];

    lcd.noCursor();
    lcd.setCursor(0, 1);
    lcd.print("        ");
    lcd.setCursor(i, 0);
    lcd.cursor();

    for (int j = 0; j < 8; j++) {
      bool tx_bit = tx_byte & (0x80 >> j);

      digitalWrite(TX_DATA, tx_bit);
      delay(DELAY);

      // Update LCD
      lcd.noCursor();
      lcd.setCursor(j, 1);
      lcd.print(tx_bit, BIN);
      lcd.setCursor(i, 0); // TODO lcd autoscroll, print char, move cursor
      lcd.cursor();

      digitalWrite(TX_CLOCK, HIGH);
      delay(DELAY);
      digitalWrite(TX_CLOCK, LOW);
    }
  }
  digitalWrite(TX_DATA, LOW);
}
