#include "PulsePosition.h"
#include "SatelliteReceiver.h"

const uint8_t rxPin = 5;
const uint8_t channels[6] = {1, 2, 3 , 4, 5, 6};
unsigned long RC_time;
unsigned long status_time;
unsigned long tmp_time; //temporary time
unsigned long report_time;
unsigned long LastActivePPM;
unsigned long tmpPPMtime; //temporary ppm time
int flashRate;
double channel[20];
double pitch_in; //channel 4
double roll_in;//channel 2
double throttle_in; //channel 3
double yaw_in; //channel 1
double mode_switch; //channel 5
double aux1; //channel 6
double aux2; //channel 7
double gear; //channel 8
int compd_pitch, compd_roll, compd_throttle;

PulsePositionInput rxIn;
PulsePositionOutput txOut;
SatelliteReceiver Rx;

bool PPMactive = 0;


void setup() {
txOut.begin(9);
PulsePositionInput txOut(RISING);
rxIn.begin(10);
PulsePositionInput rxIn(RISING);
PPMdefaults();
RC_time = millis();
report_time = millis();
}

void loop() {
  tmp_time = millis();

  if (tmp_time  > report_time + 10) {
    report();
  }
  if (tmp_time  > RC_time + 10) {
    RC();
  }
}

// --- Functions to be used ---

void readSpektrum() {
    Rx.getFrame();
    roll_in = Rx.getAile();
    pitch_in = Rx.getElev();
    throttle_in = Rx.getThro();
    yaw_in = Rx.getRudd();
    mode_switch = Rx.getFlap();
    aux1 = Rx.getGear();
    aux2 = Rx.getAux2();
}

void RC() {
  RC_time = millis();
  //Refresh RC inputs
  //readSpektrum();
  readPPM();
  if (PPMactive == 0) {
    PPMdefaults();
  } else {
    pitch_in = channel[2];
    roll_in = channel[1];
    throttle_in = channel[3];
    yaw_in = channel[4];
    mode_switch = channel[5];
    aux1 = channel[6];
  }
    //Do we want obstacle avoidance on?
    /*
  if (aux1 > 1400) {
    flashRate=125;
    compd_roll = (roll_in - int(Output_right) + int(Output_left));
    compd_pitch = (pitch_in - int(Output_rear) + int(Output_front));
    compd_roll = ConstrainPWM(compd_roll, 990, 2015);
    compd_pitch = ConstrainPWM(compd_pitch, 990, 2015);
    compd_throttle = throttle_in; //throttle passthrough
  }
  else {
    flashRate=500;
    compd_roll = roll_in;       //passthrough
    compd_pitch = pitch_in;     //passthrough
    compd_throttle = throttle_in; //passthrough
  }*/

  writePPM(); //Send values to Flight Controller
}

void readPPM() {
  int i, numCh;
  numCh = rxIn.available();
  if (numCh > 0) {
    PPMactive = 1;
    LastActivePPM = millis();
    for (i = 1; i <= 8; i++) {
      channel[i] = rxIn.read(i);
    }
  }
  else {
    //check how long it has been since last PPM signal
    tmpPPMtime = millis();
    if (tmpPPMtime > LastActivePPM + 108) { //DSMX frame is 22ms
      PPMactive = 0; 
    }
  }
}

void writePPM() {
  txOut.write(3, compd_throttle);
  txOut.write(2, compd_pitch);
  txOut.write(1, compd_roll);
  txOut.write(4, yaw_in);
  txOut.write(5, mode_switch);
  txOut.write(6, aux1);
  txOut.write(7, aux2);
  txOut.write(8, gear);
}

void PPMdefaults() {
  pitch_in = 1500; //channel 4
  roll_in = 1500; //channel 2
  throttle_in = 990; //channel 3
  yaw_in = 1500; //channel 1
  mode_switch = 990; //channel 5
  aux1 = 990; //channel 6
  aux2 = 990; //channel 7
  gear = 990; //channel 8
}

void printFunc( const char* Name, int Value, const char* Channel) {
  Serial.print(F("{TIMEPLOT:"));
  Serial.print(Channel);
  Serial.print(F("|data|"));
  Serial.print(Name);
  Serial.print(F("|T|"));
  Serial.print(Value);
  Serial.print(F("}"));
}

void report() {
  printFunc("roll_in",roll_in,"RC");
  printFunc("pitch_in",pitch_in,"RC");
  printFunc("yaw_in",yaw_in,"RC");
  printFunc("throttle_in",throttle_in,"RC");
  printFunc("mode_switch",mode_switch,"RC");
  printFunc("aux1",aux1,"RC");
}
