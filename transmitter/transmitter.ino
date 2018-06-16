#include <LiquidCrystal.h>

// Transmit rate in bps
#define TX_RATE 125

// Pin assignments
#define TX_CLOCK 2
#define TX_DATA 3
#define LCD_D4 6
#define LCD_D5 7
#define LCD_D6 8
#define LCD_D7 9
#define LCD_RS 4
#define LCD_EN 5

const char *message = "Absolute Unit!!!";
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
  // put your setup code here, to run once:
  pinMode(TX_DATA, OUTPUT);
  pinMode(TX_CLOCK, OUTPUT);

  // Initialize the LCD screen

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(message);

  for(int byte_idx = 0; byte_idx < strlen(message); byte_idx++) {
    char tx_byte = message[byte_idx];

    lcd.noCursor();
    lcd.setCursor(0, 1);
    lcd.print("        ");
    lcd.setCursor(byte_idx, 0);
    lcd.cursor();

    for(int bit_idx = 0; bit_idx < 8; bit_idx++) {
      bool tx_bit = tx_byte & (0x80 >> bit_idx);

      digitalWrite(TX_DATA, tx_bit);
      delay(1000 / TX_RATE / 2);

      // Update the LCD
      lcd.noCursor();
      lcd.setCursor(bit_idx, 1);
      lcd.print(tx_bit ? "1" : "0");
      lcd.setCursor(byte_idx, 0);
      lcd.cursor();

      digitalWrite(TX_CLOCK, HIGH);
      delay(1000 / TX_RATE / 2);
      digitalWrite(TX_CLOCK, LOW);
    }
  }
  digitalWrite(TX_DATA, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
}
