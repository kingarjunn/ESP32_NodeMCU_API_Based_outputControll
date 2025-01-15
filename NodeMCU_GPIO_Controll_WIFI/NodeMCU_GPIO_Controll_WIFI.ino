
// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <div>
        <h3>Connect</h3>
        <label class="switch">
            <input type="checkbox" id="myCheck" onclick="seturl()">
            <span class="slider round"></span>
          </label>
    </div>
    <title>NodeMCU</title>
    <script>
        // Function to call the API and handle the response
        const buttons = document.getElementsByTagName("button");
        const url = 'http://192.168.4.1/'
        var xurl=''
        function seturl()
        {
            var checkBox = document.getElementById("myCheck");
            if (checkBox.checked == true){
                xurl=url
            } else {
                xurl=''
            }
            var elements = document.querySelectorAll("button");   
            for(var i = 0, len = elements.length; i < len; i++) {   
                //console.log("Button: " +  elements[i].id);
                console.log("Button: " +  elements[i].id);
                //elements[i].innerHTML="ON"
                var b1 = document.getElementById(elements[i].id);
                b1.innerHTML="ON"
                b1.style.background='#388e3c'
            }
            
        }
        function enablebutton()
        {
            
            for (const button of buttons) {
                    button.disabled = false;
                    }
        }
        function disabledbutton(){
            
            for (const button of buttons) {
                    button.disabled = true;
                    }
        }
        function updatebutton(endpoint){
            var tx=document.getElementById(endpoint);
                if(tx.innerHTML=="ON"){
                    tx.innerHTML="OFF"
                    tx.style.backgroundColor="RED"
                }
                else
                {
                    tx.innerHTML="ON"
                    tx.style.backgroundColor="#388e3c"
                }
        }
        
        function callApi(endpoint) {
            if(xurl==url){
                var uri = xurl+endpoint + '?value='+document.getElementById(endpoint).innerHTML
                console.log(uri)
                document.getElementById(endpoint).style.backgroundColor='#2196F3'
                disabledbutton()
                fetch(uri)
                    .then(response => response.json())
                    .then(data => {
                        // Handle the response data here
                        console.log(data);
                        enablebutton();
                        updatebutton(endpoint);
                        //alert("Data received: " + JSON.stringify(data));
                    })
                    .catch(error => {
                        console.error("Error fetching data:", error);
                        //alert("Error: " + error.message);
                        enablebutton();
                        document.getElementById(endpoint).style.backgroundColor='#388e3c'
                    });
                    //updatebutton(endpoint)
                    
            }
            else
            {
                updatebutton(endpoint)
                //#388e3c
                
            }
        }
    </script>
</head>
<body>

    <div>
        <h1>Switch Controls</h1>

        <div class="button-container">
            <button id="b0" onclick="callApi('b0')">ON</button>
            <button id="b1" onclick="callApi('b1')">ON</button>
            <button id="b2" onclick="callApi('b2')">ON</button>
            <button id="b3" onclick="callApi('b3')">ON</button>
        </div>
    </div>

</body>
<style>
    /* Basic reset for margin and padding */
    * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
    }

    body {
        font-family: Arial, sans-serif;
        background-color: #f4f4f9;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        text-align: center;
        margin: 0;
    }

    h1 {
        color: #333;
        margin-bottom: 20px;
    }

    .button-container {
        display: flex;
        flex-direction: column;
        gap: 10px;
    }

    button {
        padding: 15px 30px;
        font-size: 16px;
        color: white;
        background-color: #4CAF50;
        border: none;
        border-radius: 5px;
        cursor: pointer;
        transition: background-color 0.3s;
    }

    button:hover {
        background-color: #45a049;
    }

    button:active {
        background-color: #388e3c;
    }

    button:focus {
        outline: none;
    }


.switch {
position:relative;
display: inline-block;
width: 60px;
height: 34px;
}

.switch input { 
opacity: 0;
width: 0;
height: 0;
}

.slider {
position: absolute;
cursor: pointer;
top: 0;
left: 0;
right: 0;
bottom: 0;
background-color: #ccc;
-webkit-transition: .4s;
transition: .4s;
}

.slider:before {
position: absolute;
content: "";
height: 26px;
width: 26px;
left: 4px;
bottom: 4px;
background-color: white;
-webkit-transition: .4s;
transition: .4s;
}

input:checked + .slider {
background-color: #2196F3;
}

input:focus + .slider {
box-shadow: 0 0 1px #2196F3;
}

input:checked + .slider:before {
-webkit-transform: translateX(26px);
-ms-transform: translateX(26px);
transform: translateX(26px);
}

/* Rounded sliders */
.slider.round {
border-radius: 34px;
}

.slider.round:before {
border-radius: 50%;
}

div{
    padding: 30px;
}

</style>
</html>


)rawliteral";


const char* ssid     = "Arjun-Access-Point";
const char* password = "123456789";
const int b0 = 4;
const int b1 = 5;
const int b2 = 12;
const int b3 = 13;

int b01=0;
int b11=0;
int b12=0;
int b13=0;

float t = 1.0;
float h = 0.0;


void lowall()
{
digitalWrite(LED_BUILTIN, 0);
digitalWrite(b0, 0);
digitalWrite(b1, 0);
digitalWrite(b2, 0);
digitalWrite(b3, 0);

b01=0;
b11=0;
b12=0;
b13=0;
}


String processor(const String& var){
  //Serial.println(var);
  if(var == ""){
    return String(t);
  }
  else if(var == ""){
    return String(h);
  }
  return String();
}

void setup(){

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(b0, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);

  pinMode(A0, INPUT);

  Serial.print("Setting AP (Access Point)…");
  lowall();
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/b0", HTTP_GET, [](AsyncWebServerRequest *request){
    String value = request->arg("value");
    Serial.println(value);
    Fb0(value);
    
    request->send_P(200, "text/plain", String(t).c_str() );
  });
  server.on("/b1", HTTP_GET, [](AsyncWebServerRequest *request){
    String value = request->arg("value");
    Fb1(value);
    request->send_P(200, "text/plain",String(t).c_str());
  });
  server.on("/b2", HTTP_GET, [](AsyncWebServerRequest *request){
    String value = request->arg("value");
    Fb2(value);
    request->send_P(200, "text/plain",String(t).c_str());
  });
  server.on("/b3", HTTP_GET, [](AsyncWebServerRequest *request){
    String value = request->arg("value");
    Fb3(value);
    request->send_P(200, "text/plain",String(t).c_str());
  });

  // Start server
  server.begin();
}
void Fb0(String val)
{
  if(val =="ON")
  {
    Serial.println("ON");
    digitalWrite(b0, HIGH);
    //b01=1;
  }
  else
  {
    Serial.println("OFF");
    digitalWrite(b0, LOW);
    //b01=0;
  }
  
}
void Fb1(String val)
{
  
  if(val =="ON")
  {
    digitalWrite(b1, HIGH);
    //b01=1;
  }
  else
  {
    digitalWrite(b1, LOW);
    //b01=0;
  }
}

void Fb2(String val)
{
  if(val =="ON")
  {
    digitalWrite(b2, HIGH);
    //b01=1;
  }
  else
  {
    digitalWrite(b2, LOW);
    //b01=0;
  }
}

void Fb3(String val)
{
  if(val =="ON")
  {
    digitalWrite(b3, HIGH);
    //b01=1;
  }
  else
  {
    digitalWrite(b3, LOW);
    //b01=0;
  }
}


void loop() {
//add the code that runs in loop
}
