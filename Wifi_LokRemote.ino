// Aurduino IDE settings:
// Board: ESP32 Dev Board
// Upload speed: 912600
// CPU Frequency: 240 Mhz
// Flash Frequency 80 Mhz
// Flash Mode: QIO
// Flash Size: 4MB
// Partition: 1MB PROG, 3MB SPIFFS
// PSRAM: Disabled
//--------------------------------
//PINout
// D19: DRV8871 IN1
// D21: DRV8871 IN2
//MAX98357A I2S Amp
// D26: BCLK
// D25: LRC
// D22: DIN
//--------------------------------
#include <ESP32CAN.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "SPIFFS.h"

#include "webpage.h" //Our HTML webpage contents with javascripts
#include "configpage.h" //configuration webpage
#include "settings.h"
#include "globals.h"
//#include <jled.h>
#include <ArduinoJson.h>

#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"

// These lines were required for the second task to hit the watchdog timer properly
// https://github.com/espressif/arduino-esp32/issues/595#issuecomment-423796631
#include "soc/timer_group_struct.h"  
#include "soc/timer_group_reg.h"

WebServer server(80);                         // Web server

TaskHandle_t AudioTask;

AudioGeneratorWAV *wav;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;

// Loads the configuration from SPIFF, also setting the default configuration if JSON does not exist
bool loadConfiguration(const char *filename, Config &config, String localjson) {
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<800> doc;
  
  // Open file for reading
  File configfile = SPIFFS.open(filename);
    
  if (localjson=="") {
  // Set config defaults before anything
  strlcpy(config.loconame,"LocoRemote",64);
  strlcpy(config.wifimode,"ap",10);
  strlcpy(config.ssid,"LocoRemote",30);
  strlcpy(config.password,"12345678",30);
  config.acceleration = 100;
  config.deceleration = 100;
  config.minspeed = 135;
  config.maxspeed = 255;
  config.volume = 2;
  strlcpy(config.audio1,"Bell",20);
  strlcpy(config.audio2,"Whistle",20);
  strlcpy(config.audio3,"Guard",20);
  strlcpy(config.audio4,"Horn",20);
  strlcpy(config.light3,"Cab",20);
  strlcpy(config.light4,"Engine",20);
  config.stationwait = 60;
  config.blindtime = 30;
  strlcpy(config.stationstopsound,"none",6);
  strlcpy(config.stationleavesound1,"none",6);
  strlcpy(config.stationleavesound2,"none",6);
  strlcpy(config.terminusstopsound,"none",6);
  strlcpy(config.terminusleavesound1,"none",6);
  strlcpy(config.terminusleavesound2,"none",6);
  strlcpy(config.input3sound,"none",6);
  strlcpy(config.input4sound,"none",6);
  

  if(!configfile){
      Serial.println("Failed to open file for reading");
      return false;
  }
    


  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, configfile);
  if (error) {
    Serial.print(F("Failed to read the JSON configuration file: "));
    Serial.println(error.f_str());   
    return false;
  }

  Serial.println("----- Configuration file -----");
  } else {
    DeserializationError error = deserializeJson(doc, localjson.c_str());
    if (error) {
      Serial.print(F("Failed to read the JSON data: "));
      Serial.println(error.f_str());   
      return false; 
    }
  }

  // Copy values from the JsonDocument to the Config
  if (doc.containsKey("acceleration")) {
    config.acceleration = doc["acceleration"];
  }
  if (doc.containsKey("deceleration")) {
    config.deceleration = doc["deceleration"];
  }
  if (doc.containsKey("minspeed")) {
    config.minspeed = doc["minspeed"];
  }
  if (doc.containsKey("maxspeed")) {
    config.maxspeed = doc["maxspeed"];
  }
  if (doc.containsKey("volume")) {
    config.volume = doc["volume"];
    out->SetGain((float)config.volume/10);
  }
  if (doc.containsKey("loconame")) {
    strlcpy(config.loconame,doc["loconame"],64);
  }
  if (doc.containsKey("wifimode")) {
    strlcpy(config.wifimode,doc["wifimode"],10);
  }
  if (doc.containsKey("ssid")) {
    strlcpy(config.ssid,doc["ssid"],30);
  }
  if (doc.containsKey("password")) {
    strlcpy(config.password,doc["password"],30);
  }
  if (doc.containsKey("audio1")) {
    strlcpy(config.audio1,doc["audio1"],20);
  }
  if (doc.containsKey("audio2")) {
    strlcpy(config.audio2,doc["audio2"],20);
  }
  if (doc.containsKey("audio3")) {
    strlcpy(config.audio3,doc["audio3"],20);
  }
  if (doc.containsKey("audio4")) {
    strlcpy(config.audio4,doc["audio4"],20);
  }
  if (doc.containsKey("light3")) {
    strlcpy(config.light3,doc["light3"],20);
  }
  if (doc.containsKey("light4")) {
    strlcpy(config.light4,doc["light4"],20);
  }
  if (doc.containsKey("stationwait")) {
    config.stationwait = doc["stationwait"];
  }
  if (doc.containsKey("blindtime")) {
    config.blindtime = doc["blindtime"];
  }
  if (doc.containsKey("stationstopsound")) {
    strlcpy(config.stationstopsound,doc["stationstopsound"],6);
  }
  if (doc.containsKey("stationleavesound1")) {
    strlcpy(config.stationleavesound1,doc["stationleavesound1"],6);
  }
  if (doc.containsKey("stationleavesound2")) {
    strlcpy(config.stationleavesound2,doc["stationleavesound2"],6);
  }
  if (doc.containsKey("terminusstopsound")) {
    strlcpy(config.terminusstopsound,doc["terminusstopsound"],6);
  }
  if (doc.containsKey("terminusleavesound1")) {
    strlcpy(config.terminusleavesound1,doc["terminusleavesound1"],6);
  }
  if (doc.containsKey("terminusleavesound2")) {
    strlcpy(config.terminusleavesound2,doc["terminusleavesound2"],6);
  }
  if (doc.containsKey("input3sound")) {
    strlcpy(config.input3sound,doc["input3sound"],6);
  }
  if (doc.containsKey("input4sound")) {
    strlcpy(config.input4sound,doc["input4sound"],6);
  }
  // Close the file (Curiously, File's destructor doesn't close the file)
  configfile.close();
  return true;
}

