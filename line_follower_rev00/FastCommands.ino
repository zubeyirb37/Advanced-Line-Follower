
//////////////// DIGITALWRITE ----> 170KHZ~~- /////////////
void fastDigitalWrite(uint8_t pin, uint8_t val) {
  uint8_t port = digitalPinToPort(pin);
  uint8_t bit = digitalPinToBitMask(pin);
  volatile uint8_t *out;

  out = portOutputRegister(port);

  uint8_t oldSREG = SREG;   // Interrupt bayrağını kaydet
  cli();                    // Atomik işlemler için Interrupt ı kapat

  if (val == LOW) {
    *out &= ~bit;           // Biti temizle
  } else {
    *out |= bit;            // Biti işaretle
  }

  SREG = oldSREG;           // Interrupt bayrağını geri al
}


/////////////////////////////////////////////////////////////

///////////////// ANALOGWRITE -----> 62.5KHZ  //////////////
void fastAnalogWrite(uint8_t pin, uint8_t value) {
  if (pin == 3) 
  {
    // Timer 2 kullanarak Pin 3 için PWM üretimi (OC2B)
    DDRD |= (1 << DDD3);  // Pin 3 çıkış
    TCCR2A = (1 << WGM20) | (1 << WGM21); // Hızlı PWM Modu
    TCCR2A |= (1 << COM2B1);              // OC2B aktif (Pin 3)
    TCCR2B = (1 << CS20);  // Prescaler 1 (No prescaling)
    OCR2B = value;         // PWM değerini ayarla

  } 
  
  else if (pin == 11) 
  {
    // Timer 2 kullanarak Pin 11 için PWM üretimi (OC2A)
    DDRB |= (1 << DDB3);  // Pin 11 çıkış
    TCCR2A = (1 << WGM20) | (1 << WGM21); // Hızlı PWM Modu
    TCCR2A |= (1 << COM2A1);              // OC2A aktif (Pin 11)
    TCCR2B = (1 << CS20);  // Prescaler 1 (No prescaling)
    OCR2A = value;         // PWM değerini ayarla

  } 
  
  else if (pin == 5) 
  {
    // Timer 0 kullanarak Pin 5 için PWM üretimi (OC0B)
    DDRD |= (1 << DDD5);  // Pin 5 çıkış
    TCCR0A = (1 << WGM00) | (1 << WGM01); // Hızlı PWM Modu
    TCCR0A |= (1 << COM0B1);              // OC0B aktif (Pin 5)
    TCCR0B = (1 << CS00);  // Prescaler 1 (No prescaling)
    OCR0B = value;         // PWM değerini ayarla

  } 
  
  else if (pin == 6) 
  {
    // Timer 0 kullanarak Pin 6 için PWM üretimi (OC0A)
    DDRD |= (1 << DDD6);  // Pin 6 çıkış
    TCCR0A = (1 << WGM00) | (1 << WGM01); // Hızlı PWM Modu
    TCCR0A |= (1 << COM0A1);              // OC0A aktif (Pin 6)
    TCCR0B = (1 << CS00);  // Prescaler 1 (No prescaling)
    OCR0A = value;         // PWM değerini ayarla

  } 
}




///////////////////////////////////////////////////////////////

inline uint8_t fastDigitalRead(uint8_t pin) {
  uint8_t bit = digitalPinToBitMask(pin);      // Pin maskesini al
  uint8_t port = digitalPinToPort(pin);        // Portu al (PORTB, PORTC, PORTD)

  if (*portInputRegister(port) & bit) {        // Portun giriş register'ını oku
    return HIGH;                               // Eğer bit set ise HIGH
  }
  return LOW;                                  // Aksi takdirde LOW
}


///////////////////////////////////////////////////////////

inline int fastAbs(int value) {
  int mask = value >> (sizeof(int) * 8 - 1);  // Tüm bitleri en yüksek bitten itibaren sağa kaydırarak mask oluştur
  return (value + mask) ^ mask;               // Maskeyi kullanarak mutlak değeri hesapla
}


/////////////////////////////////////////////////////////

inline void setServoAngle(uint8_t angle) {
  
  servo_pwm_width = 544 + (angle * 10.31);  // Açıdan PWM süresine çevirme
}


/////////////////////////////////////////////////////////


long fastMap(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
