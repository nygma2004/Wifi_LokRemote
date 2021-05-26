#define BUILTIN_LED 2
#define MOTORIN1 19                   // IN1 input of the DRV8871 motor driver board
#define MOTORIN2 21                   // IN2 input of the DRV8871 motor driver board
#define PWMFREQ 15000                 // PWM frequency
#define LIGHT1 4                      // Light output 1: headlights
#define LIGHT2 5                      // Light output 2: headlights
#define LIGHT3 18                     // Light output 3
#define LIGHT4 23                     // Light output 4
#define BATTERYMONITOR 36             // Analoge input for battery voltage reading
#define INPUT1 32                     // Input 1: sound trigger
#define INPUT2 33                     // Input 2: sound trigger
#define INPUT3 34                     // Input 3: station stop trigger
#define INPUT4 35                     // Input 4: terminus stop trigger

#define INPUTDEBOUNCE 250             // input software debounce delay
#define DRIVEMODE_DRIVING 0           // Shuttle mode: loco is driving, waiting for a station of terminus stop trigger
#define DRIVEMODE_STATIONBREAK 1      // Shuttle mode: station magnet detected, loco is slowing down to stop at a station
#define DRIVEMODE_STATIONSTOP 2       // Shuttle mode: loco is stopped at the station and counting down the wait time
#define DRIVEMODE_TERMINUSBREAK 3     // Shuttle mode: terminus magnet detected, loco is slowing down to stop at a terminus
#define DRIVEMODE_TERMINUSSTOP 4      // Shuttle mode: loco is stopped at the terminus and counting down the wait time

#define BATTARRSIZE 10                // Array size for averaging the battery reading

String webStat = "";
String webMsg = "";

unsigned long lastMinute, uptime, lastSecond, sec, msgcount, lastDisplay, lastSpeed, lastInput1, lastInput2, lastInput3, lastInput4;
byte direction = 0;
byte speed = 0;
byte targetspeed = 0;
byte autospeed = 0;
byte drivemode = 0;
String configJSON;
int motorspeed, stationwait, blindtime;
bool lightState, light3State, light4State, input1State, input2State, shuttlemode;
float battery;
float battarr[BATTARRSIZE];
byte battarrcount = 0;
char startsound[6];
bool triggersound = false;

//auto led_breathe = JLed(jled::Esp32Hal(LIGHT1,2)).Off();
