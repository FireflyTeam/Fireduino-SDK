 /**
 * @file ADXL345.ino
 * @brief Definitions ADXL345 Demo for Fireduino
 * @author jiang<jdz@t-chip.com.cn> 
 * @version V1.0
 * @date 2016.02
 * 
 * @par Copyright:
 * Copyright (c) 2016 T-CHIP INTELLIGENCE TECHNOLOGY CO.,LTD. \n\n
 *
 * For more information, please visit website <http://www.t-firefly.com/>, \n\n
 * or email to <service@t-firefly.com>.
 */ 
#include <Wire.h>  //调用arduino自带的I2C库

 
#define Register_ID 0
#define Register_2D 0x2D
#define Register_X0 0x32
#define Register_X1 0x33
#define Register_Y0 0x34
#define Register_Y1 0x35
#define Register_Z0 0x36
#define Register_Z1 0x37
 

 
int ADXAddress = 0xA7>>1;  //转换为7位地址
int reading = 0;
int val = 0;
int X0,X1,X_out;
int Y0,Y1,Y_out;
int Z1,Z0,Z_out;
double Xg,Yg,Zg;
 
void setup()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("setup");  
  Wire.begin();  //初始化I2C
  delay(100);
  Wire.beginTransmission(ADXAddress);
  Wire.write(Register_2D);
  Wire.write(8);
  Wire.endTransmission();
}
 
void loop()
{
  Wire.beginTransmission(ADXAddress);
  Wire.write(Register_X0);
  Wire.write(Register_X1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2);
  if(Wire.available()<=2);
  {
    X0 = Wire.read();
    X1 = Wire.read();
    X1 = X1<<8;
    X_out = X0+X1;
  }
 
  Wire.beginTransmission(ADXAddress);
  Wire.write(Register_Y0);
  Wire.write(Register_Y1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2);
  if(Wire.available()<=2);
  {
    Y0 = Wire.read();
    Y1 = Wire.read();
    Y1 = Y1<<8;
    Y_out = Y0+Y1;
  }
 
  Wire.beginTransmission(ADXAddress);
  Wire.write(Register_Z0);
  Wire.write(Register_Z1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2);
  if(Wire.available()<=2);
  {
    Z0 = Wire.read();
    Z1 = Wire.read();
    Z1 = Z1<<8;
    Z_out = Z0+Z1;
  }
 
  Xg = X_out/256.00;//把输出结果转换为重力加速度g,精确到小数点后2位。
  Yg = Y_out/256.00;
  Zg = Z_out/256.00;
  Serial.println(Xg);
  Serial.println(Yg);
  Serial.println(Zg);
   Serial.println(X_out);
  Serial.println(Y_out);
  Serial.println(Z_out);
  delay(1000);  //延时0.3秒，刷新频率这里进行调整
 
}

