#include "MatrixMiniR4.h"

float error;
float last_error;
float correction;
float Cm_CanDi;

void ChieuBenTrai() {
  DICHUYEN_TOCDO_n_n(100, 10);
  MiniR4.RC1.setAngle(50);
  DICHUYEN_TOCDO_n_n(100, 30);
  for(int i_0 = 0; i_0 < 11; i_0++)
  {
    DoDuong_Trai_Cm_n_TocDo_n_KhoangCach_Mm_n_kp_n_kd_n(133, 100, 550, 0.5, 0.5);
    while(!((MiniR4.I2C3.MXColorV3.getR() > 120) && ((MiniR4.I2C3.MXColorV3.getB() < 90) && (MiniR4.I2C3.MXColorV3.getG() < 90))))
    {
      DoTuongTrai_TocDo_n_Khoangcach_mm_n_kp_n_kd_n(40, 550, 0.5, 0.5);
    }
    DICHUYEN_TOCDO_n_n(40, 10);
    MiniR4.RC1.setAngle(50);
    DICHUYEN_TOCDO_n_n(100, 30);
  }
  DoDuong_Trai_Cm_n_TocDo_n_KhoangCach_Mm_n_kp_n_kd_n(60, 100, 550, 0.5, 0.5);
  MiniR4.M1.setBrake(true);
}

void ChieuBenPhai() {
  DICHUYEN_TOCDO_n_n(40, 10);
  MiniR4.RC1.setAngle(130);
  DICHUYEN_TOCDO_n_n(100, 30);
  for(int i_1 = 0; i_1 < 11; i_1++)
  {
    DoDuongPhai_Cm_n_TocDo_n_KC_MM_n_kp_n_kd_n(133, 100, 500, 0.5, 0.5);
    while(!((MiniR4.I2C3.MXColorV3.getB() > 120) && ((MiniR4.I2C3.MXColorV3.getR() < 60) && (MiniR4.I2C3.MXColorV3.getG() < 90))))
    {
      BamTuongPhai_TocDo_n_KC_MM_n_kp_n_kd_n(40, 500, 0.5, 0.5);
    }
    DICHUYEN_TOCDO_n_n(40, 10);
    MiniR4.RC1.setAngle(130);
    DICHUYEN_TOCDO_n_n(100, 30);
  }
  DoDuongPhai_Cm_n_TocDo_n_KC_MM_n_kp_n_kd_n(60, 100, 500, 0.5, 0.5);
  MiniR4.M1.setBrake(true);
}

unsigned int CamData[10];
int CamStatus;
void DICHUYEN_TOCDO_n_n(float tocdo, float cm) {
  MiniR4.M1.resetCounter();
  MiniR4.M1.setSpeed(tocdo);
  while(!(MiniR4.M1.getDegrees() > ((cm / (3.14 * 6.24)) * 360)));
}

void DoTuongTrai_TocDo_n_Khoangcach_mm_n_kp_n_kd_n(float tocdo, float mm, float kp, float kd) {
  error = MiniR4.I2C1.MXLaserV2.getDistance() - mm;
  correction = (error * kp) + ((error - last_error) * kd);
  MiniR4.M1.setPower(tocdo);
  MiniR4.RC1.setAngle((90 + (correction / 3.5)));
  last_error = error;
}

void BamTuongPhai_TocDo_n_KC_MM_n_kp_n_kd_n(float tocdo, float mm, float kp, float kd) {
  error = MiniR4.I2C2.MXLaserV2.getDistance() - mm;
  correction = (error * kp) + ((error - last_error) * kd);
  MiniR4.M1.setPower(tocdo);
  MiniR4.RC1.setAngle((90 - (correction / 3.5)));
  last_error = error;
}

void DoDuong_Trai_Cm_n_TocDo_n_KhoangCach_Mm_n_kp_n_kd_n(float cm, float tocdo, float mm, float kp, float kd) {
  MiniR4.M1.resetCounter();
  MiniR4.M1.setPower(tocdo);
  while(!(MiniR4.M1.getDegrees() > ((cm / (3.14 * 6.24)) * 360)))
  {
    error = MiniR4.I2C1.MXLaserV2.getDistance() - mm;
    correction = (error * kp) + ((error - last_error) * kd);
    MiniR4.RC1.setAngle((90 + (correction / 3.5)));
    last_error = error;
  }
}

void DoDuongPhai_Cm_n_TocDo_n_KC_MM_n_kp_n_kd_n(float cm, float tocdo, float mm, float kp, float kd) {
  MiniR4.M1.resetCounter();
  MiniR4.M1.setPower(tocdo);
  while(!(MiniR4.M1.getDegrees() > ((cm / (3.14 * 6.24)) * 360)))
  {
    error = MiniR4.I2C2.MXLaserV2.getDistance() - mm;
    correction = (error * kp) + ((error - last_error) * kd);
    MiniR4.RC1.setAngle((90 - (correction / 3.5)));
    last_error = error;
  }
}

void camera() {
  if((int)CamData[0] == 0)
  {
    while(!((int)CamData[2] < 200))
    {
      MiniR4.M1.setPower(50);
    }
    MiniR4.RC1.setAngle(70);
    MiniR4.DriveDC.MoveDegs(50, 50, 360, true);
  }
}

void TranhKhoi_Do() {
  MiniR4.RC1.setAngle(135);
  DICHUYEN_TOCDO_n_n(50, 40);
  delay(200);
  MiniR4.RC1.setAngle(65);
  DICHUYEN_TOCDO_n_n(50, 40);
}

void TranhKhoi_Xanh() {
  MiniR4.RC1.setAngle(65);
  DICHUYEN_TOCDO_n_n(50, 40);
  delay(200);
  MiniR4.RC1.setAngle(135);
  DICHUYEN_TOCDO_n_n(50, 40);
}

void setup()
{
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2);
  Serial.begin(9600);
  MiniR4.OLED.clearDisplay();
  MiniR4.OLED.setTextSize(1);
  MiniR4.OLED.setTextColor(SSD1306_WHITE);
  MiniR4.Vision.Begin();
  MiniR4.I2C1.MXLaserV2.begin();
  MiniR4.I2C2.MXLaserV2.begin();
  MiniR4.I2C3.MXColorV3.begin();
  MiniR4.RC1.setHWDir(false);
  MiniR4.M1.setReverse(false);
}

void loop()
{
  while(!MiniR4.BTN_DOWN.getState());
  DoDuongPhai_Cm_n_TocDo_n_KC_MM_n_kp_n_kd_n(30, 100, 550, 0.4, 0.3);
  while(!(((MiniR4.I2C3.MXColorV3.getB() > 120) && ((MiniR4.I2C3.MXColorV3.getR() < 60) && (MiniR4.I2C3.MXColorV3.getG() < 90)))||((MiniR4.I2C3.MXColorV3.getR() > 120) && ((MiniR4.I2C3.MXColorV3.getB() < 90) && (MiniR4.I2C3.MXColorV3.getG() < 90)))))
  {
    BamTuongPhai_TocDo_n_KC_MM_n_kp_n_kd_n(40, 550, 0.4, 0.3);
  }
  MiniR4.M1.setBrake(true);
  if((MiniR4.I2C3.MXColorV3.getB() > 120) && ((MiniR4.I2C3.MXColorV3.getR() < 60) && (MiniR4.I2C3.MXColorV3.getG() < 90)))
  {
    ChieuBenPhai();
  }
  else
  {
    ChieuBenTrai();
  }

}