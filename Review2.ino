// MODIFIED AIR QUALITY HEALTH INDEX
// 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DFRobot_sim808.h>
#include "DHT.h"
#include "SdsDustSensor.h"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DFRobot_SIM808 sim808(&Serial1);//Connect RX,TX,PWR,
SdsDustSensor sds(Serial2); // passing HardwareSerial& as parameter

#define DHTPIN  26     // Digital pin connected to the DHT sensor
#define CO      A0     // Analog A0 to MQ-7 CO sesnor
#define MQ135   A1     // Analog A1 to MQ135 sensor
#define Battery A5     // Analog input for battery level monitor
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);
float   dht11_hum,dht11_temp,dht11_hindex, MAQHI,PM2_5,PM10=0;   // Global Variables for sensors readings
double  LONGITUDE, LATITUDE; 
float   sensorValue0, sensorValue1, sensorValue2 ;
int     counter1 =0;      // counter 1
int     counter2 =0;      // statues update
char    latitude[12],longitude[12];
  
float kCO=1,kMQ135=1,kPM2_5=1,kPM10=1,kt1=1,kh1=1; // propotionality constants

void setup() {
    
    // Read SDS011 on Serial 
       Serial2.begin(115200);  // serial2 communicate with PM sensor 
       sds.begin(); // this line will begin Serial2 with given baud rate (9600 by default)
       Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
       Serial.println(sds.setQueryReportingMode().toString()); // ensures sensor is in 'query' reporting mode

    Serial1.begin(115200); //serial1 communicate with SIM808
    Serial.begin(115200);
    dht.begin();
    display.begin(); //OLED
     if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);}
        delay(2000);
        display.clearDisplay();
        display.setTextColor(WHITE);

        Serial.println("Initializing...");
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("Initi....");
          delay(1000);
    Serial1.println("AT"); //Once the handshake test is successful, it will back to OK
    updateSerial();delay(500);
    Serial1.println(" AT+IPR=115200"); //set baud rate
    updateSerial();delay(500);
    Serial1.println(" AT+ECHARGE=1"); //enable battery charging function. By default the charging function is closed. 
    updateSerial();delay(500);
    Serial1.println(" AT&W");       //Save parameter setting. 
    updateSerial();delay(500);
    Serial1.println("AT+CSQ");       //Signal quality test, value range is 0-31 , 31 is the best
    updateSerial();delay(500);
    Serial1.println("AT+CCID");     //Read SIM information to confirm whether the SIM is plugged
    updateSerial();delay(500);
    Serial1.println("AT+CREG?");    //Check whether it has registered in the network
    updateSerial();delay(500);
//    Serial1.println("AT+CGNSPWR=1"); // GPS power on
//    updateSerial();delay(500);
//    Serial1.println("AT+CGNSSEQ=RMC"); //Time, date, position, course and speed data
//    updateSerial();delay(500);


    
    
    //====================Initialize sim808 module=================
    while(!sim808.init()) {
        delay(6000);
        Serial.print("Sim808 init error\r\n");
    }
    //================Turn on the GPS power======================
    if( sim808.attachGPS()){
        Serial.println("Open the GPS power success");}
    else{
        Serial.println("Open the GPS power failure");}
 }
void loop() {   
 if (sim808.getGPS()){
         counter1=counter1+1;
         ReadPM();
         display_PM_oled();
         ReadDHT();
         display_dht_oled(); delay(1000);
         ReadGPS();
         ReadCO();  //311 MQ7
         display_co_oled();delay(1000);
         ReadMQ135(); //21 MQ135
         display_MQ135_oled();delay(1000);
         calMAQHI(); // calculate modified MAQHI
         delay(1000);

         update_data_Thingspeak();
         update_data_maqhi();
     
         
         if(counter1>10){ // after 50 records SMS wll send 
             sendsms();
             counter1=0;

               Serial1.println("AT+CBC"); //Inquire charging status and remaining battery capacity. 
              updateSerial();delay(500);
             }
         delay(2000);
    }

 }
void ReadPM(){
  Serial.println("wake up PM Sensor...");
  sds.wakeup();
  delay(30000); // working 30 seconds

  PmResult pm = sds.queryPm();
  if (pm.isOk()) {
    Serial.print("PM2.5 = ");
    Serial.print(pm.pm25);
    PM2_5= pm.pm25;
    Serial.print(", PM10 = ");
    Serial.println(pm.pm10);
    PM10= pm.pm10;

    // if you want to just print the measured values, you can use toString() method as well
    Serial.println(pm.toString());
  } else {
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
  }

  WorkingStateResult state = sds.sleep();
  if (state.isWorking()) {
    Serial.println("Problem with sleeping the sensor.");
  } else {
    Serial.println("PM Sensor is sleeping");
    delay(60000); // wait 1 minute
  }
        // heardware serial example
        }
