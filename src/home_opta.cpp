/*

*/

#include <LibPrintf.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  Serial.begin(115200);
  delay(500);

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
  // boot indicators
  digitalWrite(LED_D0, HIGH);
  digitalWrite(LED_D1, HIGH);
  digitalWrite(LED_D2, HIGH);
  digitalWrite(LED_D3, HIGH);
  delay(1000);
  digitalWrite(LED_D0, LOW);
  digitalWrite(LED_D1, LOW);
  digitalWrite(LED_D2, LOW);
  digitalWrite(LED_D3, LOW); 

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

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
  delay(2000);

  Serial.println("OPTA - analog board");
}

void loop() {
  float an2 = mapf(analogRead(A2) / 100.0, 0.02, 9.34, 0.02, 10.00);
  int i1 = digitalRead(A0);
  int i2 = digitalRead(A1);

  printf("A2: %.2f V | BTN 0: %d | BTN 1: %d\n", an2, i1, i2);

  if (an2 >= 4.90 && an2 <= 5.02) {
    digitalWrite(D0, HIGH);
    digitalWrite(LED_D0, HIGH);
  } else {
    digitalWrite(D0, LOW);
    digitalWrite(LED_D0, LOW);
  }

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

  delay(200);
}
