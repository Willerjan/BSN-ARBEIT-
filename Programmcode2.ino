//Meine Bibilotheken
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "DHT.h"




//Wlan
const char ssid = "WLAN NAME";
const char password = "WLAN PASSWORT";

//Telegram 
#define botschlussel "BOTSCHLÜSSEL
#define userID "USERID"

//Lüfter
const int lüfter = 14;
bool Status = LOW;

//temperatursensor
#define temp 12
#define DHTTYPE DHT11  
DHT dht(temp, DHTTYPE);





//string text
String textwillkommen = "";
String anzahltext ="";

//Anzahl Anfragen 
int anzahl = 0;

//Empfangen Text
String text ="";

WiFiClientSecure client;
UniversalTelegramBot bot(botschlussel, client);

void setup() {
  Serial.begin(115200);
  client.setInsecure();

  pinMode(lüfter, OUTPUT);
  digitalWrite(lüfter, Status);

  //Verbindung zum Wlan
  Serial.print("WLAN: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() !=WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println("");
  Serial.println("Verbunden!");

  //Startnachricht
  textwillkommen = "Moin JAN, ich bin wieder online";
  bot.sendMessage(chat_id, textwillkommen,"");
  
  //TemperaturSensor
  pinMode(temp, INPUT);
  dht.begin();
}

void loop() {
  
  //Überbrüfe auf neue anfragen
  int newAnfrage = bot.getUpdates(bot.last_message_received + 1);

  while(newAnfrage){
    Serial.print("Anfrage erhalte");
    Ausfuhrer(newAnfrage);
    newAnfrage = bot.getUpdates(bot.last_message_received + 1);
  }
  uberprufung();
  delay(500);
}

void Ausfuhrer (int newAnfrage){

  for(int i = 0; i < newAnfrage; i++){

    //Abfrage vom richtigen User
    chat_id = String(bot.messages[i].chat_id);
    if(chat_id != userID){
      bot.sendMessage(chat_id, "Du bist nicht Jan","");
      continue;
    }

    //text speichern
    text = bot.messages[i].text;


 
    if(text == "/start"){
      textwillkommen = "Hey Jan,\n";
      textwillkommen += "Du kannst diese Befehle verwenden:\n\n";
      textwillkommen += "Lüftersteuerung \n";
      textwillkommen += "/luefteran Um den Lüfter einzuschalten \n";
      textwillkommen += "/luefteraus Um den Lüfter auszuschalten \n";
      textwillkommen += "/status Um den Status abzufragen \n";
      textwillkommen += "/anzahl Um die Anzahl der Abfrage auszugeben \n\n";
      textwillkommen += "Temperatur\n";
      textwillkommen += "/temp Um die Temperatur anzuzeigen \n";
      textwillkommen += "/luft Um die Luftfeuchtigkeit anzuzeigen \n";
      textwillkommen += "\n";
      bot.sendMessage(chat_id, textwillkommen, ""); 
    }

    if(text == "/luefteran"){
      anzahl++;
      Status = HIGH;
      digitalWrite(lüfter, Status);
      bot.sendMessage(chat_id,"Der Lüfter leuft.","");
    }

    if(text == "/luefteraus"){
      anzahl++;
      Status = LOW;
      digitalWrite(lüfter, Status);
      bot.sendMessage(chat_id,"Der Lüfter ist aus.","");
    }

    if(text == "/status"){
      anzahl++;
      if(Status == HIGH){
        bot.sendMessage(chat_id, "Der Lüfter leuft.","");
      }
      if(Status == LOW){
        bot.sendMessage(chat_id, "Der Lüfter ist aus.","");
      }
    }

    if(text =="/anzahl"){
      anzahl++;
      anzahltext = "Anzahl der Anfragen: ";
      anzahltext += anzahl;
      bot.sendMessage(chat_id, anzahltext, "");
    }
    
     if(text =="/temp"){
      anzahl++;
      float t = dht.readTemperature(); 
      anzahltext = "Temperatur: ";
      anzahltext += t;
      anzahltext += "ºC";
      bot.sendMessage(chat_id, anzahltext, "");
    }

     if(text =="/luft"){
      anzahl++;
      float h = dht.readHumidity();    
      anzahltext = "Luftfeuchtigkeit: ";
      anzahltext += h;
      bot.sendMessage(chat_id, anzahltext + "%", "");
    }

   
    
  }
}

void uberprufung(){

  float t = dht.readTemperature();
    if (t >= 35){
      textwillkommen += "WARNUNG TEMPERATUR IST ZU HOCH!!!\n";
      textwillkommen += "Soll der Lüfter angemacht werden?\n";
      textwillkommen += "Ja: /Lüfteran \n";
      bot.sendMessage(chat_id, textwillkommen, ""); 
  }
  
}
