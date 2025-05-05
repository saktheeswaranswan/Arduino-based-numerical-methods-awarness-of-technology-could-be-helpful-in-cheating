#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// ======= FEM parameters =======
const int nElems = 10;              // number of elements
const int nNodes = nElems + 1;      // =11 nodes :contentReference[oaicite:5]{index=5}
float L = 1.0;                      // total length (m)
float Le = L / nElems;              // element length :contentReference[oaicite:6]{index=6}
float A = 1.0;                      // cross‑section area (m²)
float Evals[3] = {1.0, 2.0, 5.0};    // three E options :contentReference[oaicite:7]{index=7}
int Eindex = 0;                     // current E selection
float E = Evals[Eindex];

// nodal load vector and solution
float F[nNodes];
float u[nNodes];
float K[nNodes][nNodes];            // global stiffness :contentReference[oaicite:8]{index=8}

// ======= UI pins =======
const int btnUp    = 2;   // increase load
const int btnDown  = 3;   // decrease load :contentReference[oaicite:9]{index=9}
const int btnCycle = 4;   // cycle Young’s E :contentReference[oaicite:10]{index=10}

// UI state
unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 50;
int lastStateUp=HIGH, lastStateDown=HIGH, lastStateCycle=HIGH;
int displayNode = 0;
unsigned long lastRefresh=0;

// solve 11×11 via simple Gauss elimination
void solveSystem(int N, float M[][nNodes], float b[], float x[]) {
  for(int k=0; k<N-1; k++){
    for(int i=k+1; i<N; i++){
      float m = M[i][k] / M[k][k];
      for(int j=k; j<N; j++) M[i][j] -= m * M[k][j];
      b[i] -= m * b[k];
    }
  }
  for(int i=N-1; i>=0; i--){
    float s = b[i];
    for(int j=i+1; j<N; j++) s -= M[i][j]*x[j];
    x[i] = s / M[i][i];
  }
}

void assembleSolve() {
  // zero K & F
  for(int i=0;i<nNodes;i++){
    F[i]=0;
    for(int j=0;j<nNodes;j++) K[i][j]=0;
  }
  // assemble element stiffness ke into K :contentReference[oaicite:11]{index=11}
  float ke = E*A/Le;
  for(int e=0; e<nElems; e++){
    int n1=e, n2=e+1;
    K[n1][n1]+= ke;  K[n1][n2]+= -ke;
    K[n2][n1]+= -ke; K[n2][n2]+= ke;
  }
  // apply Dirichlet BC at node0 (u0=0) :contentReference[oaicite:12]{index=12}
  for(int j=0;j<nNodes;j++){ K[0][j]=0; K[j][0]=0; }
  K[0][0]=1; F[0]=0;
  // apply variable load at last node
  K[nNodes-1][nNodes-1] += 0;  // no modification to stiffness
  F[nNodes-1] = F[nNodes-1];    // already set by button logic
  // solve
  solveSystem(nNodes, K, F, u);
}

void setup() {
  lcd.begin(16,2);
  pinMode(btnUp,    INPUT_PULLUP);
  pinMode(btnDown,  INPUT_PULLUP);
  pinMode(btnCycle, INPUT_PULLUP);
  // initial load
  F[nNodes-1] = 1.0;
  assembleSolve();
}

void loop() {
  unsigned long now = millis();
  // --- read buttons with debounce ---
  int rUp    = digitalRead(btnUp);
  int rDown  = digitalRead(btnDown);
  int rCycle = digitalRead(btnCycle);
  if(rUp   == LOW && lastStateUp   == HIGH && now - lastDebounce > debounceDelay) {
    F[nNodes-1] += 0.5;        // increase load by 0.5 N :contentReference[oaicite:13]{index=13}
    assembleSolve();
    lastDebounce = now;
  }
  if(rDown == LOW && lastStateDown == HIGH && now - lastDebounce > debounceDelay) {
    F[nNodes-1] = max(0.0, F[nNodes-1] - 0.5);  // decrease but not below 0 :contentReference[oaicite:14]{index=14}
    assembleSolve();
    lastDebounce = now;
  }
  if(rCycle== LOW && lastStateCycle== HIGH && now - lastDebounce > debounceDelay) {
    Eindex = (Eindex + 1) % 3;  // cycle E option
    E = Evals[Eindex];
    assembleSolve();
    lastDebounce = now;
  }
  lastStateUp=rUp; lastStateDown=rDown; lastStateCycle=rCycle;

  // --- refresh display every 2 s ---
  if(now - lastRefresh > 2000) {
    lastRefresh = now;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Load=");
    lcd.print(F[nNodes-1],1);
    lcd.print("N E=");
    lcd.print(E,1);
    lcd.setCursor(0,1);
    lcd.print("Node"); lcd.print(displayNode);
    lcd.print(":"); lcd.print(u[displayNode],3);
    displayNode = (displayNode + 1) % nNodes;
  }
}
