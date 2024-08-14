#include <SPI.h>
#include <Ethernet.h>
#include <ICMPPing.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xCA, 0xEF, 0xFE, 0xEE
};

// fill in an available IP address on your network here,
// for manual configuration:
IPAddress ip(192, 168, 53, 120);

// fill in your Domain Name Server address here:
IPAddress myDns(8, 8, 8, 8);

IPAddress gateway(192, 168, 53, 1);
IPAddress subnet(255, 255, 255, 0);

// Ping setup
IPAddress pingAddr(88, 99, 30, 30); // ip address to ping
SOCKET pingSocket = 0;
char buffer [256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));

// initialize the library instance:
EthernetClient client;

char server[] = "logger.dataio.cloud";

// https://github.com/Ant2000/CustomJWT
String token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJsb2dnZXIuaG9tZTAzIiwibmFtZSI6ImxvZ2dlcjAwMyIsImlhdCI6MjAyMjA0MjV9.VPDRcWAFfGkfPwBNOzjZ5HoiL1DWtLUyifFAAZnNyr4"; // jwt token

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds

void ethernetInit()
{

  // Open serial communications and wait for port to open:
  // Serial.begin(9600);
  //pinMode(ledPin, OUTPUT);
  //  while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  // }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, myDns, gateway, subnet);

  Serial.print("Client is at: ");
  Serial.println(Ethernet.localIP());
}

void ethernetStart() {

  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:

  //Serial.println("client dostupan..."+String(lastConnectionTime));
  /*
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  */

  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  //if (millis() - lastConnectionTime > postingInterval) {
    ethernetHttpRequest();
  //}
}

void ethernetPing()
{
    ICMPEchoReply echoReply = ping(pingAddr, 4);
    //pingserver = false;
    Serial.print("PING: ");
    if (echoReply.status == SUCCESS)
    {
      sprintf(buffer,
              "Reply[%d] from: %d.%d.%d.%d: bytes=%d time=%ldms TTL=%d",
              echoReply.data.seq,
              echoReply.addr[0],
              echoReply.addr[1],
              echoReply.addr[2],
              echoReply.addr[3],
              REQ_DATASIZE,
              millis() - echoReply.data.time,
              echoReply.ttl);
    }
    else
    {
      sprintf(buffer, "Echo request failed; %d", echoReply.status);
    }
    Serial.println(buffer);
}

// this method makes a HTTP connection to the server:
void ethernetHttpRequest() {

  Serial.println("Ethernet send start");

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.print("saljem...");

    String t18b = buildEthernetData();
    Serial.println("data:"+t18b);
    // send the HTTP GET request:
    client.print(String("GET ?save=y&token=") + token + t18b + String("&user=10002"));
    client.println(" HTTP/1.1");
    client.println("Host: logger.dataio.cloud");
    client.println("User-Agent: arduino");
    client.println("Connection: close");
    client.println();

    Serial.println("poslano.");
  }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }

  // if there are incoming bytes available
  // from the server, read them and print them:
  Serial.print("Citam sa servera...");
  while (client.connected())
  {
    while (client.available())
    {
      // char c = client.read();
      // Serial.print(c);
    }
    Serial.print("procitano...");

    client.flush();  //Clear Data Buffer In Ethernet Controller
    client.stop();  //Close Communication With Irrigation Controller
  }
  Serial.println("konekcija zatvorena.");

  client.stop();  //Close Communication With Irrigation Controller

  ethernetPing();

}


