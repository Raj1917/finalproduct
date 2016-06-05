#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information

const String GMAIL_USER_NAME = "rajcomp1917@gmail.com";

// your Gmail App-Specific Password
const String GMAIL_PASSWORD = "eunurrxvkcyyymhq";

// the email address you want to send the email to, eg "jane.doe@temboo.com"
const String TO_EMAIL_ADDRESS = "rajiv.narayan5@gmail.com";

boolean success = false; // a flag to indicate whether we've sent the email yet or not
//When you have determined your threshold, declare your variable
//right below this line. This will change from person to person.
int sensorVal = 0;
int pulseRate;
unsigned long pulseCounter = 0;
unsigned long firstPulseStartTime = 0;
unsigned long secondPulseStartTime = 0;
unsigned long time_between_pulses = 0;
const unsigned long refractoryPeriod = 300;
const double minutes_in_milliseconds = 60000;
//When you have determined your threshold, declare your variable
//right below this line. This will change from person to person.
const double threshold = 0.6;

int count = 0;          //Count the number of people within the house
double temp;            //Variable to hold a temperature value

BridgeServer server;
String startString;
void setup() {
  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();
  Serial.begin(9600);
}
double Thermister(int RawADC) {  //Function to perform the fancy math of the Steinhart-Hart equation
 double Temp;
 Temp = log(((10240000/RawADC) - 10000));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;              // Convert Kelvin to Celsius
 //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Celsius to Fahrenheit - comment out this line if you need Celsius
 return Temp;
}
 
void loop() {
  // Get clients coming from server
  BridgeClient client = server.accept();

  if(client) {
    int pin, value;

    String command = client.readStringUntil('/');
    if (command == "led") {
      //also parseInt "consumes" the string
      pin = client.parseInt();
      value = client.parseInt();
      Serial.print(value);
      Serial.print("\n");
      analogWrite(pin, value);
    }
    else if ( command == "hearttest" ) {
      client.print(pulseRate);
    }
    else if ( command == "lightsensor") {
      client.print(analogRead(1));
    }
    else if (command == "getled") {
        //also parseInt "consumes" the string
        pin = client.parseInt();
        int status = digitalRead(pin);
        client.print(status);  
    }
    else if (command = "temp") {
        int val;                //Create an integer variable
        val=analogRead(0);      //Read the analog port 0 and store the value in val
        temp=Thermister(val);   //Runs the fancy math on the raw analog value
        client.print(temp);     //Send value to the website
    }
    disp();
    mail();
    //#-----------MAIN SETTINGS------------#//
    
    client.stop();
  } 
}
void mail() {
  if ( temp > -5 ) {
    success = false;
  }
  if (!success) {

    Serial.println("Running SendAnEmail...");
  
    TembooChoreo SendEmailChoreo;

    // invoke the Temboo client
    // NOTE that the client must be reinvoked, and repopulated with
    // appropriate arguments, each time its run() method is called.
    SendEmailChoreo.begin();
    
    // set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

    // identify the Temboo Library choreo to run (Google > Gmail > SendEmail)
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
 

    // set the required choreo inputs
    // see https://www.temboo.com/library/Library/Google/Gmail/SendEmail/ 
    // for complete details about the inputs for this Choreo

    // the first input is your Gmail email address
    SendEmailChoreo.addInput("Username", GMAIL_USER_NAME);
    // next is your Gmail App-Specific password.
    SendEmailChoreo.addInput("Password", GMAIL_PASSWORD);
    // who to send the email to
    SendEmailChoreo.addInput("ToAddress", TO_EMAIL_ADDRESS);
    // then a subject line
    SendEmailChoreo.addInput("Subject", "ALERT: Testmail");

     // next comes the message body, the main content of the email   
    SendEmailChoreo.addInput("MessageBody", "Test Main");

    // tell the Choreo to run and wait for the results. The 
    // return code (returnCode) will tell us whether the Temboo client 
    // was able to send our request to the Temboo servers
    unsigned int returnCode = SendEmailChoreo.run();

    // a return code of zero (0) means everything worked
    if (returnCode == 0) {
        Serial.println("Success! Email sent!");
        success = true;
    } else {
      // a non-zero return code means there was an error
      // read and print the error message
      while (SendEmailChoreo.available()) {
        char c = SendEmailChoreo.read();
        Serial.print(c);
      }
    } 
    SendEmailChoreo.close();

    // do nothing for the next 30 seconds
    delay(30000);
  }
  
}
void disp() {
  unsigned long timer = millis();
  
  sensorVal = analogRead(2);
  
  double voltage = convertToVoltage(sensorVal);
  
  double absorbance = calculateAbsorbance(voltage);
  
  long time_between_pulses = detectThreshold(absorbance);
  
  pulseRate = calculatePulseRate(time_between_pulses);
  //small delay to change our sampling rate
  //and stabilize our signal
  delay(25);  
}
double convertToVoltage(double ADC_Val)
{
  double volt = 0;
  volt = 9*(ADC_Val/1023);
  
  return volt;
}
double calculateAbsorbance(double volt)
{
  double absorbance = 0;
  absorbance = log10(5/volt);
  
  return absorbance;  
}
double calculatePulseRate(long time_between_pulses)
{
  return minutes_in_milliseconds/time_between_pulses;
}

long detectThreshold(double absorbance)
{
  if (millis() - firstPulseStartTime >= refractoryPeriod
    && absorbance >= threshold)
  {
    if (pulseCounter == 0)
    {
      pulseCounter++;
      firstPulseStartTime = millis();
    }
    else if (pulseCounter == 1)
    {
      secondPulseStartTime = millis();
      time_between_pulses = secondPulseStartTime - firstPulseStartTime;
      firstPulseStartTime = secondPulseStartTime;
    }
  }

  return time_between_pulses;
}


