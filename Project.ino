#include "SD.h"
#include "TMRpcm.h"
#include "SPI.h"
#include <SoftwareSerial.h>
#include "DHT.h"

/* Configurations */

/* sound level in the area from 0 -> 1023*/
#define ROOM_VOICE_VALUE 700

/* temperature at which message is sent to mother*/
#define NORMAL_TEMPERATURE_VALUE 39


/* End of Configurations */

/* SIM800 TX is connected to Arduino D8 */
#define SIM_RX_PIN         7

/* SIM800 RX is connected to Arduino D7 */

#define SIM_TX_PIN         8

/* STM TX is connected to Arduino D6    */
#define BLUETOOTH_RX_PIN            6

/* STM RX is connected to Arduino D5    */

#define BLUETOOTH_TX_PIN            5


/* SD card reader pin */
#define SD_CHIP_SELECT_PIN 10


/* Analog Pin for Rain Sensor */
#define RAIN_SENSOR_OUT_A     A0

/* Analog Pin for Sound sensor*/ 
#define SOUND_SENSOR_OUT_A    A1

/* Define DHT Pin for sending and receiving data and specify its type */
#define DHT_PIN 2
#define DHTTYPE DHT11

/* define pin for external interrupt for indicating if there is a coming message or calling */
#define EXT_INT_PIN 3

/* define IDs for other sensors to communicate with STM32 using bluetooth module */
#define SMOKE_SENSOR_ID      'S'
#define FIRE_SENSOR_ID       'F'
#define IR_SENSOR_ID         'I'

/* Create software serial object to communicate with SIM800 */
SoftwareSerial sim(SIM_RX_PIN,SIM_TX_PIN); // Rx, Tx

/* Create software serial object to comuincate with STM*/
SoftwareSerial stm(BLUETOOTH_RX_PIN,BLUETOOTH_TX_PIN);  // Rx, Tx

/* create object for temperature sensor DHT11 */
DHT dht(DHT_PIN,DHTTYPE);

static char smoke_sensor_status, fire_sensor_status, IR_sensor_status;
String message ;
TMRpcm audio;


void setup()
{
  Serial.begin(9600);
  /* For SIM GSM Module */

  sim.begin(9600);
  stm.begin(9600);
  dht.begin();
  delay(1000);
  
  pinMode(RAIN_SENSOR_OUT_A, INPUT);
  pinMode(SOUND_SENSOR_OUT_A, INPUT);
  pinMode(EXT_INT_PIN, INPUT_PULLUP);
  audio.speakerPin = 9;
  if (!SD.begin(SD_CHIP_SELECT_PIN)) 
  {
    Serial.println("SD fail");
    return;
  }
 
  Serial.println("Initializing..."); 
  delay(1000);

}

void loop()
{
  // get readings of all sensors and send message if any state is 
  // not normal
  GetReadings();
  // check if there is a request for readings
  if ( digitalRead(EXT_INT_PIN) == 0)
  {
    SendReadings();
   
   }
  delay(2000);
  Serial.println("\n---------------\n\n");
}





