
int strobe = 4;
int res = 5;
int left[7];
int right[7];
int band;
int cicle = 1;

int bandavg[7];
long bandavg_part[7];

int maxband[7];
int minband[7];

int threshold_bass = 80;
int threshold_mid = 80;
int threshold_high = 80;

float molt = 0.4;

int offcolor[3] = {};
int oncolor[3] = {};

// 2 = RED , 6  = GREEN, 9 = BLUE

int redpin = 2;
int greenpin = 6;
int bluepin = 9;

int bmtpins[3] = {bluepin, greenpin, redpin};

int basskick[3] = {255, 0, 255};
int midkick[3] = {0,0,255};
int highkick[3] = {255,255,255};

void setup()
{
  
  offcolor[0] = 0;
  offcolor[1] = 0;
  offcolor[2] = 0;
  
  oncolor[0] = 100;
  oncolor[1] = 255;
  oncolor[2] = 100;
  Serial.begin(115200);
 pinMode(res, OUTPUT); // reset
 pinMode(strobe, OUTPUT); // strobe
 digitalWrite(res,LOW); // reset low
 digitalWrite(strobe,HIGH); //pin 5 is RESET on the shield
 
 pinMode(bmtpins[0], OUTPUT);
 pinMode(bmtpins[1], OUTPUT);
 pinMode(bmtpins[2], OUTPUT);
 
 // 2 - RED
 // 6 - GREEN
 // 9 - BLUE
 
}
void readMSGEQ7()
{
 //Write the 7 bands (2 channels) and store them in an array
 digitalWrite(res, HIGH);
 digitalWrite(res, LOW);
 for(band=0; band <7; band++)
 {
   digitalWrite(strobe,LOW);
   delayMicroseconds(50);
   left[band] = analogRead(0);
   right[band] = analogRead(1);
   digitalWrite(strobe,HIGH); 
 }
}
void loop()
{
 readMSGEQ7();
 
 if(cicle/250 *250 == cicle)
 {
     for(int i = 0; i<sizeof(bandavg)/sizeof(bandavg[0]);i++)
     {
        bandavg[i] = bandavg_part[i]/250;
        bandavg_part[i] = 0;
     }
     
 }
 else
 {
   for(int i = 0; i<sizeof(bandavg)/sizeof(bandavg[0]);i++)
   {
      bandavg_part[i] += left[i];
      if(maxband[i] < left[i])
      {
         maxband[i] = left[i]; 
      }
      
      if(minband[i] > left[i])
      {
         minband[i] = left[i]; 
      }
      else
      {
         if(minband[i] == 0)
         {
           minband[i] = left[i];
         } 
      }
   }
   cicle++;
 }
 
 if(cicle == 5000)
 {
   /*for(int i = 0; i<sizeof(bandavg)/sizeof(bandavg[0]);i++)
   {
      maxband[i] = bandavg[i];
      minband[i] = bandavg[i];
   }*/
    cicle = 0;
    return;
 }
 
 Serial.print(bandavg[0]);
 Serial.print(" ");
 Serial.print(bandavg[4]);
 Serial.print(" ");
 Serial.println(bandavg[6]);
 
 //BASS!
 if(left[0] > (maxband[0] - minband[0])*molt)
 {
    
    //BASS KICK!
    analogWrite(redpin, basskick[0]);
    analogWrite(greenpin, basskick[1]);
    analogWrite(bluepin, basskick[2]);
    return;
 }
 
 if(left[6] > (maxband[6] - minband[6])*molt)
 {
    //TREBLE KICK!
    analogWrite(redpin, highkick[0]);
    analogWrite(greenpin, highkick[1]);
    analogWrite(bluepin, highkick[2]);
    return;
 }
   
 if(left[4] > (maxband[4] - minband[4])*molt)
 {
    //MID KICK!
    analogWrite(redpin, midkick[0]);
    analogWrite(greenpin, midkick[1]);
    analogWrite(bluepin, midkick[2]);
    return;
 }
 
 
 analogWrite(redpin, offcolor[0]);
 analogWrite(bluepin, offcolor[1]);
 analogWrite(greenpin, offcolor[2]);
 
}
