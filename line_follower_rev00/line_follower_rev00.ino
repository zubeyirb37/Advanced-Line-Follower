

#pragma GCC optimize("Ofast")  // Performans odaklı optimizasyon
#pragma GCC target("arch=native")  // Yerel işlemci optimizasyonu

#define SCount 8
int32_t Sensor[SCount];
const short SPin[SCount]={A0,A1,A2,A3,A4,A5,A6,A7};

int sensorMax[SCount];
int sensorMin[SCount];

int normalized_value[SCount], QTR_Normalized[SCount];

volatile int servo_pwm_width = 0; 

#define RIGHT_PWM 11      
#define RIGHT_F_PIN 9
#define RIGHT_B_PIN 10 

#define LEFT_PWM 6       
#define LEFT_F_PIN 7
#define LEFT_B_PIN 8

#define FAN_PIN 2


#define MAIN_SPEED 80       //60 (yV)    //90 (oV)      90 (yV)     
#define KP 0.044            //0,025      //0,05         0,05
#define KD 0.184            //0,075      //0,1          0,16

int Err_A;
int32_t Pos_A;
int Angle,Abs_Err,fan_flag=0;
static bool Surface=1; // 0 = Beyaz çizgi siyah zemin - 1 = Siyah çizgi beyaz zemin

const uint32_t Weights[8] = {0,1000,2000,3000,4000,5000,6000,7000};


void setup() 
{


  
  pinMode(5,OUTPUT);
  pinMode(2,OUTPUT);
  
  pinMode(RIGHT_PWM,OUTPUT);
  pinMode(RIGHT_F_PIN,OUTPUT);
  pinMode(RIGHT_B_PIN,OUTPUT);
  
  pinMode(LEFT_B_PIN,OUTPUT);
  pinMode(LEFT_F_PIN,OUTPUT);
  pinMode(LEFT_PWM,OUTPUT);
  
  ADCSRA = (ADCSRA & 0xF8) | (1 << ADPS2) | (1 << ADPS1);  // Prescale = 64

   for(uint8_t y=0;y<SCount;y++)
   {
    sensorMax[y] = 0;
    sensorMin[y] = 1023;
   }

for (long x = 0; x < 7000; x++) {
    for (uint8_t i = 0; i < SCount; i++) {
        Sensor[i] = analogRead(SPin[i]);
        sensorMax[i] = max(sensorMax[i], Sensor[i]);
        sensorMin[i] = min(sensorMin[i], Sensor[i]);
    }
}

  delay(3000);
  
  noInterrupts();           // Interruptları kapat
  TCCR1A = 0;               // Timer1'in normal moda ayarlanması (CTC için)
  TCCR1B = 0;               // Timer'ı sıfırla

  TCNT1 = 0;                // Timer sayacını sıfırla
  OCR1A = 4000;             // 250 µs için karşılaştırma değeri

  TCCR1B |= (1 << WGM12);   // CTC (Clear Timer on Compare Match) modunu etkinleştir
  TCCR1B |= (1 << CS10);    // Prescaler 1 ile timer'ı başlat (16 MHz frekans)

  TIMSK1 |= (1 << OCIE1A);  // Timer karşılaştırma interrupt'unu etkinleştir
  
  interrupts();             // Interruptları tekrar aç

}

void loop() 
{
  fastDigitalWrite(5,1);

  #pragma GCC ivdep
  for(uint8_t i=0;i<SCount;i++)     
  {
      Sensor[i]=0;
      Sensor[i]=analogRead(SPin[i]);
      normalized_value[i] = (Sensor[i] - sensorMin[i]) * 1000 / (sensorMax[i] - sensorMin[i]);
      if(Surface==0)
      {
        QTR_Normalized[i] = (sensorMax[i]-normalized_value[i]);
      }
      else
      {
        QTR_Normalized[i]=normalized_value[i];
      }
  }




  
  FindPosition(QTR_Normalized);

  
  fastDigitalWrite(5,0);
}










ISR(TIMER1_COMPA_vect) 
{
  static byte counter=0;

  if (counter < (servo_pwm_width / 250))  // Servo PWM genişliğini karşılaştır
  {
    fastDigitalWrite(FAN_PIN, 1);  // PWM HIGH
  } 
  else 
  {
    fastDigitalWrite(FAN_PIN, 0);  // PWM LOW
  }
  
  counter++;
  
  counter=(counter==80)?0:counter;
}
