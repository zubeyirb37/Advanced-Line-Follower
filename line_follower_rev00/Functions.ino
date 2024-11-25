inline void FindPosition(int X[])
{
  int32_t Sum_Weighted=0, Sum_nonWeighted=0;
  long Position; 

  Sum_Weighted=0;
  Sum_nonWeighted=0;

  
  for(short i=0; i<8; i++)
  {
    Sum_Weighted+=X[i]* Weights[i];   
    Sum_nonWeighted+= X[i];               
  }

  Position=Sum_Weighted/Sum_nonWeighted;

  
  Pos_A=Position;    

  PD(Position);
}



inline void PD(long Position)
{
  static double Error, P_OUT, D_OUT, PD_OUT, Prev_Error=0;


  Error=Position-3500;
  Err_A=int(Error);
  
  P_OUT=KP*Error;
  D_OUT=KD*(Error-Prev_Error);                                                                               
 
  PD_OUT=P_OUT+D_OUT;                  

  Prev_Error=Error;

  Motor_PD(PD_OUT);
         
}



inline void Motor_PD(double Main_Iteration)
{
  static double sol,sag;      //Sol ve Sağ PWM değişkeni
  static short Sol,Sag;

  sag=(Main_Iteration<0)? MAIN_SPEED+Main_Iteration : MAIN_SPEED;  //Motorların gerekli hesaplamalarını yap
  sol=(Main_Iteration<0)? MAIN_SPEED : MAIN_SPEED-Main_Iteration;

  sag=(sag>255)? 255 :(sag<=-MAIN_SPEED)? -MAIN_SPEED : sag;
  sol=(sol>255)? 255 :(sol<=-MAIN_SPEED)? -MAIN_SPEED : sol;

  Sol=(short)sol;
  Sag=(short)sag;

  Motor(Sol,Sag);

}

inline void Motor(short sol, short sag)
{
  if(sol<0)
  { 
    sol = fastAbs(sol);  
    fastDigitalWrite(LEFT_B_PIN, HIGH); 
    fastDigitalWrite(LEFT_F_PIN, LOW);
    fastAnalogWrite(LEFT_PWM, sol); 
  }   //Eğer sol 0'dan küçükse (Geri Hareket için) sol mutlak değer, Yön Pini 1 ve PWM 255-SOL olur
  
  else
  { 
    fastDigitalWrite(LEFT_B_PIN, LOW); 
    fastDigitalWrite(LEFT_F_PIN, HIGH); 
    fastAnalogWrite(LEFT_PWM, sol); 
  }   //Değilse zaten değer pozitiftir Yön Pini 0 ve PWM SOL olur


  if(sag<0)
  { 
    sag = fastAbs(sag);  
    fastDigitalWrite(RIGHT_B_PIN, HIGH); 
    fastDigitalWrite(RIGHT_F_PIN, LOW); 
    fastAnalogWrite(RIGHT_PWM, sag); 
  } //Eğer sag 0'dan küçükse (Geri Hareket için) sol mutlak değer, Yön Pini 1 ve PWM 255-SAG olur
  
  else
  {
    fastDigitalWrite(RIGHT_B_PIN, LOW); 
    fastDigitalWrite(RIGHT_F_PIN, HIGH);
    fastAnalogWrite(RIGHT_PWM, sag); 
  } //Değilse zaten değer pozitiftir Yön Pini 0 ve PWM SAG olur
}