// Conversion of the settings structure to JSON string
String ConfigToString() {
  String message = "{";
  message += "\"loconame\": \"";
  message += config.loconame;
  message += "\", \"wifimode\": \"";
  message += config.wifimode;
  message += "\", \"ssid\": \"";
  message += config.ssid;
  message += "\", \"password\": \"";
  message += config.password;
  message += "\", \"acceleration\": ";
  message += config.acceleration;
  message += ", \"deceleration\": ";
  message += config.deceleration;
  message += ", \"minspeed\": ";
  message += config.minspeed;
  message += ", \"maxspeed\": ";
  message += config.maxspeed;
  message += ", \"volume\": ";
  message += config.volume;
  message += ", \"audio1\": \"";
  message += config.audio1;
  message += "\", \"audio2\": \"";
  message += config.audio2;
  message += "\", \"audio3\": \"";
  message += config.audio3;
  message += "\", \"audio4\": \"";
  message += config.audio4;
  message += "\", \"light3\": \"";
  message += config.light3;
  message += "\", \"light4\": \"";
  message += config.light4;
  message += "\", \"stationwait\": ";
  message += config.stationwait;
  message += ", \"blindtime\": ";
  message += config.blindtime;
  message += ", \"stationstopsound\": \"";
  message += config.stationstopsound;
  message += "\", \"stationleavesound1\": \"";
  message += config.stationleavesound1;
  message += "\", \"stationleavesound2\": \"";
  message += config.stationleavesound2;
  message += "\", \"terminusstopsound\": \"";
  message += config.terminusstopsound;
  message += "\", \"terminusleavesound1\": \"";
  message += config.terminusleavesound1;
  message += "\", \"terminusleavesound2\": \"";
  message += config.terminusleavesound2;
  message += "\", \"input3sound\": \"";
  message += config.input3sound;
  message += "\", \"input4sound\": \"";
  message += config.input4sound;
    
  message += "\"}";
  return message;  
}

