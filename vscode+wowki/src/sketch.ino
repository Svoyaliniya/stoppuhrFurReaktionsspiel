/*
  Stoppuhr für Reaktionsspiel

  Dieser Code verwendet: 
    I2C-Display; 
    3 LEDs;
    1 Lautsprecher;
    2 Tasten. 

  Ein Beispiel für eine solche Verbindung ist hier zu sehen: https://wokwi.com/projects/428425594051544065

  Der Code verwendet eine Bibliothek für die Arbeit mit dem I2C-Display: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

  Datum: 16.04.2025
  Von: Bandurovskiy Stanislav
*/

// Anschluss und Konfiguration der Bibliothek
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LED_PIN 13
#define BUTTON_PIN_ON 12
#define BUTTON_PIN_RESET 11

#define LED_PIN_RED 7
#define LED_PIN_YLW 6
#define LED_PIN_GRN 5
#define SPEAKER_PIN 4

// Verwendete Variablen
int minutes_tens = 0;
int minutes_ones = 0;
int senonds_tens = 0;
int seconds_ones = 0;
int miliseconds_tens = 0;
int miliseconds_ones = 0;

bool timer_running = false;
bool last_button_state = HIGH;

void setup() {
  // Mit dem Display arbeiten und die aktuelle Uhrzeit anzeigen
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(String(minutes_tens) + String(minutes_ones) + "." + String(senonds_tens) + String(seconds_ones) + "." + String(miliseconds_tens) + String(miliseconds_ones));

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN_ON, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RESET, INPUT_PULLUP);

  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_YLW, OUTPUT);
  pinMode(LED_PIN_GRN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
}

// Funktion zum Zählen der Zeit (Stoppuhr)
void timerStart() {
  delay(10);
  if(timer_running) {
    digitalWrite(LED_PIN, HIGH);
    miliseconds_ones++;
    if(miliseconds_ones == 10) {
      miliseconds_ones = 0;
      miliseconds_tens++;
    }
    if(miliseconds_tens == 10) {
      miliseconds_tens = 0;
      seconds_ones++;
    }
    if(seconds_ones == 10) {
      seconds_ones = 0;
      senonds_tens++;
    }
    if(senonds_tens == 6) {
      senonds_tens = 0;
      minutes_ones++;
    }
    if(minutes_ones == 10) {
      minutes_ones = 0;
      minutes_tens++;
    }
    if(minutes_tens == 10) {
      minutes_tens = 0;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print(String(minutes_tens) + String(minutes_ones) + "." + String(senonds_tens) + String(seconds_ones) + "." + String(miliseconds_tens) + String(miliseconds_ones));
}

// Funktion zum Zurückstellen der Uhrzeit
void timerReset() {
  delay(50);
    timer_running = false;
    minutes_tens = 0;
    minutes_ones = 0;
    senonds_tens = 0;
    seconds_ones = 0;
    miliseconds_tens = 0;
    miliseconds_ones = 0;
}

// Funktion für das Warnzeichen
void timerAlert() {
  digitalWrite(LED_PIN_RED, HIGH);
  delay(500);
  digitalWrite(LED_PIN_RED, LOW);
  digitalWrite(LED_PIN_YLW, HIGH);
  delay(500);
  digitalWrite(LED_PIN_YLW, LOW);
  digitalWrite(LED_PIN_GRN, HIGH);
  delay(500);
  digitalWrite(LED_PIN_GRN, LOW);
  tone(SPEAKER_PIN, 262, 500);
}

void loop() {
  int button_state_on = digitalRead(BUTTON_PIN_ON);
  if (button_state_on == LOW && !timer_running && last_button_state == HIGH) { // Wenn Taste 1 gedrückt wird
    delay(50);
    timer_running = true;
    timerAlert();
  } else if (button_state_on == LOW && timer_running && last_button_state == HIGH) {
    delay(50);
    timer_running = false;
  }

  int button_state_reset = digitalRead(BUTTON_PIN_RESET);
  if (button_state_reset == LOW && last_button_state == HIGH) { // Wenn Taste 2 gedrückt wird
    timerReset();
  }

  last_button_state = button_state_on;
  timerStart();

  /*  
    Die Variablen delay und last_button_state werden zusätzlich im Code verwendet

    Damit sollen Fehlbedienungen vermieden werden.
  */
}