#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);

float A[2][2]={{2,1},{1,1}}, b[2]={5,3}, x[2]={0,0};
float ω = 1.25; // relaxation factor
int k=0;

void setup(){
  lcd.begin(16,2);
}
void loop(){
  if(k<5){
    for(int i=0;i<2;i++){
      float sigma = 0;
      for(int j=0;j<2;j++) if(j!=i) sigma += A[i][j]*x[j];
      float x_old = x[i];
      x[i] = (1-ω)*x_old + (ω/A[i][i])*(b[i]-sigma);
    }
    lcd.setCursor(0,0); lcd.print("SOR Itr"); lcd.print(k+1);
    lcd.setCursor(0,1); lcd.print("x="); lcd.print(x[0],3);
    lcd.print(" y="); lcd.print(x[1],3);
    k++; delay(2000);
  }
}