// HTTP request to get the current settings, called when the settings page is loaded
void handleGetSettingsRequest() {
  String message;
  message = ConfigToString();
  Serial.println("Sending config json");
  Serial.println(message);
  server.send(200, "application/json", message);
}

// Saves the configuration to a file
bool saveConfiguration(const char *filename, const Config &config) {
  bool result = false;
  // Open file for writing
  File configfile = SPIFFS.open(filename, FILE_WRITE);
  if (!configfile) {
    Serial.println(F("Failed to create file"));
    return result;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<800> doc;

  result = true;

  // Set the values in the document
  
  doc["loconame"] = config.loconame;
  doc["wifimode"] = config.wifimode;
  doc["ssid"] = config.ssid;
  doc["password"] = config.password;
  doc["acceleration"] = config.acceleration;
  doc["deceleration"] = config.deceleration;
  doc["minspeed"] = config.minspeed;
  doc["maxspeed"] = config.maxspeed;
  doc["volume"] = config.volume;
  doc["audio1"] = config.audio1;
  doc["audio2"] = config.audio2;
  doc["audio3"] = config.audio3;
  doc["audio4"] = config.audio4;
  doc["light3"] = config.light3;
  doc["light4"] = config.light4;
  doc["stationwait"] = config.stationwait;
  doc["blindtime"] = config.blindtime;
  doc["stationstopsound"] = config.stationstopsound;
  doc["stationleavesound1"] = config.stationleavesound1;
  doc["stationleavesound2"] = config.stationleavesound2;
  doc["terminusstopsound"] = config.terminusstopsound;
  doc["terminusleavesound1"] = config.terminusleavesound1;
  doc["terminusleavesound2"] = config.terminusleavesound2;
  doc["input3sound"] = config.input3sound;
  doc["input4sound"] = config.input4sound;

  // Serialize JSON to file
  if (serializeJson(doc, configfile) == 0) {
    Serial.println(F("Failed to write to file"));
    result = false;
  }

  // Close the file
  configfile.close();
  return result;
}

// Internal stats, not really used for anything now
void refreshStats() {
  char temp[5];
  webStat = "RSSI: ";
  webStat += WiFi.RSSI();
  webStat += "<br/>";
  webStat += "Uptime [min]: ";
  webStat += uptime;
  webStat += "<br/>";
  webStat += "Message count: ";
  webStat += msgcount;
  webStat += "<br/>";
}

// HTTP request to pass the new settings from the settings page back to the ESP to store the new settings
void handleUpdateSettingsRequest() {
  if (server.hasArg("plain")== false){ //Check if body received
    server.send(200, "text/plain", "Body not received");
    return;
  }
  String message = "";
  message += server.arg("plain");
  Serial.print("Configuraiton update received: ");
  Serial.println(message);
  if (loadConfiguration("", config, message)) {
    if (saveConfiguration(filename, config)) {
      server.send(200, "text/plain", "OK");
    } else {
      server.send(200, "text/plain", "Saving the configuration failed");
    }
  } else {
    server.send(200, "text/plain", "Processing the update failed");
  }
}

// HTTP request to change the target speed (slider or the Stop 1/3, 1/3, Full buttons
void handleSpeedRequest() {
  String message = "";
  
  if (server.arg("speed")== "") {    
    message = "Speed data is missing";
  }
  if (message=="") {
    message = server.arg("speed");
    targetspeed = message.toInt();
    message = "Speed " + message + " set";    
  }
  // Send dummy response
  Serial.println(message);
  server.send(200, "text/html", message);
}

// HTTP request to turn the shuttle mode on and off
void handleShuttleRequest() {
  String message = "Shuttle mode ";
  
  shuttlemode = !shuttlemode;

  message += (shuttlemode) ? "On" : "Off";

  // Send dummy response
  Serial.println(message);
  server.send(200, "text/html", message);
}

// HTTP request to change the direction
void handleDirectionRequest() {
  String message = "";
  
  if (server.arg("direction")== "") {    
    message = "Speed data is missing";
  }
  if (message=="") {
    message = server.arg("direction");
    direction = message.toInt();
    speed = 0;
    targetspeed = 0;
    message = "Direction set to " + message + ", speed 0";    
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    SetHeadLights();
  }
  // Send dummy response
  Serial.println(message);
  server.send(200, "text/html", message);
}

// Setting the PWM signal for the motor driver based on the current speed and direction
void setMotorSpeed() {
  if (speed==0) {
    motorspeed = 0;
  } else {
    motorspeed = config.minspeed + (config.maxspeed-config.minspeed)*speed/100;
  }
  if (direction==0) {
    // going forward
    ledcWrite(0, motorspeed);
  } else {
    // going backward
    ledcWrite(1, motorspeed);
  }
}

// HTTP call from the main webpage every second, which updates the screen controls/labels
void handleStatusRequest() {
  String message = "{";
  message += "\"direction\": ";
  message += direction;
  message += ", \"speed\": ";
  message += speed;
  message += ", \"targetspeed\": ";
  message += targetspeed;
  message += ", \"motorspeed\": ";
  message += motorspeed;
  message += ", \"uptime\": ";
  message += uptime;
  message += ", \"battery\": ";
  message += battery;
  message += ", \"drivemode\": ";
  message += drivemode;
  message += ", \"stationwait\": ";
  message += stationwait;
  message += ", \"shuttlemode\": ";
  message += shuttlemode;
  message += "}";
  server.send(200, "text/html", message);
}

// Central audio selection/playing routine
void PlayWAV(const char* filename) {
  if (strcmp(filename,"slot1")==0) filename = "/01.wav";
  if (strcmp(filename,"slot2")==0) filename = "/02.wav";
  if (strcmp(filename,"slot3")==0) filename = "/03.wav";
  if (strcmp(filename,"slot4")==0) filename = "/04.wav";
  if (strcmp(filename,"slot5")==0) filename = "/05.wav";
  if (strcmp(filename,"slot6")==0) filename = "/06.wav";
  if (strcmp(filename,"slot7")==0) filename = "/07.wav";
  if (strcmp(filename,"slot8")==0) filename = "/08.wav";

  if (wav->isRunning()) {
    wav->stop();
    //delete file;
  }
  Serial.print("Playing audio: ");
  Serial.println(filename);
  file = new AudioFileSourceSPIFFS(filename);
  wav->begin(file, out);
}

// HTTP request to play and audio
void handleSoundRequest() {
  String message = "";
  
  if (server.arg("sound")== "") {    
    message = "Sound data is missing";
  }
  if (message=="") {
    message = server.arg("sound");
    int sound = message.toInt();
    switch (sound) {
      case 0: 
        PlayWAV("/01.wav");
        break;
      case 1: 
        PlayWAV("/02.wav");
        break;
      case 2: 
        PlayWAV("/03.wav");
        break;
      case 3: 
        PlayWAV("/04.wav");
        break;
    }
    message = "Playing sound " + message;    
  }
  // Send dummy response
  Serial.println(message);
  server.send(200, "text/html", message);
}

// Turn on lights on outpu1 and output2 based on the direction
void SetHeadLights() {
  if (direction==0) {
    digitalWrite(LIGHT1, lightState);
    digitalWrite(LIGHT2, LOW);
  } else {
    digitalWrite(LIGHT2, lightState);
    digitalWrite(LIGHT1, LOW);
  }
}

// HTTP request to turn on/off lights
void handleLightRequest() {
  String message = "";
  
  if (server.arg("id")== "") {    
    message = "Light id is missing";
  }
  if (message=="") {
    message = server.arg("id");
    int light = message.toInt();
    switch (light) {
      case 0: 
        lightState = !lightState;
        SetHeadLights();
        break;
      case 3:
        light3State = !light3State;
        digitalWrite(LIGHT3, light3State);
        break;
      case 4:
        light4State = !light4State;
        digitalWrite(LIGHT4, light4State);
        break;
    }
    message = "Switching light " + message;    
  }
  // Send dummy response
  Serial.println(message);
  server.send(200, "text/html", message);
}

// Interrupt Service Routine for input 1 (station magnet)
void IRAM_ATTR Input1Handler() {
  if (millis() - lastInput1 >= INPUTDEBOUNCE) {
    lastInput1 = millis();
    Serial.println("Input 1 triggered");
    if ((drivemode==DRIVEMODE_DRIVING)&&(shuttlemode)&&(blindtime==0)) {
      // Station magnet detected, slowing down, and set the drivemode to station break
      autospeed = targetspeed;
      targetspeed = 0;
      drivemode=DRIVEMODE_STATIONBREAK;
      Serial.println("Stopping at a station...");
    }
  }
}

// Interrupt Service Routine for input 2 (terminus magnet)
void IRAM_ATTR Input2Handler() {
  if (millis() - lastInput2 >= INPUTDEBOUNCE) {
    lastInput2 = millis();
    Serial.println("Input 2 triggered");
    if ((drivemode==DRIVEMODE_DRIVING)&&(shuttlemode)&&(blindtime==0)) {
      // Terminus magnet detected, slowing down, and set the drivemode to terminus break
      autospeed = targetspeed;
      targetspeed = 0;
      drivemode=DRIVEMODE_TERMINUSBREAK;
      Serial.println("Stopping at a terminus...");
    }
  }
}

// Interrupt Service Routine for input 3 (audio trigger)
void IRAM_ATTR Input3Handler() {
  if (millis() - lastInput3 >= INPUTDEBOUNCE) {
    lastInput3 = millis();
    Serial.println("Input 3 triggered ");
    // Play the sound if configured
    if (strcmp(config.input3sound,"none")!=0) {
      triggersound = true;
      strncpy(startsound,config.input3sound,6);
    }
  }
}

// Interrupt Service Routine for input 4 (audio trigger)
void IRAM_ATTR Input4Handler() {
  if (millis() - lastInput4 >= INPUTDEBOUNCE) {
    lastInput4 = millis();
    Serial.println("Input 4 triggered ");
    // Play the sound if configured
    if (strcmp(config.input4sound,"none")!=0) {
      triggersound = true;
      strncpy(startsound,config.input4sound,6);
    }
  }
}



// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//                                 SETUP
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void setup() {
  Serial.begin(115200);
  Serial.println("LocoRemote");
  
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(MOTORIN1, OUTPUT);
  pinMode(MOTORIN2, OUTPUT);
  pinMode(LIGHT1, OUTPUT);
  pinMode(LIGHT2, OUTPUT);
  pinMode(LIGHT3, OUTPUT);
  pinMode(LIGHT4, OUTPUT);
  pinMode(INPUT1, INPUT_PULLUP);
  pinMode(INPUT2, INPUT_PULLUP);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  ledcAttachPin(MOTORIN1,0);  // assign the speed control PWM pin to a channel
  ledcAttachPin(MOTORIN2,1);  // assign the speed control PWM pin to a channel
   
  // initialize channel 0-15, resolution 1-16 bit, frequency limit depend on resolution
  ledcSetup(0,PWMFREQ,8);  // channel- 0, frequency- 4000, Resolution bit- 8
  ledcSetup(1,PWMFREQ,8);  // channel- 0, frequency- 4000, Resolution bit- 8
   
  uptime = 0;
  msgcount = 0;

  // Loading the audio objects
  audioLogger = &Serial;
  out = new AudioOutputI2S();
  wav = new AudioGeneratorWAV();
  out->SetGain(0.2);


  if(!SPIFFS.begin(true)){
     Serial.println("An Error has occurred while mounting SPIFFS");
  }
  // List all the files in the SPPIFS
  Serial.println("===== SPIFFS file list ======");
  File root = SPIFFS.open("/");
  File filelister = root.openNextFile();
  while(filelister){
   
        Serial.print("FILE: ");
        Serial.println(filelister.name());
   
        filelister = root.openNextFile();
  }  
  // End of SPIFFS list
  loadConfiguration(filename, config,"");

  delay(100);
  Serial.println();
  Serial.print("Connecting to wifi network");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(BUILTIN_LED, uptime % 2 == 0);
    uptime++;
  }
  uptime = 0;
  digitalWrite(BUILTIN_LED, LOW);

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal [RSSI]: ");
  Serial.println(WiFi.RSSI());

/*
  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp32.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin("locoremote")) {
      Serial.println("Error setting up MDNS responder!");
      while(1) {
          delay(1000);
      }
  }
  Serial.println("mDNS responder started");
  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
*/
  
  server.on("/", [](){                              // main page
     String s = webPage; //Read HTML contents
     server.send(200, "text/html", s); //Send web page
  });
  server.on("/config", [](){                        // config page
     String s = configPage; //Read HTML contents
     server.send(200, "text/html", s); //Send config page
  });
  server.on("/setspeed", handleSpeedRequest);              
  server.on("/setdirection", handleDirectionRequest);              
  server.on("/play", handleSoundRequest);              
  server.on("/light", handleLightRequest);              
  server.on("/status", handleStatusRequest);              
  server.on("/shuttle", handleShuttleRequest);              
  server.on("/getsettings", handleGetSettingsRequest);              
  server.on("/updatesettings", handleUpdateSettingsRequest);              
   
  server.begin();
  Serial.println("HTTP server started");

  refreshStats();


    //Setup the task for the audio playing
    xTaskCreatePinnedToCore(
      codeForAudioTask,            /* Task function. */
      "AudioTask",                 /* name of task. */
      10240,                    /* Stack size of task - this needed to be enhanced */
      NULL,                     /* parameter of the task */
      1,                        /* priority of the task */
      &AudioTask,                   /* Task handle to keep track of created task */
      1);                       /* Core */ 


  // Configure the input interrupts
  attachInterrupt(INPUT1, Input1Handler, FALLING);
  attachInterrupt(INPUT2, Input2Handler, FALLING);
  attachInterrupt(INPUT3, Input3Handler, FALLING);
  attachInterrupt(INPUT4, Input4Handler, FALLING);

}

