#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
String Data_SMS;
int k=0;
int a;
int b;
int c;
float d;
float e;
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing..."); 
  delay(1000);
  mySerial.println("AT"); 
  updateSerial();
  pinMode(9, INPUT);  //load detection
  pinMode(8, INPUT);  //AC detection
  pinMode(10, INPUT); //tamper detection
  pinMode(11, OUTPUT); //BATled
  pinMode(7, OUTPUT);  //GSMN/W
  pinMode(6, OUTPUT);  //LoadLED
  pinMode(A0, INPUT);  //Pressure sensor
  pinMode(A1, INPUT);  //Level sensor
}

void loop()
{
  while(k<10)
  {
    k+=1;
    delay(60000);
  a=digitalRead(9); //load
   b=digitalRead(8); //AC
   c=digitalRead(10); //tamper
   d=analogRead(A0); //Pressure
   e=analogRead(A1); //Level
  }
  Data_SMS = "Parameters = \n"+String(a,1)+" C \n"+String(b,1)+" C \n"+String(d,1)+" C \n"+String(e,1)+" %";  //parameters to send in string format
  Send_Data();                      //This function set the sending SMS mode, prepare the phone number to which we gonna send, and send "Data_SMS" String
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void Send_Data()
{
  Serial.println("Sending Data...");
  mySerial.print("AT+CMGF=1\r");          //Set the module to SMS mode
  delay(100);
  mySerial.print("AT+CMGS=\"+918896379952\"\r");
  delay(500);  
  mySerial.print(Data_SMS);  //This string is sent as SMS
  delay(500);
  mySerial.print((char)26);//Required to tell the module that it can send the SMS
  delay(500);
  mySerial.println();
  Serial.println("Data Sent.");
  delay(500);
}
