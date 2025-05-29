#include <Keypad.h>
#include <LiquidCrystal.h>

// LCD pins (change according to your wiring)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // RS, E, D4, D5, D6, D7

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;

// Define the keymap (common calculator layout)
char keys[ROWS][COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'C', '0', '=', '/'}
};

byte rowPins[ROWS] = {A0, A1, A2, A3}; // Connect to row pins of the keypad
byte colPins[COLS] = {A4, A5, 2, 3};   // Connect to column pins of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String currentInput = "";    // To store input digits/operators
double operand1 = 0, operand2 = 0;
char operation = 0;
bool waitingForSecondOperand = false;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Arduino Calc");
  delay(1500);
  lcd.clear();
  lcd.print("Enter number:");
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    if (key >= '0' && key <= '9') {
      // If we already have an operation and waiting second operand, append to currentInput
      currentInput += key;
      lcd.clear();
      lcd.print(currentInput);
    } 
    else if (key == '+' || key == '-' || key == '*' || key == '/') {
      if (!waitingForSecondOperand && currentInput.length() > 0) {
        operand1 = currentInput.toDouble();
        operation = key;
        waitingForSecondOperand = true;
        currentInput = "";  // Clear input for second operand
        lcd.clear();
        lcd.print(operand1);
        lcd.print(" ");
        lcd.print(operation);
      }
    }
    else if (key == '=') {
      if (waitingForSecondOperand && currentInput.length() > 0) {
        operand2 = currentInput.toDouble();
        double result = calculate(operand1, operand2, operation);
        lcd.clear();
        lcd.print("Result:");
        lcd.setCursor(0, 1);
        lcd.print(result, 6);  // Display result with 6 decimal places
        // Reset for next calculation
        currentInput = "";
        waitingForSecondOperand = false;
        operation = 0;
        operand1 = result;  // Optionally keep result as new operand1 for chaining
      }
    }
    else if (key == 'C') {
      clearCalculator();
    }
  }
}

double calculate(double a, double b, char op) {
  switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return (b != 0) ? a / b : 0;  // simple divide by zero handling
    default: return 0;
  }
}

void clearCalculator() {
  currentInput = "";
  operand1 = 0;
  operand2 = 0;
  operation = 0;
  waitingForSecondOperand = false;
  lcd.clear();
  lcd.print("Cleared");
  delay(1000);
  lcd.clear();
  lcd.print("Enter number:");
}

