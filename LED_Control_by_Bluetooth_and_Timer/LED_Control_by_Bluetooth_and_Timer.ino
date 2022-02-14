/*In this program the Timer will be ON and OFF as per the singal comming from the mobile via Bluetooth device HC-05.
When the Timer on it will start counting the second in i variable. When Stop will come from Mobile the Timer will be
STOP and Time will be reset to 0.

connection detals
TX of Bluetooth - Pin 2 of Arduino
RX of Bluetooth - Pin 3 of Arduino
VCC - 5V
GND-GND
LED - Pin 10 of Arduino

Date- February, 2022
Made by - Uttam Basu
Mobile Application- https://play.google.com/store/apps/details?id=com.csa.bluetoothremote&hl=en
For more example- https://github.com/UttamBasu
*/

#include<SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);          // RX, TX for Bluetooth

boolean toggle1 = 0;
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(10, OUTPUT);

  //For 1 Second Time Generate on Timer 1.
  cli();                                //stop interrupts
  TCCR1A = 0;                           // set entire TCCR1A register to 0
  TCCR1B = 0;                           // same for TCCR1B
  TCNT1  = 0;                           //initialize counter value to 0
  OCR1A = 15624;                        // = (16*10^6) / (1*1024) - 1 (must be <65536)
  TCCR1B |= (1 << WGM12);               // turn on CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Set CS12 and CS10 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);              // enable timer compare interrupt
  sei();                                //allow interrupts
}

int i = 0;
//Application of Timer Interrupt
ISR(TIMER1_COMPA_vect)                  //Timer1 interrupt 1Hz toggles pin 10 (LED)
{
  //Generates pulse wave of frequency 1Hz/2 = 0.5Hz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle1)
  {
    digitalWrite(10, HIGH);
    toggle1 = 0;
    i++;
    Serial.println(i);
  }
  else
  {
    digitalWrite(10, LOW);
    toggle1 = 1;
    i++;
    Serial.println(i);
  }
}

void loop()
{
  if (mySerial.available() > 0)
  {
    char c = mySerial.read();
    if (c == 'a')
    {
      digitalWrite(10, LOW);
      TCCR1B = 0;                             //stop timer
      TCCR1B |= (0 << WGM12);                 //for timer1
      i = 0;
    }
    else if (c == 'b')
    {
      TCCR1B = 1;                           //start timer
      TCCR1B |= (1 << WGM12);               //for timer1
      TCCR1B |= (1 << CS12) | (1 << CS10);  // Set CS12 and CS10 bits for 1024 prescaler
    }
    else
    {

    }
  }
}