void SendMessage(String message)
{
  sim.println("AT"); //Once the handshake test is successful, it will back to OK
  UpdateSerial();

  sim.println("AT+CMGF=1"); // Configuring TEXT mode
  UpdateSerial();
  sim.println("AT+CMGS=\"+201++++\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  UpdateSerial();
  sim.print(message+"\0"); //text content
  UpdateSerial();
  sim.write(26);
}
void UpdateSerial()
{
  delay(500);
    while(Serial.available())      
  {                    
    sim.write(Serial.read());//Forward what Serial received to Software Serial Port
  }

  while(sim.available()) 
  {
    Serial.write(sim.read());//Forward what Software Serial received to Serial Port
  }
}
bool IsBabyCry()
{
  static byte read_nmuber;
  bool is_cry = false;
  /* print sound sensor output */
  Serial.print("Sound Sensor : ");

  Serial.println(analogRead(SOUND_SENSOR_OUT_A)); 
  if (analogRead(SOUND_SENSOR_OUT_A) > ROOM_VOICE_VALUE)
  {
    if (read_nmuber == 5)
    {
      is_cry = true;
      read_nmuber = 0;
    }
    else
    {
      read_nmuber ++;
    }
    
  }
  else
  {
    is_cry = false;
  }
  return is_cry;
}

void RunMusic()
{
  audio.setVolume(4);
  audio.play("test.wav");
}

bool IsBabyWet()
{
  bool is_wet = false;
  static byte read_nmuber ;
  if(analogRead(RAIN_SENSOR_OUT_A) < 500) 
  {
    if (read_nmuber == 5)
    {
      is_wet = true;
      read_nmuber = 0;
    }
    else
    {
      read_nmuber ++;  
    }
  }
  else
  {
    is_wet = false;
  } 
  delay(10);
  Serial.print("Rain Sensor: ");
  Serial.println(analogRead(RAIN_SENSOR_OUT_A));
  return is_wet;
}
bool IsTemperatureHigh()
{
  bool is_high = false;
  float temperature = dht.readTemperature();
  static byte read_nmuber ;

  Serial.print("TEMPRATURE = ");
  Serial.print(temperature);
  Serial.println(" C");

  if (temperature >= NORMAL_TEMPERATURE_VALUE)
  {
    if (read_nmuber == 5)
    {
      is_high = true;
      read_nmuber = 0;
    }
    else
    {
      read_nmuber ++;
    }
    
  }
  else
  {
    is_high = false;
  }

return is_high;
}



void GetReadings()
{
  message = "";

  if (IsBabyCry() == true)
  {
    if (audio.isPlaying() == false)
    {
      RunMusic();
      message += "Crying";
    }
    
  }
  if (IsBabyWet() == true)
  {
      message += " Wet";

  }
  if (IsTemperatureHigh() == true)
  {
    message += " hot";

  }
  
  /* Get inforamtion from STM  */
    stm.listen();
    GetSTMReadings();
    sim.listen();
  if (message != "")
  {
    SendMessage(message);
  } 
}



void SendReadings()
{
  message = "";
  if (IsBabyCry() == true)
  {
    message += "Crying";
  }

  if (IsBabyWet() == true)
  {
    message += " Wet";
  }

  if (IsTemperatureHigh() == true)
  {
    message += " Hot";
  }
    else 
    {
      message += " cool";
      
      }

  /* Get inforamtion from STM  */
    stm.listen();
    GetSTMReadings();
    sim.listen();

  /* Then Send Total message*/
  if (message != "")
  {
    SendMessage(message);
  } 
  else 
  {
  
      SendMessage("Fine");
    
  }

}

void GetSTMReadings()
{
  
     stm.write(IR_SENSOR_ID);
     IR_sensor_status = stm.read();
    while (IR_sensor_status == -1)
    {
      IR_sensor_status = stm.read();
      delay(1);
      }
    if (IR_sensor_status == '1')
    {
           message += "Near Thing";
          Serial.println ("Near Thing");
    }
    else if (IR_sensor_status == '0')
    {
          Serial.println ("No thing");
    }

/********************************************/

stm.write(SMOKE_SENSOR_ID);
    smoke_sensor_status = stm.read();
    while (smoke_sensor_status == -1)
    {
      smoke_sensor_status = stm.read();
      delay(1);
      }
      if (smoke_sensor_status == '1')
    {
          message += "Smoke";
          Serial.println ("Smoke");
          
    }
    else if (smoke_sensor_status == '0')
    {
          Serial.println ("No Smoke");
    }
    
    
    /*****************************/

    stm.write(FIRE_SENSOR_ID);
    fire_sensor_status = stm.read();
    while (fire_sensor_status == -1)
    {
      fire_sensor_status = stm.read();
      delay(1);
      }
    if (fire_sensor_status == '1')
    {
          message += "Fire";
          Serial.println ("Fire");
          
    }
    else if (fire_sensor_status == '0')
    {
          Serial.println ("No Fire");
    }

}


