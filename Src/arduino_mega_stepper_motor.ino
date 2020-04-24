
#include "Wiegand.h"
#include "credetentials.h"


// ZDR

// ZDR 2

int Index;
int card = CARD_ID;     
int tags[10] = {0};    // static database
int pos = -1;           // position in the @tags array 
WIEGAND wg;            // instance of the rfid reader     
  
/**
 *  @brief State for the door if it is locked or not.
 *  false The door is unlocked.
 *  true  The door is locked.
 */
static bool state = false;

// Motor pins
#define step_pin 24
#define dir_pin 22
#define en_pin 28
#define ms1_pin 23
#define ms2_pin 25
#define ms3_pin 27

#define motor_steps 200
#define left 0
#define right 1

// Wiegand card reader data pins
#define D0_PIN 2
#define D1_PIN 3

#define BUTTON 26

void setup() 
{
  Serial.begin(9600);  

  // Init for the motor driver.
  pinMode(step_pin, OUTPUT);           // set pin to input
  digitalWrite(step_pin, LOW);       // turn on pullup resistors

  pinMode(dir_pin, OUTPUT);           // set pin to input
  digitalWrite(dir_pin, LOW);       // turn on pullup resistors

  pinMode(en_pin, OUTPUT);           // set pin to input
  digitalWrite(en_pin, HIGH);       // turn on pullup resistors

  pinMode(ms1_pin, OUTPUT);           // set pin to input
  digitalWrite(ms1_pin, LOW);       // turn on pullup resistors

  pinMode(ms2_pin, OUTPUT);           // set pin to input
  digitalWrite(ms2_pin, LOW);       // turn on pullup resistors

  pinMode(ms3_pin, OUTPUT);           // set pin to input
  digitalWrite(ms3_pin, LOW);       // turn on pullup resistors

  pinMode(BUTTON, INPUT_PULLUP);

  // Init for the card reader.
  wg.begin(D0_PIN, D1_PIN);

  if (digitalRead(BUTTON) == LOW) // if the button is pressed on startup
  {
    Serial.println("Button pressed. Scan a card to add it to the database.");

    while(1)
    {
      if(wg.available())
      {
        tags[++pos] = wg.getCode();
        Serial.println("Card scanned and added to the database:");
        Serial.println(tags[pos]);
        break;
       }
    }
  }

  //digitalWrite(RESET, HIGH);
  Serial.println("Program started.");
}

void loop() 
{
  int i;
  if(wg.available())
  {
    int c = wg.getCode();
    Serial.println("Card detected:");
    Serial.println(c);
    
    //Serial.println("Pos:");
    //Serial.println(pos);
    for (i = 0;i <= pos; i++)
       if (c == tags[i]) 
        {
          Serial.println("Access gained.");
          stepper_move (300, state);
        } 
  }
}

void stepper_move(int steps, int dir) //dir: left=0, right=1
{
  float full_p = 0.5;
  float half_p = 0.2;
  float quarter_p = 0.15;
  float eighth_p = 0.1;
  float sixteenth_p = 0.05;

  int full_s = full_p * steps;
  int half_s = half_p * steps * 2;
  int quarter_s = quarter_p * steps * 4;
  int eighth_s = eighth_p * steps * 8;
  int sixteenth_s = sixteenth_p * steps * 16;
  
  digitalWrite(en_pin, LOW); 
  digitalWrite(dir_pin, dir);
  delay(10);

  set_sixteenth_step();
  move(sixteenth_s/2);
  set_eighth_step();
  move(eighth_s/2);
  set_quarter_step();
  move(quarter_s/2);
  set_half_step();
  move(half_s/2);
  set_full_step();
  move(full_s);
  set_half_step();
  move(half_s/2);
  set_quarter_step();
  move(quarter_s/2);
  set_eighth_step();
  move(eighth_s/2);
  set_sixteenth_step();
  move(sixteenth_s/2);
  
  digitalWrite(en_pin, HIGH); 

  state = !state;
}

void move(int steps)
{
  int delay_ms = 2;
  for(int i = 0; i < steps; i++)
  {
    
    digitalWrite(step_pin, HIGH);
    digitalWrite(step_pin, LOW);
    
    delay(delay_ms);
    
  }
}

void set_full_step()
{
  digitalWrite(ms1_pin, LOW);
  digitalWrite(ms2_pin, LOW);
  digitalWrite(ms3_pin, LOW);
}

void set_half_step()
{
  digitalWrite(ms1_pin, HIGH);
  digitalWrite(ms2_pin, LOW);
  digitalWrite(ms3_pin, LOW);
}

void set_quarter_step()
{
  digitalWrite(ms1_pin, LOW);
  digitalWrite(ms2_pin, HIGH);
  digitalWrite(ms3_pin, LOW);
}

void set_eighth_step()
{
  digitalWrite(ms1_pin, HIGH);
  digitalWrite(ms2_pin, HIGH);
  digitalWrite(ms3_pin, LOW);
}

void set_sixteenth_step()
{
  digitalWrite(ms1_pin, HIGH);
  digitalWrite(ms2_pin, HIGH);
  digitalWrite(ms3_pin, HIGH);
}

