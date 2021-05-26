const char configPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>LocoRemote</title>
<style>
  body {
      background: #fff;
      color: #333;
      font-family: "Open Sans",sans-serif;
      font-style: normal;
      line-height: 1.5;
  }

  .title {
      color: #333;
      text-align: left;
      font-size:2.5rem;
      font-style: bold;
  }
  
  .group {
      color: #333;
      text-align: left;
      font-size:2rem;
      font-style: italic;
      border-bottom: 1px solid #e2001a;
  }

  .option {
      color: #333;
      text-align: right;
      margin: 0;
      font-size:1rem;
  }

  .help {
      color: #e2001a;
      text-align: right;
      margin: 0;
      font-size:0.8rem;

  }

  input {
      font-size: 2rem;
  }

  select {
      font-size: 2rem;
  }
  
  button {
    border-radius: 0;
    border-style: solid;
    border-width: 0;
    cursor: pointer;
    font-family: "Open Sans",sans-serif;
    font-size: 3rem;
    line-height: normal;
    margin: 0.5rem 0.5rem 0.5rem 0.5rem;
    position: relative;
    text-align: left;
    text-decoration: none;
    display: inline-block;
    padding: 1rem 1rem 1rem 1rem;
    background-color: #e2001a;
    border-color: #ccc;
    color: #fff;
}


.loading {
    width: 100%;
    height: 100%;
    position: fixed;
    top: 0;
    right: 0;
    bottom: 0;
    left: 0;
    background-color: rgba(0,0,0,.5);
}
.loading-wheel {
    width: 5rem;
    height: 5rem;
    margin-top: -40px;
    margin-left: -40px;
    
    position: absolute;
    top: 50%;
    left: 50%;
    
    border-width: 2rem;
    border-radius: 50%;
    -webkit-animation: spin 1s linear infinite;
}
.style-2 .loading-wheel {
    border-style: double;
    border-color: #ccc transparent;
}
@-webkit-keyframes spin {
    0% {
        -webkit-transform: rotate(0);
    }
    100% {
        -webkit-transform: rotate(-360deg);
    }
}
  
</style>  
</head>
<body onload="LoadSettings()">
<script>

function UpdateSettings() {

  var xhr = new XMLHttpRequest();
  xhr.open("POST", "updatesettings", true);
  xhr.setRequestHeader("Content-Type", "application/json");
  xhr.onreadystatechange = function () {
    if (xhr.readyState == 4 && xhr.status == 200) {
      if (xhr.responseText=="OK") {
        alert("Configuration updated!");
      } else {
        alert("Configuration update failed!");
      }
    }
  };
  var data = JSON.stringify( {
    "loconame": document.getElementById("loconame").value, 
    "wifimode": document.getElementById("wifimode").value,
    "ssid": document.getElementById("ssid").value,
    "password": document.getElementById("password").value,
    "acceleration": document.getElementById("acceleration").value,
    "deceleration": document.getElementById("deceleration").value,
    "minspeed": document.getElementById("minspeed").value,
    "maxspeed": document.getElementById("maxspeed").value,
    "audio1": document.getElementById("audio1").value,
    "audio2": document.getElementById("audio2").value,
    "audio3": document.getElementById("audio3").value,
    "audio4": document.getElementById("audio4").value,
    "light3": document.getElementById("light3").value,
    "light4": document.getElementById("light4").value,
    "volume": document.getElementById("volume").value,
    "stationwait": document.getElementById("stationwait").value,
    "blindtime": document.getElementById("blindtime").value,
    "stationstopsound": document.getElementById("stationstopsound").value,
    "stationleavesound1": document.getElementById("stationleavesound1").value,
    "stationleavesound2": document.getElementById("stationleavesound2").value,
    "terminusstopsound": document.getElementById("terminusstopsound").value,
    "terminusleavesound1": document.getElementById("terminusleavesound1").value,
    "terminusleavesound2": document.getElementById("terminusleavesound2").value,
    "input3sound": document.getElementById("input3sound").value,
    "input4sound": document.getElementById("input4sound").value
    });
  xhr.send(data);

}

