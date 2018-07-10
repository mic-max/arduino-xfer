#include <LiquidCrystal.h>

#define RX_CLOCK 2
#define RX_DATA 3

#define LCD_D4 6
#define LCD_D5 7
#define LCD_D6 8
#define LCD_D7 9
#define LCD_RS 4
#define LCD_EN 5

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

char message[16];
volatile byte rx_byte;
volatile int bit_position;
volatile bool update_lcd;

void setup() {
  pinMode(RX_DATA, INPUT);
  lcd.begin(16, 2);
  set_values();
  attachInterrupt(digitalPinToInterrupt(RX_CLOCK), onClockPulse, RISING);
}

void set_values() {
  strcpy(message, "");
  rx_byte = 0;
  bit_position = 0;
  update_lcd = true;
}

void onClockPulse() {
  bool rx_bit = digitalRead(RX_DATA);

  if (bit_position == 8) {
    rx_byte = 0;
    bit_position = 0;
  }

  if (rx_bit) {
    rx_byte |= (0x80 >> bit_position);
  }

  bit_position++;

  if (bit_position == 8) {
    strncat(message, (char*) &rx_byte, 1);
    if (strlen(message) >= 16)
      strcpy(message, message + 1);
  }
  update_lcd = true;
}

void loop() {
  if (!update_lcd)
    return;
  update_lcd = false;
  // Add static last_time to reset values when no data received in 5s ?
  lcd.noCursor();
  lcd.setCursor(0, 0);
  lcd.print(message);
  lcd.setCursor(0, 1);
  for (int i = 0; i < 8; i++) {
    if (i < bit_position)
      lcd.print(rx_byte & (0x80 >> i) ? 1 : 0);
    else
      lcd.print(" ");
  }
  lcd.setCursor(strlen(message), 0);
  lcd.cursor();
}
