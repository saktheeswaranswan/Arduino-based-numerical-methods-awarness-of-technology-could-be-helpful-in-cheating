#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);

// Solve:
//  a11*x + a12*y = b1
//  a21*x + a22*y = b2
float a11=2, a12=1, b1=5,
      a21=1, a22=1, b2=3;
float m, x, y;
unsigned long last=0;
int step=0;

void setup(){
  lcd.begin(16,2);
}
void loop(){
  if(step==0){
    // eliminate a21
    m = a21/a11;
    a21 -= m*a11; // becomes 0
    a22 -= m*a12;
    b2  -= m*b1;
    lcd.setCursor(0,0); lcd.print("Elim: m=");
    lcd.print(m,2);
    step++;
    delay(2000);
  }
  else if(step==1){
    // back-substitute
    y = b2/a22;
    x = (b1 - a12*y)/a11;
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("x=");
    lcd.print(x,4);
    lcd.setCursor(0,1); lcd.print("y=");
    lcd.print(y,4);
    step++;
  }
}
