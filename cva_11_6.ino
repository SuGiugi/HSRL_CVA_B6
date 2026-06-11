#include "MatrixMiniR4.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  MiniR4.begin();
  MiniR4.M1.setReverse(true);
  MiniR4.M2.setReverse(false);
}

void dichuyen(int m1,int m2, int dg){
  MiniR4.M1.resetCounter();
  MiniR4.M1.setSpeed(m1);
  MiniR4.M2.resetCounter();
  MiniR4.M2.setSpeed(m2);
  while((abs(MiniR4.M1.getDegrees()) + abs(MiniR4.M2.getDegrees())) / 2 <= dg);
  MiniR4.M1.setSpeed(0);
  MiniR4.M2.setSpeed(0);
}
// m1 m2 quangduong duongkinhbanhxe (dichuyen_cm)

void dichuyen_cm(int m1, int m2, int dk, int qd){
  MiniR4.M1.resetCounter();
  MiniR4.M1.setSpeed(m1);
  MiniR4.M2.resetCounter();
  MiniR4.M2.setSpeed(m2);
  // int dg = 0;
  // while(((dk*3.14)*(dg/360)) <= qd) {
  //   dg = (abs(MiniR4.M1.getDegrees()) + abs(MiniR4.M2.getDegrees())) / 2;
  // }
  while((abs(MiniR4.M1.getDegrees()) + abs(MiniR4.M2.getDegrees())) / 2 <= qd * 360 / (3.14 * dk));
  MiniR4.M1.setSpeed(0);
  MiniR4.M2.setSpeed(0);
}

void dichuyen_s(int m1, int m2, int s){
  MiniR4.M1.resetCounter();
  MiniR4.M1.setSpeed(m1);
  MiniR4.M2.resetCounter();
  MiniR4.M2.setSpeed(m2);
  delay(s);
  MiniR4.M1.setSpeed(0);
  MiniR4.M2.setSpeed(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(1) {
      if (MiniR4.BTN_UP.getState()) {
        break;
      }
  }
  for(int i = 0; i < 4; i++){
    dichuyen_cm(100, 100, 6.5, 3);
    MiniR4.M1.setSpeed(0);
    MiniR4.M2.setSpeed(0);
    delay(500);
    dichuyen(100, -100, 90);
    MiniR4.M1.setSpeed(0);
    MiniR4.M2.setSpeed(0);
    delay(500);
    delay(500);
  }
  
  
}