// Audio thread handler which is responsible for keeping the audio buffer topped up when the audio is playing
void codeForAudioTask( void * parameter ) {

  for (;;) {
    TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed=1;
    TIMERG0.wdt_wprotect=0;

    if (wav->isRunning()) {
      if (!wav->loop()) wav->stop();
    } else {
      //Serial.printf("MP3 done\n");
      //delay(1000);
    }

    //vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//                                 LOOP
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void loop() {


 
  // Handle HTTP server requests
  server.handleClient();

  // play a sound triggered by the interrupt
  // this was required to keep the ISR short and not short dump due to WDT
  if (triggersound) {
    if (!wav->isRunning()) {
      PlayWAV(startsound);
    }
    triggersound = false;
  }

  // Acceleration and Deceleration
  //if ((drivemode==DRIVEMODE_MANUAL)||(drivemode==DRIVEMODE_STATIONBREAK)) {
    if (speed!=targetspeed) {
      if (speed>targetspeed) {
        if (millis()-lastSpeed>config.deceleration) {
          speed--;
          setMotorSpeed();
          lastSpeed = millis();
        }
      } else {
        if (millis()-lastSpeed>config.acceleration) {
          speed++;
          setMotorSpeed();
          lastSpeed = millis();
        }      
      }
    }
  //}
  
  // Stopped at the station
  if (drivemode==DRIVEMODE_STATIONBREAK) {
    if (speed==0) {
      drivemode=DRIVEMODE_STATIONSTOP;
      stationwait = config.stationwait;
      Serial.println("Stopped at a station");
      if (strcmp(config.stationstopsound,"none")!=0) {
        PlayWAV(config.stationstopsound);
      }
    }
  }

  // At the station
  if (drivemode==DRIVEMODE_STATIONSTOP) {
    // Time to play the sound 1
    if ((stationwait==10)&&(strcmp(config.stationleavesound1,"none")!=0)&&(!wav->isRunning())) {
      PlayWAV(config.stationleavesound1);
    }
    // Time to leave the station
    if (stationwait==0) {
      // Play sound 2
      if (strcmp(config.stationleavesound2,"none")!=0) {
        PlayWAV(config.stationleavesound2);
      }
      drivemode=DRIVEMODE_DRIVING;
      targetspeed = autospeed;
      blindtime = config.blindtime;
      autospeed = 0;
      Serial.println("Leaving the station");
    }
  }

  // Stopped at the terminus
  if (drivemode==DRIVEMODE_TERMINUSBREAK) {
    if (speed==0) {
      drivemode=DRIVEMODE_TERMINUSSTOP;
      stationwait = config.stationwait;
      Serial.println("Stopped at a terminus");
      if (strcmp(config.terminusstopsound,"none")!=0) {
        PlayWAV(config.terminusstopsound);
      }
    }
  }

  // At the terminus
  if (drivemode==DRIVEMODE_TERMINUSSTOP) {
    // Time to play the sound 1
    if ((stationwait==10)&&(strcmp(config.terminusleavesound1,"none")!=0)&&(!wav->isRunning())) {
      PlayWAV(config.terminusleavesound1);
    }
    // Time to leave the terminus
    if (stationwait==0) {
      // Play sound 2
      if (strcmp(config.terminusleavesound2,"none")!=0) {
        PlayWAV(config.terminusleavesound2);
      }
      if (direction==0) {
        direction=1;
      } else {
        direction=0;
      }
      SetHeadLights();
      drivemode=DRIVEMODE_DRIVING;
      targetspeed = autospeed;
      blindtime = config.blindtime;
      autospeed = 0;
      Serial.println("Leaving the terminus");
    }
  }

  // Uptime calculation (every minute)
  if (millis() - lastMinute >= 60000) {            
    lastMinute = millis();            
    uptime++;            
  }   

  // second calculation
  if (millis() - lastSecond >= 1000) {            
    lastSecond = millis();            
    sec++;   
    if (sec%10==0) {
      refreshStats();  
    }
    // Battery level check
    battery = (float)analogRead(BATTERYMONITOR) / 4096 * 33 * 11111 / 11000 * 1.1;

    //Battery average calculation - average the last 10 readings
    if (battarrcount<BATTARRSIZE-1) {
      battarr[battarrcount] = battery;
      battarrcount++;
    } else {
      for(int i=1; i<BATTARRSIZE; i++) {
        battarr[i-1]=battarr[i];
      }
      battarr[BATTARRSIZE-1]=battery;
    }
    battery = 0;
    for(int i=0; i<battarrcount; i++) {
      battery += battarr[i];
    }
    battery = battery / battarrcount;

    // If we are waiting at the station, decrement the counter
    if (stationwait>0) {
      stationwait--;
      Serial.print("Wait time left: ");
      Serial.println(stationwait);
    }
    // count down the blindtime
    if (blindtime>0) blindtime--;
    
    // reconnect to the wifi network in station mode if connection is lost
    if (config.wifimode=="sta")&&(WiFi.status() != WL_CONNECTED)) {
      Serial.println("Reconnecting to wifi...");
      WiFi.reconnect();
    }
  }   

}
