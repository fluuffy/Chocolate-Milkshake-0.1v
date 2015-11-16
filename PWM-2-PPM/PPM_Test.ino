// Sim: https://sandropilipovic3-gmail.tinytake.com/sf/MTUxMjMyXzkzOTA1Nw
int dlch1 = 0;
int dlch2 = 1;
int us1 = 2;
int us2 = 3;
int us3 = 4;
int us4 = 5;
int out1 = 6;
int out2 = 7;
int fs = 8; // faisafe in case of ultrasonic sensor failure OR avoiding system manual switch-off 
int  val = 0;
int  val1 = 0;
int  val2 = 0;
int  val3 = 0;
int  val4 = 0;
int  val5 = 0;
int  val6 = 0;



// define channels
void setup()
{
  pinMode (dlch1, INPUT);
  pinMode (dlch2, INPUT);
  pinMode (us1, INPUT);
  pinMode (us2, INPUT);
  pinMode (us3, INPUT);
  pinMode (us4, INPUT);
  pinMode (out1, OUTPUT);
  pinMode (out2, OUTPUT);
  pinMode (fs, INPUT);
}




void loop()

{
  
  // Stronger ping will always be in control
  // If obstacles are in front and back, ping will balance the drone in midle
  //valfs 300 is just a random number. In practice it will be naza failsafe channel with Y harness OR any flipswitch on taranis.  
  val1 = analogRead(us1);     // read the ultrasonic 1
  val2 = analogRead(us2);     // read the ultrasonic 2
  val3 = analogRead(dlch1);   // read the input channel from DL1
  int sensorVal1 = analogRead(us1);
  int sensorVal2 = analogRead(us2);
  int valfs = analogRead(fs); 
  
  if ((valfs < 300) && (val1 > 660) && (val1 > val2)) analogWrite(out1,  map(sensorVal1, 660, 1023, 512, 750)); // ULTRASONIC CONTROL OVERRIDE, BACK SENSOR
  if ((valfs < 300) &&(val2 > 660) && (val2 > val1)) analogWrite(out1, map(sensorVal2, 660, 1023, 512, 350)); // ULTRASONIC CONTROL OVERRIDE, FRONT SENSOR
  if (((val1 < 660) && (val2 < 660)) || (valfs > 300)) analogWrite(out1, val3 / 1); // PASSTHROUGH
  
  
  // Stronger ping will always be in control
  // If obstacles are in left and right, ping will balance the drone in midle
  val4 = analogRead(us3);     // read the ultrasonic 3
  val5 = analogRead(us4);     // read the ultrasonic 4
  val6 = analogRead(dlch2);   // read the input channel from DL2
  int sensorVal4 = analogRead(us3);
  int sensorVal5 = analogRead(us4);


  if ((valfs < 300) &&(val4 > 660) && (val4 > val5)) analogWrite(out2, map(sensorVal4, 660, 1023, 512, 750)); // ULTRASONIC CONTROL OVERRIDE, LEFT SENSOR
  if ((valfs < 300) &&(val5 > 660) && (val5 > val4)) analogWrite(out2, map(sensorVal5, 660, 1023, 512, 350)); // ULTRASONIC CONTROL OVERRIDE, RIGHT SENSOR
  if (((val4 < 660) && (val5 < 660)) || (valfs > 300)) analogWrite(out2, val6 / 1); // PASSTHROUGH
}
