

// Update the below parameters for your project
const char* ssid = "xxxx";
const char* password = "xxxx";


struct Config {
  char loconame[64];
  char wifimode[10];
  char ssid[30];
  char password[30];
  byte acceleration;
  byte deceleration;
  byte minspeed;
  byte maxspeed;
  byte volume;
  char audio1[20];
  char audio2[20];
  char audio3[20];
  char audio4[20];
  char light3[20];
  char light4[20];
  int stationwait;
  int blindtime;
  char stationstopsound[6];
  char stationleavesound1[6];
  char stationleavesound2[6];
  char terminusstopsound[6];
  char terminusleavesound1[6];
  char terminusleavesound2[6];
  char input3sound[6];
  char input4sound[6]; 
};
Config config;

const char *filename = "/config.json";  // 
