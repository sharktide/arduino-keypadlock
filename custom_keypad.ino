#include <Keypad.h>
#include <LinkedList.h>
#include <Servo.h>
#define GREEN 49
#define RED 45
#define BLUE 47
#define WHITE 43
#define LOCK_PIN 10
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns

// Define the symbols on the buttons of the keypad
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; // Connect to the column pinouts of the keypad

// Initialize an instance of the Keypad class
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// LinkedList to hold the input characters
LinkedList<char> ASCII;

unsigned long lastInputTime = 0;  // Used to track the last input time


Servo lockServo;

void setup() {
  Serial.begin(9600);  // Start serial communication
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(WHITE, OUTPUT);

  lockServo.attach(LOCK_PIN);  // Attach the servo to the lock pin
  lockServo.write(0);  // Lock the mechanism initially (0 degrees)

  lastInputTime = millis();  // Initialize last input time
}

void loop() {
  char customKey = customKeypad.getKey();  // Get the key pressed from the keypad

  if (customKey) {  // Check if a key is pressed
    if (customKey == '*') {  // If '*' is pressed, clear the list
      digitalWrite(BLUE, HIGH);
      digitalWrite(WHITE, HIGH);
      delay(750);
      digitalWrite(BLUE, LOW);
      digitalWrite(WHITE, LOW);
      ASCII.clear();
      Serial.println("List cleared due to customKey being '*'");
    } else { 
      Serial.print("Key pressed: ");
      Serial.println(customKey);  // Print the key pressed
      ASCII.add(customKey);  // Add the key to the list
      digitalWrite(WHITE, HIGH);
      delay(250);
      digitalWrite(WHITE, LOW);
    }
  }

  // Check if the sequence is found in the list
  if (checkSequence()) {
    Serial.println("Sequence found, triggering action!");
    performAction();
  }
}

// Function to check for the specific sequence 7, 'A', 6, 4
bool checkSequence() {
  if (ASCII.size() >= 4) {
    if (ASCII.get(0) == '7' && ASCII.get(1) == 'A' && ASCII.get(2) == '6' && ASCII.get(3) == '4') {
      Serial.println("Sequence matched!");
      return true;  // Sequence matched
    }
    else{
      Serial.println("Wrong Code!");
      digitalWrite(RED, HIGH);
      delay(1000);
      digitalWrite(RED, LOW);
      ASCII.clear();
    }
  }
  return false;  // Sequence not matched
}

// Function to perform the action when the sequence is found
void performAction() {
  Serial.println("Action performed because the sequence matched!");
  digitalWrite(GREEN, HIGH);
  lockServo.write(90);  // Unlock the mechanism (servo to 90 degrees)
  delay(3000);
  lockServo.write(0);  // Lock the mechanism again (servo to 0 degrees)
  digitalWrite(GREEN, LOW);
  ASCII.clear();  // Clear the list after the action is performed
}
