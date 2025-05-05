#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);

float A[2][2]={{2,1},{1,1}}, b[2]={5,3}, x[2]={0,0}, x_new[2];
int iter=0;

void setup(){
  lcd.begin(16,2);
}
void loop(){
  if(iter<5){
    x_new[0] = (b[0]-A[0][1]*x[1])/A[0][0];
    x_new[1] = (b[1]-A[1][0]*x[0])/A[1][1];
    x[0]=x_new[0]; x[1]=x_new[1];
    lcd.setCursor(0,0);
    lcd.print("Itr"); lcd.print(iter+1);
    lcd.setCursor(0,1);
    lcd.print("x="); lcd.print(x[0],3);
    lcd.print(" y="); lcd.print(x[1],3);
    iter++; delay(2000);
  }
}
