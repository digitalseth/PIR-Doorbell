#include <SPI.h>
#include <Ethernet.h>

//button pin
const int BUTTON = 7;

//hue variables
const char hueBridgeIP[] = "192.168.1.142";
const char hueUsername[] = "newdeveloper";
const int hueBridgePort = 80;

String hueOn[2];
int hueBri[2];
long hueHue[2];
int hueSat[2];
String command;
int randomNum;

byte mac[] = {0x98, 0x4F, 0xEE, 0x01, 0xD2, 0x7A};
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetClient client;
  
void setup() {
 pinMode(BUTTON, INPUT);
 Serial.begin(9600);
 Ethernet.begin(mac, ip);
}
void loop() {
  //listen for button press
  if(digitalRead(BUTTON) == HIGH) {
    getHue(2);
    command = "{\"on\": true,\"bri\": 215,\"hue\": 25178,\"sat\":235}";
    setHue(2, command);
    delay(500);
    command = "{\"on\":" + String(hueOn[1]) + ",\"bri\":" + String(hueBri[1]) + ",\"hue\":" + String(hueHue[1]) + ",\"sat\":" + String(hueSat[1]) + "}";
    Serial.println(command);
    setHue(2,command);
  }
  
 if (Serial.available() > 0) {
  if (Serial.read() == 'D'){

    //get the current state of lighbulb
    getHue(1);
    

    //make the lights flash
    randomNum = 2000; 
    command = "{\"on\": true,\"bri\": 225,\"hue\": " + String(randomNum) + ",\"sat\":225}";
    setHue(1, command);
    delay(1000);
    Serial.println("Changed lights due to motion");
    //set the hue back to the original state
    Serial.println("Original Lights");
    Serial.println(hueOn[0]);
    Serial.println(hueHue[0]);
    Serial.println(hueBri[0]);
    Serial.println(hueSat[0]);
    Serial.println(hueOn[1]);
    Serial.println(hueHue[1]);
    Serial.println(hueSat[1]);
    command = "{\"on\":" + String(hueOn[0]) + ",\"bri\":" + String(hueBri[0]) + ",\"hue\":" + String(hueHue[0]) + ",\"sat\":" + String(hueSat[0]) + "}";
    setHue(1,command);
    Serial.print("Hue on:");
    Serial.println(hueOn[0]);
    delay(1000);
  }
 }

}

boolean setHue(int lightNum, String command){
  if (client.connect(hueBridgeIP, hueBridgePort)){
    while (client.connected()){
      client.print("PUT /api/");
      client.print(hueUsername);
      client.print("/lights/");
      client.print(lightNum);
      client.println("/state HTTP/1.1");
      client.println("keep-alive");
      client.print("Host: ");
      client.println(hueBridgeIP);
      client.print("Content-Length: ");
      client.print(command.length());
      client.println("Content-Type: text/plain;charset=UTF-8");
      client.println();
      client.println(command);
      Serial.print("Sent command for light #");
      Serial.println(lightNum);
    }
    client.stop();
    return true;
  }
  else {
    Serial.println("Command failed");
    return false;
  }
 }

 boolean getHue(int lightNum)
{
  if (client.connect(hueBridgeIP, hueBridgePort))
  {
    client.print("GET /api/");
    client.print(hueUsername);
    client.print("/lights/");
    client.print(lightNum);  
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(hueBridgeIP);
    client.println("Content-type: application/json");
    client.println("keep-alive");
    client.println();
    while (client.connected())
    {
      if (client.available())
      {
        //read the current bri, hue, sat, and whether the light is on or off 
        client.findUntil("\"on\":", '\0');
        hueOn[lightNum-1] = (client.readStringUntil(','));
 
        client.findUntil("\"bri\":", '\0');
        hueBri[lightNum-1] = client.readStringUntil(',').toInt();  
 
        client.findUntil("\"hue\":", '\0');
        hueHue[lightNum-1] = client.readStringUntil(',').toInt(); 

        client.findUntil("\"sat\":", '\0');
        hueSat[lightNum-1] = client.readStringUntil(',').toInt(); 
        break;
      }
    }
    client.stop();
    return true;
  }
  else
    return false; 
}

