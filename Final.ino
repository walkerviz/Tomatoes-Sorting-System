#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <LiquidCrystal_I2C.h>
#include <HX711_ADC.h>
#include <Servo.h>

HX711_ADC LoadCell(6, 7); // DT pin 6, SCK pin 7
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);
LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo servo_can;
Servo servo_ok;
Servo servo_green;
int pos1 = 120;
int pos2 = 70;

char array1[] = "TOMATOES CLASSIFY>>>";
char array2[] = "TOTAL=";
char array3[] = " GOOD  GREEN  NOT-OK";
char array4[] = "  ";
char array5[] = "WEIGHT = ";

int s=0;  int ok=0; int gr=0; int no=0;
int color = 0; int color_r=0;

int can = 4;  int cb_can; float w=0; float wr=0;

int gt_ok = 8;  int cb_ok;
int gt_green = 12;  int cb_green;
int gt_color = 13; int cb_color;

int dc = 3; int speed = 200;
int buzzer = 2; 

uint16_t r, g, b, c, colorTemp, lux;
unsigned long t = 0;

void setup() {
  Serial.begin(112500);
  coi1();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,2);  lcd.print(array1);
  LoadCell.begin();
  LoadCell.start(2000);
  ktcb();
  LoadCell.setCalFactor(304.80);

  servo_can.attach(5); 
  servo_can.write(0);
  servo_ok.attach(9); 
  servo_ok.write(pos2);
  servo_green.attach(10); 
  servo_green.write(pos2);

  pinMode(can, INPUT);
  pinMode(gt_ok, INPUT);
  pinMode(gt_green, INPUT);
  pinMode(gt_color, INPUT);

  pinMode(dc, OUTPUT);
  analogWrite(dc, 0);
  pinMode(buzzer, OUTPUT);  
}

void loop() {
  if(w<0.0) w=0.0;
  get_weight();
  analogWrite(dc, speed);
  cb_can = digitalRead(can);
  cb_ok = digitalRead(gt_ok);
  cb_green = digitalRead(gt_green);
  cb_color = digitalRead(gt_color);
  get_color();
  lenh_can();
  lenh_color();
  in_lcd();
}

void ktcb(){
  if (tcs.begin()) {
    lcd.clear();
    lcd.setCursor(0,2);  lcd.print("  Found sensor ");
    delay(500);
  } else {
    lcd.clear();
    lcd.setCursor(0,1);  lcd.print("Check connection");
    lcd.setCursor(0,2);  lcd.print("NoTCS34725 found");
    while (1);
  }

  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    lcd.clear();
    lcd.setCursor(0,2);  lcd.print("    Timeout    ");
    while (1);
  }
  else {
    LoadCell.setCalFactor(1.0);
    lcd.clear();
    lcd.setCursor(0,2);  lcd.print("   Completed   ");
    delay(500);
  }
  while (!LoadCell.update());
}

void in_lcd(){
  lcd.clear();
  lcd.setCursor(0,1);  lcd.print(array2);  lcd.print(s);
  lcd.setCursor(0,2);  lcd.print(array3);
  lcd.setCursor(0,3);  lcd.print(array4);
  lcd.print(ok);
  lcd.print("     ");
  lcd.print(gr);
  lcd.print("       ");
  lcd.print(no);
  delay(200);
}

void lenh_can(){
  if(cb_can==0 && cb_ok==1 && cb_green==1 && color_r==0 && cb_color==1){
    coi2();
    analogWrite(dc, 0);
    for(int i=0; i<10; i++){
      for(int j=0; j<20000; j++)  get_weight();
    }
    wr=w;
    lcd.clear();
    lcd.setCursor(0,2);  lcd.print(array5);
    lcd.print(w);  lcd.print(" (g)");
    delay(1000);
    servo_can.write(pos1);
    delay(1000);
    servo_can.write(0);
    s++;
    delay(1000);
  }
  else{
    analogWrite(dc, speed);
  }
}

void lenh_color(){
  ktmau();
  if(wr>80.0){
    if(color_r==1){
      if(cb_ok==0){
        servo_ok.write(0);
        delay(2000);
        servo_ok.write(pos2);
        delay(2000);
        color_r=0;
      }
    }
    if(color_r==2){
      if(cb_green==0){
        servo_green.write(0);
        delay(2000);
        servo_green.write(pos2);
        delay(2000);
        color_r=0;
      }
    }
  }
}

void ktmau(){
  if(wr<=80.0) color_r=0;
  if((ok+gr+no)<s && cb_color==0){
    if(wr<=80.0) no=s-ok-gr;
    else{
      if(color==1){
        color_r=1;
        ok++;
      }
      else if(color!=1){
        color_r=2;
        gr++;
      }
    }
  }
}

void get_weight(){
  static boolean newDataReady = 0;
  if (LoadCell.update()) newDataReady = true;
  if (newDataReady) {
      w = LoadCell.getData();
      newDataReady = 0;
  }
}

void get_color(){
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b); //Nhiệt độ màu theo thang đo Kelvin
  lux = tcs.calculateLux(r, g, b); //Độ rọi soi, cường độ sáng
  if(r>g && r>b && r<c && lux>10 && lux<100)      color = 1;  //red    
  else if(g>r && g>b && g<c && lux>200 && lux<300)      color = 2; //green
  else color=0;
}

void coi1(){
  tone(buzzer, 3500, 200);
  delay(200);
  tone(buzzer, 4000, 100);
  delay(200);
  noTone(buzzer);
  delay(200);
}

void coi2(){
  tone(buzzer, 4000, 100);
  delay(200);
  noTone(buzzer);
  delay(200);
  delay(600);
}