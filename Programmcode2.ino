//Bibliotheken
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

//WLAN-Zugangsdaten
const char* ssid = "Willan";
const char* password = "35580199024334574582";

//Telegram-Bot initialisieren
#define botToken "2049642450:AAEelfj5jhgITMlbN5USnfA4S62-zjxrd5A"  // den Bot-Token bekommst du vom Botfather)

//UserID
#define userID "220972468"

WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

//Variable für das Licht
const int lightPin = 14; //Am ESP8266 Pin D5
bool lightState = LOW;

//Variable für die Anzahl der Anfragen
int numNewRequests;

//Variable für den Text der Anfrage, die du sendest
String text = "";

//UserID des Absenders
String chat_id = "";

//Name des Absenders
String from_name = "";

//Variable für die Willkommensnachricht
String welcome = "";

//testvariablen
String test11 = "0";

//Funktion fürs Verarbeiten neuer Anfragen
void handleNewRequests(int numNewRequests) {

  for (int i = 0; i < numNewRequests; i++) { //loopt durch die neuen Anfragen

    //Checkt, ob du die Anfrage gesendet hast oder jemand anderes
    chat_id = String(bot.messages[i].chat_id);
    if (chat_id != userID) {
      bot.sendMessage(chat_id, "Du bist nicht autorisiert!", "");
      continue;
    }

    // Anfragetext speichern
    text = bot.messages[i].text;
    Serial.println(text);

    from_name = bot.messages[i].from_name;

    if (text == "/start") {
      welcome = "Willkommen, " + from_name + ".\n";
      welcome += "Folgende Befehle kannst du verwenden: \n\n";
      welcome += "/lichtEin \n";
      welcome += "/lichtAus \n";
      welcome += "/status \n";
      bot.sendMessage(chat_id, "http://gph.is/1Rc70ke", "");
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/lichtEin") {
      lightState = HIGH;
      digitalWrite(14, lightState);
      bot.sendMessage(chat_id, "Das Licht ist an.", "");
    }
    if (text == "/anzahl") {
      bot.sendMessage(chat_id, "Die Anfrage Anzahl ist:" + String(test11), "");
  

    }

    if (text == "/lichtAus") {
      lightState = LOW;
      digitalWrite(14, lightState);
      bot.sendMessage(chat_id, "Das Licht ist aus.", "");
    }

    if (text == "/status") {
      if (digitalRead(lightPin)) {
      bot.sendMessage(chat_id, "Das Licht ist an.", "");
      }
      else {
        bot.sendMessage(chat_id, "Das Licht ist aus.", "");
      }
    }
  }
}

void setup() {

  Serial.begin(115200);
  client.setInsecure();

  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, lightState);

  //Verbindung zum WLAN
  Serial.print("Verbinde mich mit: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("");
  Serial.println("Verbunden!");
}

void loop() {

  //checkt, ob eine neue Anfrage reinkam
  int numNewRequests = bot.getUpdates(bot.last_message_received + 1);

  while (numNewRequests) { //wird ausgeführt, wenn numNewRequests == 1
    Serial.println("Anfrage erhalten");
    handleNewRequests(numNewRequests);
    numNewRequests = bot.getUpdates(bot.last_message_received + 1);
  }
  delay(1000);
  test11 = numNewRequests;
}