function LoadSettings() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myarr = JSON.parse(this.responseText);
      document.getElementById("loconame").value = myarr.loconame;
      document.getElementById("wifimode").value = myarr.wifimode;
      document.getElementById("ssid").value = myarr.ssid;
      document.getElementById("password").value = myarr.password;
      document.getElementById("acceleration").value = myarr.acceleration;
      document.getElementById("deceleration").value = myarr.deceleration;
      document.getElementById("minspeed").value = myarr.minspeed;
      document.getElementById("maxspeed").value = myarr.maxspeed;
      document.getElementById("audio1").value = myarr.audio1;
      document.getElementById("audio2").value = myarr.audio2;
      document.getElementById("audio3").value = myarr.audio3;
      document.getElementById("audio4").value = myarr.audio4;
      document.getElementById("light3").value = myarr.light3;
      document.getElementById("light4").value = myarr.light4;
      document.getElementById("volume").value = myarr.volume;
      document.getElementById("stationwait").value = myarr.stationwait;
      document.getElementById("blindtime").value = myarr.blindtime;
      document.getElementById("stationstopsound").value = myarr.stationstopsound;
      document.getElementById("stationleavesound1").value = myarr.stationleavesound1;
      document.getElementById("stationleavesound2").value = myarr.stationleavesound2;
      document.getElementById("terminusstopsound").value = myarr.terminusstopsound;
      document.getElementById("terminusleavesound1").value = myarr.terminusleavesound1;
      document.getElementById("terminusleavesound2").value = myarr.terminusleavesound2;
      document.getElementById("input3sound").value = myarr.input3sound;
      document.getElementById("input4sound").value = myarr.input4sound;

      // hide the progress div
      document.getElementById("loading2").style.display = 'none';
      document.getElementById("loading1").style.display = 'none';
    }
  };
  xhttp.open("GET", "getsettings", true);
  xhttp.send(); 
}



</script>



