#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

const char* host = "esp32";
const char* ssid = "RUTRADA";
const char* password = "0894280008";

//variabls for blinking an LED with Millis
const int led = 2; // ESP32 Pin to which onboard LED is connected
WebServer server(80);

/* Style */
String style =
"<style>"
"  * { box-sizing: border-box; margin: 0; padding: 0; }"
"  body { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; "
"         display: flex; align-items: center; justify-content: center; height: 100vh; color: #333; }"
"  form { background: rgba(255, 255, 255, 0.95); padding: 40px; border-radius: 15px; "
"         box-shadow: 0 15px 35px rgba(0,0,0,0.2); width: 100%; max-width: 350px; text-align: center; }"
"  h1 { font-size: 24px; margin-bottom: 25px; color: #4a4a4a; font-weight: 600; }"
"  input { width: 100%; height: 45px; border-radius: 8px; margin-bottom: 15px; font-size: 14px; "
"          border: 1px solid #e1e1e1; background: #f8f9fa; padding: 0 15px; transition: all 0.3s; }"
"  input:focus { outline: none; border-color: #764ba2; background: #fff; box-shadow: 0 0 8px rgba(118,75,162,0.2); }"
"  #file-input { display: block; background: #f8f9fa; border: 2px dashed #cbd5e0; padding: 15px; "
"                border-radius: 8px; cursor: pointer; margin-bottom: 20px; transition: 0.3s; color: #718096; }"
"  #file-input:hover { background: #edf2f7; border-color: #764ba2; }"
"  .btn { background: #764ba2; color: #fff; border: none; font-weight: bold; cursor: pointer; transition: 0.3s; }"
"  .btn:hover { background: #5a378d; transform: translateY(-1px); box-shadow: 0 5px 15px rgba(0,0,0,0.1); }"
"  #prgbar { background: #edf2f7; border-radius: 20px; height: 12px; margin-top: 15px; overflow: hidden; display:none; }"
"  #bar { background: linear-gradient(90deg, #667eea, #764ba2); width: 0%; height: 100%; transition: width 0.3s; }"
"  #prg { font-size: 12px; color: #718096; margin-top: 5px; }"
"</style>";

/* Login page */
String loginIndex = 
"<form name=loginForm>"
"<h1>ESP32 Login</h1>"
"<input name=userid placeholder='User ID'> "
"<input name=pwd placeholder=Password type=Password> "
"<input type=submit onclick=check(this.form) class=btn value=Login></form>"
"<script>"
"function check(form) {"
"if(form.userid.value=='admin' && form.pwd.value=='admin')"
"{window.open('/serverIndex')}"
"else"
"{alert('Error Password or Username')}"
"}"
"</script>" + style;
 
/* Server Index Page */
String serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>   Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br></form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"
"$('form').submit(function(e){"
"$('#prgbar').show();"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!') "
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" + style;

/* setup function */
void setup(void) {

pinMode(led,  OUTPUT);
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(1);
  digitalWrite(led, HIGH); delay(500);
  digitalWrite(led, LOW);  delay(500);
}