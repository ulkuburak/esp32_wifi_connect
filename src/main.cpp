#include <Arduino.h>
#include <Firebase_ESP_Client.h>


// Insert your network credentials
#define WIFI_SSID "Kablonet Netmaster-dogan"
#define WIFI_PASSWORD "42e1bc3e"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDyof08i5lXRiBAHbGH3XBGMY4eoib5gjs"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://esp32-fb-38035-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
    break;
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setInt(&fbdo, "test/int", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " );
       Serial.print(fbdo.dataPath());
      Serial.println("TYPE: ");
      Serial.print( fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
     
    }
    count++;
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "test/float", 0.01 + random(0,100))){
      Serial.println("PASSED");
      Serial.println("PATH: " );
      Serial.print( fbdo.dataPath());
      Serial.println("TYPE: ");
      Serial.print( fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: ");
      Serial.print( fbdo.errorReason());
    }
  }
}