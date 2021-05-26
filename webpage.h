const char webPage[] PROGMEM = R"=====(
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
  }

  h1 {
      color: #e2001a;
      font-size: 5rem;
  }

  .smaller {
      font-size: 1rem;
      color: #08088A;
  }

  .battery {
      font-size: 2rem;
  }



  .button_active {
    border-radius: 0;
    border-style: solid;
    border-width: 0;
    cursor: pointer;
    font-family: "Open Sans",sans-serif;
    font-size: 5rem;
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

  .button_inactive {
    border-radius: 0;
    border-style: solid;
    border-width: 0;
    cursor: pointer;
    font-family: "Open Sans",sans-serif;
    font-size: 5rem;
    line-height: normal;
    margin: 0.5rem 0.5rem 0.5rem 0.5rem;
    position: relative;
    text-align: left;
    text-decoration: none;
    display: inline-block;
    padding: 1rem 1rem 1rem 1rem;
    background-color: #e2001a;
    border-color: #ccc;
    color: #333;
  }

.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 4rem;
  border-radius: 2rem;  
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 8rem;
  height: 8rem;
  border-radius: 50%; 
  background: #e2001a;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 8rem;
  height: 8rem;
  border-radius: 50%;
  background: #e2001a;
  cursor: pointer;
}

.slidecontainer {
  width: 100%; /* Width of the outside container */
  padding-top: 1rem;
}

.shuttlemode {
  width: 50vw;
  height: 50vw;
  background: linear-gradient(to top right, white 50%, blue);
  position: fixed;
  top: 0;
  right: 0;
  z-index: -1;
  text-align: right;
}

.shuttleicon {
  color: #b0d2ff;
  font-size: 10rem;
  text-align: right;
  margin: 0.5rem 1rem 0 0;
  padding: 0 0 0 0;
}

progress {
  -webkit-appearance: progress-bar;
  -webkit-writing-mode: horizontal-tb;
  box-sizing: border-box;
  display: inline-block;
  height: 6rem;
  width: 100%;
}

progress::-webkit-progress-bar {
  background-color: #e2001a;
  border-radius: 1rem;
  box-shadow: 0 1rem 1rem rgba(0, 0, 0, 0.25) inset;
}

progress::-moz-progress-bar {
  background-color: #e2001a;
  border-radius: 1rem;
  box-shadow: 0 1rem 1rem rgba(0, 0, 0, 0.25) inset;
}

progress::-webkit-progress-value {background-color: #e2001a !important;}
progress::-moz-progress-bar {background-color: #e2001a !important;}

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
    width: 10rem;
    height: 10rem;
    margin-top: -40px;
    margin-left: -40px;
    
    position: absolute;
    top: 50%;
    left: 50%;
    
    border-width: 4rem;
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

var SettingsLoaded = false;
var success = false;
var tryno = 0;

function getStatus() {
  if (SettingsLoaded) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var myarr = JSON.parse(this.responseText);
        document.getElementById("direction").innerHTML = myarr.direction;
        document.getElementById("speed").innerHTML = myarr.speed + " (" + myarr.motorspeed + ")";
        document.getElementById("speedprogress").value = myarr.speed;
        document.getElementById("targetspeed").innerHTML = myarr.targetspeed;
        document.getElementById("slidespeed").value = myarr.targetspeed;
        document.getElementById("uptime").innerHTML = myarr.uptime;
        document.getElementById("battery").innerHTML = myarr.battery + " V";
        if (myarr.direction==0) {
          document.getElementById("forward").className = "button_inactive";
          document.getElementById("reverse").className = "button_active";
        } else {
          document.getElementById("forward").className = "button_active";
          document.getElementById("reverse").className = "button_inactive";          
        }
        switch (myarr.drivemode) {
          case 0:
            document.getElementById("shuttleicon").innerHTML = "&harr;&#xFE0E;";
            break;
          case 1:
            document.getElementById("shuttleicon").innerHTML = "&darr;&#xFE0E;";
            break;
          case 3:
            document.getElementById("shuttleicon").innerHTML = "&darr;&#xFE0E;";
            break;
          case 2:
            document.getElementById("shuttleicon").innerHTML = myarr.stationwait;
            break;
          case 4:
            document.getElementById("shuttleicon").innerHTML = myarr.stationwait;
            break;
        }
        document.getElementById("shuttlebutton").innerHTML = (myarr.shuttlemode) ? "&bigotimes;&#xFE0E; Shuttle" : "&bigodot;&#xFE0E; Shuttle"
        if (myarr.shuttlemode) {
          document.getElementById("shuttlediv").style.display = "block";
        } else {
          document.getElementById("shuttlediv").style.display = "none";
        }
      }
    };
    xhttp.open("GET", "status", true);
    xhttp.send();
  }
}
window.setInterval(getStatus, 1000);

