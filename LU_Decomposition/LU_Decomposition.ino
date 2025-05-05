#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);

float A[2][2] = {{2,1},{1,1}};
float b[2]    = {5,3};
float L[2][2], U[2][2], z[2], x[2];
int ph=0;

void setup(){
  lcd.begin(16,2);
}
void loop(){
  if(ph==0){
    // Doolittle
    L[0][0]=1; U[0][0]=A[0][0];
    U[0][1]=A[0][1];
    L[1][0]=A[1][0]/U[0][0];
    U[1][1]=A[1][1]-L[1][0]*U[0][1];
    lcd.print("LU fact"); ph++; delay(2000);
  }
  else if(ph==1){
    // forward solve Lz=b
    z[0]=b[0]/L[0][0];
    z[1]=(b[1]-L[1][0]*z[0])/L[1][1];
    lcd.clear(); lcd.print("z0="); lcd.print(z[0],3);
    lcd.setCursor(0,1); lcd.print("z1="); lcd.print(z[1],3);
    ph++; delay(2000);
  }
  else if(ph==2){
    // back solve Ux=z
    x[1]=z[1]/U[1][1];
    x[0]=(z[0]-U[0][1]*x[1])/U[0][0];
    lcd.clear(); lcd.print("x="); lcd.print(x[0],3);
    lcd.setCursor(8,1); lcd.print("y="); lcd.print(x[1],3);
    while(1);
  }
}
