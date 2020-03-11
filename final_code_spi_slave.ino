#include <SPI.h>
int ascii = 0;

int eo[]={0,1,0,1,0,0};
const boolean dataset [] [6]= 
{ 
             {1,0,0,0,0,0},//A0
             
             {1,0,1,0,0,0},
             
             {1,1,0,0,0,0},
             
             {1,1,0,1,0,0},
             
             {1,0,0,1,0,0},
             
             {1,1,1,0,0,0},
                
             {1,1,1,1,0,0},
              
             {1,0,1,1,0,0},
              
             {0,1,1,0,0,0},
              
             {0,1,1,1,0,0},
              
             {1,0,0,0,1,0},
            
             {1,0,1,0,1,0},
                  
             {1,1,0,0,1,0},
              
             {1,1,0,1,1,0},
              
             {1,0,0,1,1,0},
              
             {1,1,1,0,1,0},
              
             {1,1,1,1,1,0},
              
             {1,0,1,1,1,0},
              
             {0,1,1,0,1,0},
             
             {0,1,1,1,1,0},
              
             {1,0,0,0,1,1},
              
             {1,0,1,0,1,1},
              
             {0,1,1,1,0,1},
              
             {1,1,0,0,1,1},
            
             {1,1,0,1,1,1},
             
             {1,0,0,1,1,1},
                         
      {1,0,0,1,0,0},//9  35
      {1,0,0,1,0,0},//SPACE 36
      {1,0,0,1,0,0},//FULLSTOP 37
};

/*void cwtenrotation(int z)
{
  for(int count=0;count<40;count++)
  {
    digitalWrite(z, HIGH);
    digitalWrite(z+1, LOW);
    delay(200);
    digitalWrite(z, HIGH);
    digitalWrite(z+1, HIGH);
    delay(200);
    digitalWrite(z, LOW);
    digitalWrite(z+1, HIGH);
    delay(200);
    digitalWrite(z, LOW);
    digitalWrite(z+1, LOW);
    delay(200);
  }
}
void ccwtenrotation(int z)
{
  for(int count=0;count<40;count++)
  {
    digitalWrite(z, HIGH);
    digitalWrite(z+1, LOW);
    delay(200);
    digitalWrite(z, LOW);
    digitalWrite(z+1, LOW);
    delay(200);
    digitalWrite(z, LOW);
    digitalWrite(z+1, HIGH);
    delay(200);
    digitalWrite(z, HIGH);
    digitalWrite(z+1, HIGH);
    delay(200);
  }
}*/
void setup (void)
{
  Serial.begin (9600);// debugging
  for (int z=2;z<8;z++)
  {
    pinMode(z, OUTPUT);
  }
  /*pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);*/
  SPCR |= bit (SPE); // turn on SPI in slave mode
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  // get ready for an interrupt 
  // now turn on interrupts
  SPI.attachInterrupt();
}  // end of setup

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR;  // grab byte from SPI Data Register
  Serial.print(c);
  ascii = int(c);
  if (ascii>=65 && ascii<=90){
    eo[0]=dataset [ascii-65] [0];
    eo[1]=dataset [ascii-65] [1];
    eo[2]=dataset [ascii-65] [2];
    eo[3]=dataset [ascii-65] [3];
    eo[4]=dataset [ascii-65] [4];
    eo[5]=dataset [ascii-65] [5];
  }
  else if (ascii>=48 && ascii<=57){
    eo[0]=dataset [ascii-22] [0];
    eo[1]=dataset [ascii-22] [1];
    eo[2]=dataset [ascii-22] [2];
    eo[3]=dataset [ascii-22] [3];
    eo[4]=dataset [ascii-22] [4];
    eo[5]=dataset [ascii-22] [5];
  }
  else if (ascii==32){
    eo[0]=dataset [ascii+4] [0];
    eo[1]=dataset [ascii+4] [1];
    eo[2]=dataset [ascii+4] [2];
    eo[3]=dataset [ascii+4] [3];
    eo[4]=dataset [ascii+4] [4];
    eo[5]=dataset [ascii+4] [5];
  }
  else {
    eo[0]=dataset [ascii-9] [0];
    eo[1]=dataset [ascii-9] [1];
    eo[2]=dataset [ascii-9] [2];
    eo[3]=dataset [ascii-9] [3];
    eo[4]=dataset [ascii-9] [4];
    eo[5]=dataset [ascii-9] [5];
  }
  for (int b=2;b<8;b++){
    if (eo[b-2]==1){
      digitalWrite(b,HIGH);
    }
    else digitalWrite(b,LOW);
  }
  /*for (int m=0;m<=9;m=m+2)
  {
    if (eo[m/2]==1){
      ccwtenrotation(m);
      }
  }
  for(int m=0;m<=9;m=m+2)
  {
    if(eo[m/2]==1){
      cwtenrotation(m);
    }
  }*/
}// end of interrupt routine SPI_STC_vect
// main loop - wait for flag set in interrupt routine
void loop (void)
{
  
}
// end of loop