void ReadGPS(){
           Serial1.println("AT+CGPSPWR=1"); // open GPS
           delay(1000);
           Serial1.println("AT+CGPSSTATUS=1"); //GPS  fix with 3D status
           delay(2000);
           Serial1.println("AT+CGPSINF=0"); //Get the current GPS location information. 
           //Parameters formate: <mode>, <altitude>, <longitude>, <UTC time>, <TTFF>, <num>, <speed>, <course> 
           delay(2000);
           //Serial1.println("AT+CGPSRST=0"); // Reset GPS in Cold Start Mode. 
           //delay(2000);
           //Serial1.println("AT+CGPSRST=1"); // Reset GPS in Hot Start Mode. 
           //delay(2000);

      Serial.print(sim808.GPSdata.year);
      Serial.print("/");
      Serial.print(sim808.GPSdata.month);
      Serial.print("/");
      Serial.print(sim808.GPSdata.day);
      Serial.print(" ");
      Serial.print(sim808.GPSdata.hour);
      Serial.print(":");
      Serial.print(sim808.GPSdata.minute);
      Serial.print(":");
      Serial.print(sim808.GPSdata.second);
      Serial.print(":");
      Serial.println(sim808.GPSdata.centisecond);
      Serial.print("latitude :");
      Serial.println(sim808.GPSdata.lat+.0077820,7);
      Serial.print("longitude :");
      Serial.println(sim808.GPSdata.lon+0.3960045,7);
      Serial.print("speed_kph :");
      Serial.println(sim808.GPSdata.speed_kph);
      Serial.print("heading :");
      Serial.println(sim808.GPSdata.heading);
      Serial.println();
  
      double LATITUDE = ((sim808.GPSdata.lat+.0077820)); // gps data to LATITUDE variable
      double LONGITUDE = ((sim808.GPSdata.lon+0.3960045)); // gps data to LONGITUDE variable
      double ws = sim808.GPSdata.speed_kph;

  
      dtostrf(LATITUDE, 2,7, latitude); //put float value of LATITUDE into char array of latitude where 6 = number of digits after decimal sign. 2 = number of digits befor the decimal sign.
      dtostrf(LONGITUDE, 2,7, longitude); //put float value of LONGITUDE into char array of longitude where 6 = number of digits after decimal sign. 2 = number of digits befor the decimal sign.
    //  dtostrf(ws, 2,7, wspeed);  //put float value of ws into char array of wspeed

      Serial.println(latitude); //print data using a char array
      delay(500);
     
      Serial.println(longitude); // print data using a chaar arry 
      delay(500);

      //sim808.detachGPS();//Turn off the GPS power
       }
void ReadCO(){
      // read the input on analog pin 0:
      sensorValue0 = analogRead(CO);
      // print out the value you read:
      //Serial.print("%  CO:");
      Serial.print(sensorValue0);
      Serial.print(",");
      delay(500);
      display_co_oled();
      delay(500);}
void ReadMQ135(){
      // read the input on analog pin 0:
      sensorValue1 = analogRead(MQ135);
      // print out the value you read:
      //Serial.print("%  CO:");
      Serial.print(sensorValue1);
      Serial.print(",");
      delay(500);
      display_MQ135_oled();
      delay(500);}
void calMAQHI(){
  MAQHI=(((CO*kCO)+(MQ135*kMQ135)+(PM2_5*kPM2_5)+(PM10*kPM10))*kt1*kh1*.5); //k1,k2,k3,k4 propotionality constants t1,h1 propotinality constant for tem & humidity
  Serial.print(MAQHI);
  delay(100);
  diaply_maqhi_oled();
  delay(200);
  }
void ReadDHT(){
         delay(1000);
         dht11_hum = dht.readHumidity();
         dht11_temp = dht.readTemperature();
      // Check if any reads failed and exit early (to try again).
      //  if (isnan(dht11_hum) || isnan(dht11_temp)) {
      //    Serial.println(F("Failed to read from DHT sensor!"));
      //    return;
      //  }
        float dht11_hindex = dht.computeHeatIndex(dht11_temp, dht11_hum, false);
        
        //Serial.print(F("Humidity: "));
        Serial.print(dht11_hum);
         Serial.print(",");
        //Serial.print(F("%  Temperature: "));
        Serial.print(dht11_temp);
         Serial.print(",");
       // Serial.print(F("°C "));
       // Serial.print(F("°F  Heat index: "));
        Serial.println(dht11_hindex);
  }