<table border="0" cellspacing="0" cellpadding="10" align="center">
  <tr>
    <td class="title" colspan="2">
        LocoRemote Settings
    </td>
  </tr>

  <tr>
    <td class="group" colspan="2">
        General Settings
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Loco Name:</p>
        <p class="help">Name to appear on the top of the page</p>
    <td style="vertical-align:top"><input type="text" id="loconame" name="loconame" value="aa"></td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Wifi mode:</p>
        <p class="help">Controls how the wifi is used on the LocoRemote<br/>
            - Access Point: LocoRemote creates its own wifi network that a phone or table can connect to. This mode is ideal for outdoor use where there is no wifi network otherwise.<br/>
            - Station: LocoRemote connects to an existing wifi network. This could be used for indoor or exhibition layouts and would allow multiple locos to be controller from a single device, by switching between browser screens.<br/>
            The same SSID and password is used both for station and access point mode.
        </p>
    <td style="vertical-align:top">
        <select id="wifimode" name="wifimode" value="ap">
          <option value="ap">Access Point</option>
          <option value="sta">Station</option>
        </select>
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Wifi SSID:</p>
        <p class="help">Name of the wifi network</p>
    <td style="vertical-align:top"><input type="text" id="ssid" name="ssid" value="aa"></td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Wifi Password:</p>
        <p class="help">Password for the wifi network</p>
    <td style="vertical-align:top"><input type="text" id="password" name="password" value="aa"></td>
  </tr>

  <tr>
    <td class="group" colspan="2">
        Motor Settings
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Acceleration:</p>
        <p class="help">Delay between each speed step change in miliseconds. Smaller value faster acceleration, lower value smaller acceleration. There are 100 speed steps, so acceleration of 100 means standstill to full speed is 10 seconds, 200 will be 20 seconds.</p>
    <td style="vertical-align:top"><input type="text" id="acceleration" name="acceleration" value="1"></td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Deceleration:</p>
        <p class="help">Delay between each speed step change in miliseconds. Smaller value faster deceleration, lower value smaller deceleration. There are 100 speed steps, so deceleration of 100 means full speed to standstill is 10 seconds, 200 will be 20 seconds.</p>
    <td style="vertical-align:top"><input type="text" id="deceleration" name="deceleration" value="1"></td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Minimum speed:</p>
        <p class="help">Minimum speed value, in case the motor does not start the loco at very low speeds. This field accepts values between 0 and 255.</p>
    <td style="vertical-align:top"><input type="text" id="minspeed" name="minspeed" value="0"></td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Maximum speed:</p>
        <p class="help">Maximum speed value. This field accepts values between 0 and 255.</p>
    <td style="vertical-align:top"><input type="text" id="maxspeed" name="maxspeed" value="255"></td>
  </tr>

  <tr>
    <td class="group" colspan="2">
        Audio Settings
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Volume:</p>
        <p class="help">Setting the gain on the amplifier. Use value from 0 to 40.</p>
    <td style="vertical-align:top"><input type="text" id="volume" name="volume" value="aa"></td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Audio Slot 1:</p>
        <p class="help">Name of the first audio, that will appear on the audio button (e.g. 'Horn', 'Bell', 'Whistle'). Keep the name short</p>
    <td style="vertical-align:top"><input type="text" id="audio1" name="audio1" value="aa"></td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Audio Slot 2:</p>
        <p class="help">Same as above</p>
    <td style="vertical-align:top"><input type="text" id="audio2" name="audio2" value="aa"></td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Audio Slot 3:</p>
        <p class="help">Same as above</p>
    <td style="vertical-align:top"><input type="text" id="audio3" name="audio3" value="aa"></td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Audio Slot 4:</p>
        <p class="help">Same as above</p>
    <td style="vertical-align:top"><input type="text" id="audio4" name="audio4" value="aa"></td>
  </tr>


  <tr>
    <td class="group" colspan="2">
        Light Settings
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">AUX3 name:</p>
        <p class="help">Name of the AUX3 button, that will appear on the button (e.g. 'Cab light', 'Bell', 'Whistle'). Keep the name short</p>
    <td style="vertical-align:top"><input type="text" id="light3" name="light3" value="aa"></td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">AUX4 name:</p>
        <p class="help">Name of the AUX4 button, that will appear on the button (e.g. 'Cab light', 'Bell', 'Whistle'). Keep the name short</p>
    <td style="vertical-align:top"><input type="text" id="light4" name="light4" value="aa"></td>
  </tr>

  <tr>
    <td class="group" colspan="2">
        Inputs and Shuttle Mode
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Station wait time:</p>
        <p class="help">This is used in the shuttle mode to define how many seconds the loco waits on the station or terminus before starting again.</p>
    <td style="vertical-align:top"><input type="text" id="stationwait" name="stationwait" value="60"></td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Sensor blind time:</p>
        <p class="help">This is used in the shuttle mode to define how many seconds Input 1 and Input 2 are ignoring magnet detection. This is to ensure the stop magnet does not stop the engine when it is leaving the station. This should be the time it takes for the loco and accelerate and pass the stop magnet.</p>
    <td style="vertical-align:top"><input type="text" id="blindtime" name="blindtime" value="30"></td>
  </tr>
  
  <tr>
    <td style="vertical-align:top">
        <p class="option">Station stop sound:</p>
        <p class="help">The audio to be played when the loco comes to a stop at the station in shuttle mode. This is when the loco stops after the station magnet is detected by Input 1 in shuttle mode. This can be an station announcement like "Welcome to...".</p>
    <td style="vertical-align:top">
        <select id="stationstopsound" name="stationstopsound" value="none">
          <option value="none">No sound</option>
          <option value="slot1">Sound slot 1</option>
          <option value="slot2">Sound slot 2</option>
          <option value="slot3">Sound slot 3</option>
          <option value="slot4">Sound slot 4</option>
          <option value="slot5">Sound slot 5</option>
          <option value="slot6">Sound slot 6</option>
          <option value="slot7">Sound slot 7</option>
          <option value="slot8">Sound slot 8</option>
        </select>
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Station leave sound 1:</p>
        <p class="help">The audio to be played when the loco is about to leave the station. This only applies in Shuttle mode. This could be a guards whistle or "All aboard" announcement.</p>
    <td style="vertical-align:top">
        <select id="stationleavesound1" name="stationleavesound1" value="none">
          <option value="none">No sound</option>
          <option value="slot1">Sound slot 1</option>
          <option value="slot2">Sound slot 2</option>
          <option value="slot3">Sound slot 3</option>
          <option value="slot4">Sound slot 4</option>
          <option value="slot5">Sound slot 5</option>
          <option value="slot6">Sound slot 6</option>
          <option value="slot7">Sound slot 7</option>
          <option value="slot8">Sound slot 8</option>
        </select>
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Station leave sound 2:</p>
        <p class="help">The audio to be played just before the loco leaves the station. This only applies in Shuttle mode. This could be short whistle.</p>
    <td style="vertical-align:top">
        <select id="stationleavesound2" name="stationleavesound2" value="none">
          <option value="none">No sound</option>
          <option value="slot1">Sound slot 1</option>
          <option value="slot2">Sound slot 2</option>
          <option value="slot3">Sound slot 3</option>
          <option value="slot4">Sound slot 4</option>
          <option value="slot5">Sound slot 5</option>
          <option value="slot6">Sound slot 6</option>
          <option value="slot7">Sound slot 7</option>
          <option value="slot8">Sound slot 8</option>
        </select>
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Terminus stop sound:</p>
        <p class="help">The audio to be played when the loco comes to a stop at the terminus in shuttle mode. This is when the loco stops after the terminus magnet is detected by Input 2 in shuttle mode. This can be an station announcement like "Service terminates here, all change!".</p>
    <td style="vertical-align:top">
        <select id="terminusstopsound" name="terminusstopsound" value="none">
          <option value="none">No sound</option>
          <option value="slot1">Sound slot 1</option>
          <option value="slot2">Sound slot 2</option>
          <option value="slot3">Sound slot 3</option>
          <option value="slot4">Sound slot 4</option>
          <option value="slot5">Sound slot 5</option>
          <option value="slot6">Sound slot 6</option>
          <option value="slot7">Sound slot 7</option>
          <option value="slot8">Sound slot 8</option>
        </select>
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Terminus leave sound 1:</p>
        <p class="help">The audio to be played when the loco is about to leave the terminus. This only applies in Shuttle mode. This could be a guards whistle or "All aboard" announcement.</p>
    <td style="vertical-align:top">
        <select id="terminusleavesound1" name="terminusleavesound1" value="none">
          <option value="none">No sound</option>
          <option value="slot1">Sound slot 1</option>
          <option value="slot2">Sound slot 2</option>
          <option value="slot3">Sound slot 3</option>
          <option value="slot4">Sound slot 4</option>
          <option value="slot5">Sound slot 5</option>
          <option value="slot6">Sound slot 6</option>
          <option value="slot7">Sound slot 7</option>
          <option value="slot8">Sound slot 8</option>
        </select>
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Terminus leave sound 2:</p>
        <p class="help">The audio to be played just before the loco leaves the terminus. This only applies in Shuttle mode. This could be short whistle.</p>
    <td style="vertical-align:top">
        <select id="terminusleavesound2" name="terminusleavesound2" value="none">
          <option value="none">No sound</option>
          <option value="slot1">Sound slot 1</option>
          <option value="slot2">Sound slot 2</option>
          <option value="slot3">Sound slot 3</option>
          <option value="slot4">Sound slot 4</option>
          <option value="slot5">Sound slot 5</option>
          <option value="slot6">Sound slot 6</option>
          <option value="slot7">Sound slot 7</option>
          <option value="slot8">Sound slot 8</option>
        </select>
    </td>
  </tr>
  
  <tr>
    <td style="vertical-align:top">
        <p class="option">Input 3 sound trigger:</p>
        <p class="help">The audio to be played when input 3 triggers (when a magnet is detected at input 3)</p>
    <td style="vertical-align:top">
        <select id="input3sound" name="input3sound" value="none">
          <option value="none">No sound</option>
          <option value="slot1">Sound slot 1</option>
          <option value="slot2">Sound slot 2</option>
          <option value="slot3">Sound slot 3</option>
          <option value="slot4">Sound slot 4</option>
          <option value="slot5">Sound slot 5</option>
          <option value="slot6">Sound slot 6</option>
          <option value="slot7">Sound slot 7</option>
          <option value="slot8">Sound slot 8</option>
        </select>
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
        <p class="option">Input 4 sound trigger:</p>
        <p class="help">The audio to be played when input 4 triggers (when a magnet is detected at input 4)</p>
    <td style="vertical-align:top">
        <select id="input4sound" name="input4sound" value="none">
          <option value="none">No sound</option>
          <option value="slot1">Sound slot 1</option>
          <option value="slot2">Sound slot 2</option>
          <option value="slot3">Sound slot 3</option>
          <option value="slot4">Sound slot 4</option>
          <option value="slot5">Sound slot 5</option>
          <option value="slot6">Sound slot 6</option>
          <option value="slot7">Sound slot 7</option>
          <option value="slot8">Sound slot 8</option>
        </select>
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
    </td>
    <td style="vertical-align:top">
      <button onclick="UpdateSettings()" class="button">Save Settings</button>
    </td>
  </tr>

  <tr>
    <td style="vertical-align:top">
    </td>
    <td style="vertical-align:top">
      <button onclick="location.href='/'" type="button">Back</button>
    </td>
  </tr>
  
</table>


<div class="loading style-2" id="loading1"><div class="loading-wheel" id="loading2"></div></div>

</body>
</html>
)=====";
