#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Wifi vars
String ssid = "ledzGO";
String psk = "password"; // Wifi password
boolean hidden = false;  // Hide wifi ssid
int channel = 10;        // Wifi channel
int max_connection = 8;

WiFiUDP Udp;
unsigned int localUdpPort = 4210;                     // local port to listen on
// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged\r\n";       // a string to send back

void setup()
{
  Serial.begin(115200);
  Serial.println();

  // Wifi setup
  Serial.println("Starting AP");
  Serial.println(WiFi.softAP(ssid, psk, channel, hidden, max_connection) ? "AP ready" : "AP failed");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Udp.begin(localUdpPort);  
}

void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n",
                  packetSize,
                  Udp.remoteIP().toString().c_str(), Udp.remotePort(),
                  Udp.destinationIP().toString().c_str(), Udp.localPort(),
                  ESP.getFreeHeap());

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}
