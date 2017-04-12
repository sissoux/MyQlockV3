#include "FastLED.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

//#include <TimeLib.h>
#include "MyQlock.h"
#include "define.h"
//#include <Wire.h>
//#include <DS1307RTC.h>
#include "Color.h"
#include <elapsedMillis.h>

char ssid[] = "AndroidAlex";  //  your network SSID (name)
char pass[] = "benisoitfreemobile";       // your network password

#define SERIAL_VERBOSE

unsigned int localPort = 2390;      // local port to listen for UDP packets


IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
unsigned long sendNTPpacket(IPAddress& address);

MyQlock Qlock = MyQlock();


elapsedMillis TimeRefreshTimer = 0;
#define TIME_REFRESH_RATE 500 //ms

elapsedMillis DisplayRefreshTimer = 0;
#define DISPLAY_REFRESH_RATE 20 //ms (50fps)

elapsedMillis SerialRefreshTimer = 0;
#define SERIAL_REFRESH_RATE 10 //ms

elapsedMillis ChangeColorTimer = 0;
#define CHANGE_COLOR_RATE 500 //ms

elapsedMillis SecondCounter = 0;
#define ONE_SECOND 100 // to Reset each second

elapsedMillis NTPTimeout = 0;
#define NTP_TIMEOUT 5000 // to Reset each second

elapsedMillis TimeReSyncTimer = 0;
#define DEFAULT_RESYNC_PERIOD 3600000UL
uint32_t TimeReSyncPeriod = DEFAULT_RESYNC_PERIOD; //try so sync NTP each hour
boolean NTPUpdateStarted = false;

uint16_t TimeCounter = 0;

double Sign = -0.005;


void startWifi()
{
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
#ifdef SERIAL_VERBOSE
  Serial.println("");
  Serial.println("");
  Serial.println("*********** MyQlock V3.00 - Vision Of Light 04/11/2017 ***********");
  Serial.println("Qlock Intialization...");
#endif
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, GBR>(Qlock.leds, NUM_LEDS);
  Qlock.begin();
  //Qlock.rainbowLoop();
#ifdef SERIAL_VERBOSE
  Serial.println("Wifi Initialization...");
#endif
  //startWifi();
}

void loop()
{
  TaskManager();
}

void TaskManager()
{
  while (SecondCounter >= ONE_SECOND)
  {
    SecondCounter -= ONE_SECOND; //Substract only one second as we can to remain synchronized to system time
    Qlock.UnixTimeStamp++;
    TimeCounter++;
#ifdef SERIAL_VERBOSE
    if (TimeCounter >= 60) //Print state every minute
    {
      TimeCounter = 0;
      Serial.print("Current time is : ");
      Serial.print(Qlock.Hour);
      Serial.print(":");
      Serial.println(Qlock.Minute);
      Serial.print("s before next sync : ");
      Serial.println((TimeReSyncPeriod - TimeReSyncTimer) / 1000);
    }
#endif
  }

  if (DisplayRefreshTimer >= DISPLAY_REFRESH_RATE)
  {
    DisplayRefreshTimer = 0;
    Qlock.writeOutput();
    FastLED.show();
  }

  if (SerialRefreshTimer >= SERIAL_REFRESH_RATE)
  {
    SerialRefreshTimer = 0;
    SerialCheck();
  }

  if (ChangeColorTimer >= CHANGE_COLOR_RATE)
  {
    ChangeColorTimer = 0;
    ColorUpdate();
  }

  if (TimeReSyncTimer >= TimeReSyncPeriod || !Qlock.HasBeenSync)
  {
    TimeReSyncTimer = 0;
    //UpdateNTP();
  }
}

void ColorUpdate()
{/*
  if (Qlock.CurrentColor.h < 360)
  {
    Qlock.CurrentColor.h += 0.1;
  }
  else
  {
    Qlock.CurrentColor.h = 0;
  }

  Qlock.CurrentColor.s = Qlock.CurrentColor.s + Sign;
  if (Qlock.CurrentColor.s >= 1)
  {
    Sign = -0.005;
    Qlock.CurrentColor.s = 1;
  }
  else if (Qlock.CurrentColor.s <= 0)
  {
    Sign = 0.01;
    Qlock.CurrentColor.s = 0;
  }
  Qlock.CurrentColor.convertToRGB();*/
}

void SerialCheck()
{
  if (Serial.available())
  {
    /*time_t t = processSyncMessage();
      if (t)
      {
      RTC.set(t);   // set the RTC and the system time to the received value
      setTime(t);
      }*/
  }
}

void InitRTC()
{
  /*setSyncProvider(RTC.get);
    if (timeStatus() != timeSet)
    Serial.println("Unable to sync with the RTC");
    else
    Serial.println("RTC has set the system time");
    setSyncInterval(120);*/
}

void checkWifi()
{
  //connect wifi if not connected
  if (WiFi.status() != WL_CONNECTED)
  {
    startWifi();
  }
}

void UpdateNTP()
{
  checkWifi();
  if (!NTPUpdateStarted)
  {
    //get a random server from the pool
    WiFi.hostByName(ntpServerName, timeServerIP);

    sendNTPpacket(timeServerIP); // send an NTP packet to a time server
    NTPUpdateStarted = true;
    NTPTimeout = 0;
    TimeReSyncPeriod = 500; //Change update rate to half a second because we are waiting for NTP Packet
  }
  else if (NTPTimeout < NTP_TIMEOUT)
  {
    int cb = udp.parsePacket();
    if (!cb)
    {
      //Serial.println("no packet yet");
    }
    else
    {
      Serial.print("packet received, length=");
      Serial.println(cb);
      // We've received a packet, read the data from it
      udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
      //the timestamp starts at byte 40 of the received packet and is four bytes,
      // or two words, long. First, esxtract the two words:

      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      // combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      unsigned long secsSince1900 = highWord << 16 | lowWord;

      // now convert NTP time into everyday time:
      // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:

      int error = Qlock.UnixTimeStamp - (secsSince1900 - 2208988800UL);
      Serial.print("Time error in s : ");
      Serial.println(error);
      Qlock.UnixTimeStamp = secsSince1900 - 2208988800UL;

      if (!Qlock.HasBeenSync) Qlock.HasBeenSync = true;
      NTPUpdateStarted = false;
      TimeReSyncPeriod = DEFAULT_RESYNC_PERIOD; //Back to normal period
    }
  }
  else
  {
    Serial.println("Failed to get NTP time, retry next hour");
    NTPUpdateStarted = false;
    TimeReSyncPeriod = DEFAULT_RESYNC_PERIOD; //Back to normal period
  }
}


unsigned long processSyncMessage()
{
  unsigned long pctime = 0L;
  if (Serial.find(TIME_HEADER))
  {
    pctime = Serial.parseInt();
  }
  return pctime;
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress & address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