function SendRequest(url) {
  success = false;
  tryno = 0;
  while ((!success)&&(tryno<3)) {
    var done = false;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4) { 
        if (this.status == 200) {
          document.getElementById("reply").innerHTML = this.responseText;
          success = true;
        }
        tryno++;
        done = true;
      }
    };
    xhttp.open("GET", url, true);
    xhttp.send();
    while (!done);
  }
}

function Speed(newspeed) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("reply").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "setspeed?speed=" + newspeed, true);
  xhttp.send();
}

function Direction(newdirection) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("reply").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "setdirection?direction=" + newdirection, true);
  xhttp.send();
}

function Sound(soundid) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("reply").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "play?sound=" + soundid, true);
  xhttp.send();
}

function Light(lightid) {
  //SendRequest("light?id=" + lightid);

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("reply").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "light?id=" + lightid, true);
  xhttp.send();
}

function Shuttle() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("reply").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "shuttle", true);
  xhttp.send();
}

function LoadSettings() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myarr = JSON.parse(this.responseText);
      document.getElementById("loconame").innerHTML = myarr.loconame;
      document.getElementById("audio1").innerHTML = myarr.audio1;
      document.getElementById("audio2").innerHTML = myarr.audio2;
      document.getElementById("audio3").innerHTML = myarr.audio3;
      document.getElementById("audio4").innerHTML = myarr.audio4;
      document.getElementById("light3").innerHTML = myarr.light3;
      document.getElementById("light4").innerHTML = myarr.light4;

      // hide the progress div
      document.getElementById("loading2").style.display = 'none';
      document.getElementById("loading1").style.display = 'none';
      SettingsLoaded = true;
    }
  };
  xhttp.open("GET", "getsettings", true);
  xhttp.send(); 
}


</script>

<div class="shuttlemode" id="shuttlediv"><p class="shuttleicon" id="shuttleicon"></p></div>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
  <tr>
    <td style="vertical-align:top; text-align: center" align="center"><h1 id="loconame">LocoRemote</h1>
      <p><button onclick="Direction(1)" class="button_active" id="reverse">Reverse</button>&nbsp;<button onclick="Direction(0)" class="button_active" id="forward">Forward</button></p>
      <p><button onclick="Speed(0)" class="button_active">Stop</button>&nbsp;<button onclick="Speed(30)" class="button_active">1/3</button>&nbsp;<button onclick="Speed(60)" class="button_active">2/3</button>&nbsp;<button onclick="Speed(100)" class="button_active">Full</button></p>
      <p class="slidecontainer"><input type="range" min="1" max="100" value="0" class="slider" id="slidespeed"></p>
      <p class="slidecontainer"><progress max="100" value="0" id="speedprogress"></progress></p>
      <p><button onclick="Sound(0)" class="button_active" id="audio1">Play Sound 1</button>&nbsp;<button onclick="Sound(1)" class="button_active" id="audio2">Play Sound 2</button>&nbsp;<button onclick="Sound(2)" class="button_active" id="audio3">Play Sound 3</button>&nbsp;<button onclick="Sound(3)" class="button_active" id="audio4">Play Sound 4</button></p>
      <p><button onclick="Light(0)" class="button_active">Lights</button>&nbsp;<button onclick="Light(3)" class="button_active" id="light3">AUX 3</button>&nbsp;<button onclick="Light(4)" class="button_active" id="light4">AUX 4</button></p>
      <p><button onclick="Shuttle()" class="button_active" id="shuttlebutton">Shuttle</button></p>
      <p class="battery">Battery: <span id="battery"></span></p>
      <p class="smaller">Wifi LocoRemote  v1.0 | <a href="mailto:csongor.varga@gmail.com">email me</a> | <a href="https://github.com/nygma2004/marklin_can_wifi">GitHub</a></p>
      <p class="smaller">Debug | Uptime: <span id="uptime"></span> | Direction: <span id="direction"></span> | Speed: <span id="speed"></span> | Target speed: <span id="targetspeed"></span> | Reply: <span id="reply"></span></p>
      <p><a href="/config">Settings</a></p>
    </td>
  </tr>
  
</table>

<script>
var slider = document.getElementById("slidespeed");

slider.oninput = function() {
  Speed(this.value);
}
</script>

<div class="loading style-2" id="loading1"><div class="loading-wheel" id="loading2"></div></div>

</body>
</html>
)=====";
