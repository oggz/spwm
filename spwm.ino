/* 
   SPWM Inverter Driver
   By: Alex Doccolo

   Output Ports: 9 and 10
*/


bool flag = false;
unsigned int count = 0;
unsigned int sinPWM[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
unsigned int sin_size = sizeof(sinPWM) / sizeof(int);
unsigned int period = 4000;
unsigned int a0;
char command;


void setup() 
{
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);

  // Set waveform 8
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(COM1C1);
  TCCR1B = _BV(CS10) | _BV(WGM13);
  ICR1 = period;
  OCR1A = 0;
  OCR1B = 0;
  TIMSK1 = _BV(TOIE1); // Enable overflow interrupt
}

ISR(TIMER1_OVF_vect) {
  if(count > sin_size) {
    count = 0;
    flag = !flag;
    if(flag) {
      digitalWrite(13, true);
      digitalWrite(13, false);
    }
}

  if(flag)
    OCR1A = (period / 10) * sinPWM[count];
  else
    OCR1B = (period / 10) * sinPWM[count];
  count++;

  ICR1 = period;
}


void loop() 
{
  while(Serial.available() > 0) {
    command = Serial.read();
    if(command == 'u')
      period += 100;
    else if (command == 'd')
      period -= 100;

    Serial.println(period);
  }
}