void updateSerial(){ //Update Seial to serial1 and Serial1 to Serial
        delay(500);
        while (Serial.available()>0) 
        {Serial1.write(Serial.read());}//Forward what Serial received to Serial1 Port
        while(Serial1.available()>0) 
        {Serial.write(Serial1.read());}//Forward what Serial1 received to Serial Port
         }
void sendsms(){
        
         Serial1.println("AT"); //Once the handshake test is successful, it will back to OK
         updateSerial();
         Serial1.println("AT+CMGF=1"); // Configuring TEXT mode
         updateSerial();
         Serial1.println("AT+CMGS=\"+94718278524\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
         updateSerial();
         Serial1.print("MAQHI: "); //text content
         updateSerial();
         Serial1.print(MAQHI); //text content
         updateSerial();
         Serial1.print("           CO :  "); //text content
         updateSerial();
         Serial1.print(CO); //text content
         updateSerial();
         Serial1.print("          Temperature :  "); //text content
         updateSerial();
         Serial1.print(dht11_temp); //text content
         updateSerial();
         Serial1.print("    Humidity :  "); //text content
         updateSerial();
         Serial1.print(dht11_hum); //text content
         updateSerial(); 
             Serial1.print("  http://maps.google.com/maps?q=");
              Serial1.print(latitude);
              Serial1.print(",");
              Serial1.print (longitude);
              delay(200);
         Serial1.write(26);
  }

void display_dht_oled(){
        display.clearDisplay();
        //================== display temperature===========
        display.setTextSize(1);
        display.setCursor(0,0);
        display.print("Temperature: ");
        display.setTextSize(2);
        display.setCursor(0,10);
        display.print(dht11_temp);
        display.print(" ");
        display.setTextSize(1);
        display.cp437(true);
        display.write(167);
        display.setTextSize(2);
        display.print("C"); 
        //==================display humidity==================
        display.setTextSize(1);
        display.setCursor(0, 35);
        display.print("Humidity: ");
        display.setTextSize(2);
        display.setCursor(0, 45);
        display.print(dht11_hum);
        display.print(" %"); 
        display.display();
  }
void display_co_oled(){
        display.clearDisplay();
        //=========display CO==================
        display.setTextSize(1);
        display.setCursor(0,0);
        display.print("CARBON MONOXIDE: ");
        display.setTextSize(3);
        display.setCursor(0,25);
        display.print(CO);
        display.print(" ");
        //  display.setTextSize(1);
        //  display.cp437(true);
        //  display.write(167);
        display.setTextSize(3);
        display.print("PPM");
        display.display();
    
   }
void display_MQ135_oled(){
        display.clearDisplay();
        //=========display CO==================
        display.setTextSize(1);
        display.setCursor(0,0);
        display.print("SULPER DIOXIDE: ");
        display.setTextSize(3);
        display.setCursor(0,25);
        display.print(MQ135);
        display.print(" ");
        //  display.setTextSize(1);
        //  display.cp437(true);
        //  display.write(167);
        display.setTextSize(3);
        display.print("PPM");
         display.display();
    
   }
void display_PM_oled(){
        display.clearDisplay();

        display.setTextSize(1);
        display.setCursor(0,0);
        display.print("PARTICULAR MATTER 2.5 : ");
        display.setTextSize(2);
        display.setCursor(0,20);
        display.print(PM2_5);
        display.print(" ");
        //  display.setTextSize(1);
        //  display.cp437(true);
        //  display.write(167);
        display.setTextSize(2);
        display.print("PPM");
         display.display();
         delay(2000);
        
        display.clearDisplay();

        display.setTextSize(1);
        display.setCursor(0,0);
        display.print("PARTICULAR MATTER 10 : ");
        display.setTextSize(2);
        display.setCursor(0,20);
        display.print(PM10);
        display.print(" ");
        //  display.setTextSize(1);
        //  display.cp437(true);
        //  display.write(167);
        display.setTextSize(2);
        display.print("PPM");
         display.display();
         delay(2000);
    
   }  
void diaply_maqhi_oled(){
        display.clearDisplay();
        //=========display CO==================
        display.setTextSize(2);
        display.setCursor(0,0);
        display.print("MAQHI: ");
        display.setTextSize(2);
        display.setCursor(5,40);
        display.print(MAQHI);
        display.print(" ");
        display.setTextSize(2);
        display.print("%");
         display.display();
  }

void update_data_Thingspeak(){
        Serial1.println("AT");         updateSerial(); delay(1000); // handshake and wait 
        Serial1.println("AT+CPIN?");   updateSerial();delay(1000);  // check weather SIM is inserted 
        Serial1.println("AT+CREG?");   updateSerial();delay(1000);  // check network Registation status
        Serial1.println("AT+CGATT?");  updateSerial();delay(1000);  // check status of Packet Service
        Serial1.println("AT+CIPSHUT"); updateSerial();delay(1000);  // close the GPRS PDP context.
        Serial1.println("AT+CIPSTATUS");                            // TCP/UDP/SSL connection status and information
        delay(2000);
        updateSerial();
        Serial1.println("AT+CIPMUX=0");                             // start single IP or multi-IP connection
        delay(2000);
        updateSerial();
        Serial1.println("AT+CSTT=\"mobitel\"");                     //start task and setting the APN,
        delay(1000);
        updateSerial();
        Serial1.println("AT+CIICR");                                //bring up wireless connection
        delay(3000);
        updateSerial();
        Serial1.println("AT+CIFSR");                                //get local IP adress
        delay(2000);
        updateSerial();
        Serial1.println("AT+CIPSPRT=0");                            // echo prompt ">" after issuing "AT+CIPSEND" command
        delay(3000);
        updateSerial();
        Serial1.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");  //start up the connection
        delay(1000); 
        updateSerial();
        Serial1.println("AT+CIPSEND");                              //begin send data to remote server
        delay(1000);
        updateSerial(); 
        String str="GET https://api.thingspeak.com/update?api_key=5IT5SW243PTTATGT&field1="+String(dht11_temp)+"&field2="+String(dht11_hum)+"&field3="+String(CO)+"&field4="+String(MQ135)+"&field5="+String(PM2_5)+"&field6="+latitude+"&field7="+longitude+"&field8="+String(MAQHI);
        Serial.println(str);
        Serial1.println(str);                                       //begin send data to remote server
        delay(1000);
        updateSerial();
        Serial1.println("AT+CIPSEND");                              //begin send data to remote server
        delay(1000);
        updateSerial();
        Serial1.println((char)26);                                  //sending
        delay(5000);                                                //waitting for reply, important! the time is base on the condition of internet 
        Serial1.println();
        updateSerial();
        Serial1.println("AT+CIPSHUT");                              //close the connection/ repaly = SHUT OK
        delay(100);
        updateSerial();}
void update_data_maqhi(){
  Serial1.println("AT");         updateSerial(); delay(1000); // handshake and wait 
        Serial1.println("AT+CPIN?");   updateSerial();delay(1000);  // check weather SIM is inserted 
        Serial1.println("AT+CREG?");   updateSerial();delay(1000);  // check network Registation status
        Serial1.println("AT+CGATT?");  updateSerial();delay(1000);  // check status of Packet Service
        Serial1.println("AT+CIPSHUT"); updateSerial();delay(1000);  // close the GPRS PDP context.
        Serial1.println("AT+CIPSTATUS");                            // TCP/UDP/SSL connection status and information
        delay(2000);
        updateSerial();
        Serial1.println("AT+CIPMUX=0");                             // start single IP or multi-IP connection
        delay(2000);
        updateSerial();
        Serial1.println("AT+CSTT=\"mobitel\"");                     //start task and setting the APN,
        delay(1000);
        updateSerial();
        Serial1.println("AT+CIICR");                                //bring up wireless connection
        delay(3000);
        updateSerial();
        Serial1.println("AT+CIFSR");                                //get local IP adress
        delay(2000);
        updateSerial();
        Serial1.println("AT+CIPSPRT=0");                            // echo prompt ">" after issuing "AT+CIPSEND" command
        delay(3000);
        updateSerial();
        Serial1.println("AT+CIPSTART=\"TCP\",\"asql102.epizy.com\",\"80\"");  //start up the connection
        delay(6000); 
        updateSerial();
        Serial1.println("AT+CIPSEND");                              //begin send data to remote server
        delay(10000);
        updateSerial(); 
         String str="http://maqhi.epizy.com/receive_data.php?api_key=WR5HG9FHWEVQ6GYQ&co="+String(30)+"&temp="+String(30);
         //http://maqhi.epizy.com/receive_data.php?api_key=WR5HG9FHWEVQ6GYQ&co=10.02&temp=13.0
        Serial.println(str);
        Serial1.println(str);                                       //begin send data to remote server
        delay(10000);
        updateSerial();
        Serial1.println("AT+CIPSEND");                              //begin send data to remote server
        delay(10000);
        updateSerial();
        Serial1.println((char)26);                                  //sending
        delay(5000);                                                //waitting for reply, important! the time is base on the condition of internet 
        Serial1.println();
        updateSerial();
        Serial1.println("AT+CIPSHUT");                              //close the connection
        delay(100);
        updateSerial();}

 
