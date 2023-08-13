/*

*/

#include <LibPrintf.h>
#include <SPI.h>
#include <Ethernet.h>
#include <string> 

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

byte digitLED[][4] = { 
  {1,1,1,1}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1},
  {0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1}, 
  {1,0,0,0}, {1,0,0,1}, {0,0,0,0}
};

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void showDigitOn4LED(uint8_t i) {
  // printf("%d %d%d%d%d\n", (int)i, (int)digitLED[i][0], (int)digitLED[i][1], (int)digitLED[i][2], (int)digitLED[i][3]);
  digitalWrite(LED_D0, digitLED[i][0]);
  digitalWrite(LED_D1, digitLED[i][1]);
  digitalWrite(LED_D2, digitLED[i][2]);
  digitalWrite(LED_D3, digitLED[i][3]);
  delay(4000);
}

void showNumberOn4LED(const char *s) {
  for(const char* p=s; *p;) {
    showDigitOn4LED(*p++ - '0');
  }
}

void showIPaddressOn4LED(IPAddress ip) {
  auto ip0 = std::to_string(ip[0]);
  auto ip1 = std::to_string(ip[1]);
  auto ip2 = std::to_string(ip[2]);
  auto ip3 = std::to_string(ip[3]);

  showNumberOn4LED(ip0.c_str());
  showNumberOn4LED(ip1.c_str());
  showNumberOn4LED(ip2.c_str());
  showNumberOn4LED(ip3.c_str());
  showDigitOn4LED(10);  // leds off
}

void ethernetInit() {
  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println();
  Serial.println("OPTA - analog board");
  // declare the ledPin as an OUTPUT:
  pinMode(LED_D0, OUTPUT);
  pinMode(LED_D1, OUTPUT);
  pinMode(LED_D2, OUTPUT);
  pinMode(LED_D3, OUTPUT);
  //
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  //
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  ethernetInit();

  Serial.println("OPTA - begin show ip address");
  showIPaddressOn4LED(Ethernet.localIP());
  Serial.println("OPTA - end   show ip address");
}

void ethernetProcess() {
  switch (Ethernet.maintain()) {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      break;

    case 2:
      //renewed success
      Serial.println("Renewed success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;

    case 4:
      //rebind success
      Serial.println("Rebind success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    default:
      //nothing happened
      break;
  }
}

void loop() {
  // calibration ?
  // float an2 = mapf(analogRead(A2) / 100.0, 0.02, 9.34, 0.02, 10.00);
  //
  float an2 = analogRead(A2) / 100.0;
  int i1 = digitalRead(A0);
  int i2 = digitalRead(A1);

  printf("A2: %.2f V | BTN 0: %d | BTN 1: %d\n", an2, i1, i2);

  if (an2 >= 4.90 && an2 <= 5.10) {
    digitalWrite(D0, HIGH);
    digitalWrite(LED_D0, HIGH);
  } else {
    digitalWrite(D0, LOW);
    digitalWrite(LED_D0, LOW);
  }
  ethernetProcess();
  delay(200);
}
