#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);

float A[2][3] = {{2,1,5},{1,1,3}}; // augmented
int phase=0;

void setup(){
  lcd.begin(16,2);
}
void loop(){
  if(phase==0){
    // make leading 1 in row0
    float f = A[0][0];
    for(int j=0;j<3;j++) A[0][j]/=f;
    lcd.setCursor(0,0); lcd.print("R0/= ");
    lcd.print(f,2);
    phase++; delay(2000);
  }
  else if(phase==1){
    // eliminate row1 col0
    float m = A[1][0];
    for(int j=0;j<3;j++) A[1][j] -= m*A[0][j];
    lcd.clear(); lcd.print("Elim R1 m="); lcd.print(m,2);
    phase++; delay(2000);
  }
  else if(phase==2){
    // make leading 1 in row1
    float f = A[1][1];
    A[1][2]/=f; A[1][1]=1;
    lcd.clear(); lcd.print("R1/= "); lcd.print(f,2);
    phase++; delay(2000);
  }
  else if(phase==3){
    // eliminate above
    float m = A[0][1];
    A[0][2] -= m*A[1][2];
    A[0][1]=0;
    lcd.clear(); lcd.print("Elim R0 m="); lcd.print(m,2);
    phase++; delay(2000);
  }
  else {
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("x=");
    lcd.print(A[0][2],4);
    lcd.setCursor(0,1); lcd.print("y=");
    lcd.print(A[1][2],4);
    while(1);
  }
}
