/*
 created July 6 2013
 by Chris Campbell
 */

// include the library code:
#include <LiquidCrystal.h>

// Character Height and Width
#define CH_H 8
#define CH_W 5

// initialize the library with the numbers of the interface pins
// This matches up with the SainSmart LCD Keypad for Arduino
// http://www.amazon.com/dp/B006OVYI1G/ref=pe_175190_21431760_M3T1_ST1_dp_1

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


// This holds the CPU values
int cpu[64];
// This holds the number of CPUs
int count = 0;
int memAvail = 500;
int memUsed = 335;

// These are the special characters used for the bars
byte b2[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};
byte b3[8]=
{
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111
};
byte b4[8]=
{
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111
};
byte b5[8]={
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte b6[8]=
{
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte b7[8]=
{
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte b8[8]=
{
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte b9[8]=
{
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

void setup() {
  // initialize the serial communications:
  Serial.begin(9600);
  
  // Create the characters
  lcd.createChar(0,b2);
  lcd.createChar(1,b3);
  lcd.createChar(2,b4);
  lcd.createChar(3,b5);
  lcd.createChar(4,b6);
  lcd.createChar(5,b7);
  lcd.createChar(6,b8);
  lcd.createChar(7,b9);
  
  
  // Initial message
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("Waiting for data");

}

void loop() {
  // If we have received data count will not be 0
  if(count != 0){
	// Clear the display so we don't get ghost characters
    lcd.clear();
  }
  // If we have data available, parse it
  if(Serial.available() > 0){
    readData();
  }
  // Display the data
  displayData();
  delay(500);
}

/* Expected serial format:
#C,<Number of CPUs>,<cpu1>,<cpu2>,<...>,<cpuN>,M,<Memory used>,<Memory Available>,D,<Date string>#

Example: for 8 cores
#C,8,1,29,4,3,6,5,6,9,M,11324,16331,D,11:25 AM#
*/
void readData(){
  int c = 0;
  while(c != '#'){
    c = Serial.read();
  }
  if(Serial.peek() == 'C'){
    Serial.read(); // clear the 'C'
    Serial.read(); // Clear the ','
    count = Serial.parseInt();
    for(int i = 0; i < count; i++){
      Serial.read(); // clear the ','
      cpu[i] = Serial.parseInt();
    }
    Serial.read(); // clear the ','
  }
  
  if(Serial.peek() == 'M'){
    Serial.read(); Serial.read();
    memUsed = Serial.parseInt();
    Serial.read();
    memAvail = Serial.parseInt();
    Serial.read();
  }
  
  if(Serial.peek() == 'D'){
    while(Serial.read() != '#'){}
  }
    
}


// This displays memory on the top character row
// and each CPU gets two columns on the bottom row
void displayData(){
  if(count == 0){
    return;
  }
  // Display memory
  const int WIDTH = 16;
  int seg = memAvail/WIDTH; // memory per block
  int top = memUsed/seg;
  int extra = memUsed-(top*seg);
  int perc = (extra*100)/seg;
  
  for(int i = 0; i < top; i++){
    lcd.setCursor(i,0);
    lcd.write(7); // full block
  }
  writePerc(top,0,perc); // show the remaining memory
  
  // Show CPUS
  // In the future there will be code to automatically determine
  // how many columns to give each CPU
  for(int i = 0; i < count; i++){
    writePerc((i*2),1,cpu[i]);
    writePerc((i*2)+1,1,cpu[i]);
  }
}


// Calculates a percentage and displays it at the given location
void writePerc(int x, int y, int perc){
  // sanity checks
  if(perc > 99){ perc = 99; }
  if(perc < 2) { perc = 0; }
  
  lcd.setCursor(x,y);
  
  // threshold per row (about 12%)
  const int thresh = 100/CH_H;
  // find the max size and display it
  for(int i = 0; i < 9; i++){
    if(perc < thresh*(i+1)){
      if(i == 0){
        lcd.write(" "); // blank
      }else{
        lcd.write(i-1);
      }
      break;
    }
  }
}

