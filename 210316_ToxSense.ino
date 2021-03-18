
//imports ------------------------------------------------------------------------------------------------

#include <base64.h>
#include "esp_camera.h"
#include "BluetoothSerial.h"
#include "HMC5883L.h"

//definitions and variables ------------------------------------------------------------------------------

BluetoothSerial SerialBT;

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


bool DEBUG=false;

//PWM properties
const int baseStrength=5;
const unsigned char freq=100;
const unsigned char pwm_resolution = 16; //16-bit resolution -> 0 - 65535 dutycycle
const int exciter_deg[6]={135,90,45,315,270,225};
float exciter_strength[6]={0,0,0,0,0,0};

const unsigned char exciter_pin[6]={13,12,2,4,15,14};//GPIO zuweisung

const char aqi_arraysize=4;
unsigned char min_dir=4;
float aqi_direction[aqi_arraysize];//={0,0,0,0};

float bearing=0.0;
String command;

bool RUN=false;
bool BT_ON=true;
char i=0;

const char bearing_size=3;
float last_bearing[bearing_size];
char o=0;

float stepsize=0.01;
//main thread---------------------------------------------------------------------------------------------

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  
  if(event == ESP_SPP_SRV_OPEN_EVT){
    RUN=true;
    }
  else if(event == ESP_SPP_CLOSE_EVT){
    
    RUN=false;
    
    BT_ON=false;
  }
}


void setup()
{
    camera_setup();
    for (int channel=2;channel<8;channel++){
        ledcSetup(channel, freq, pwm_resolution);
        ledcAttachPin(exciter_pin[channel-2], channel);
    }
    
    SerialBT.register_callback(callback);

    if(!SerialBT.begin("ESP32")){
      if(DEBUG==true)SerialBT.println("info:An error occurred initializing Bluetooth");
    }else{
      if(DEBUG==true)SerialBT.println("info:Bluetooth initialized");
    }
}

void loop(){
    
    if(RUN==true){
      inner_loop();
    }else{
      for(char e=0;e<6;e++){
        ledcWrite(e+2,0);
      }
      if(BT_ON==false){
        //SerialBT.register_callback(callback);
        if(!SerialBT.begin("ESP32")){
          if(DEBUG==true)SerialBT.println("info:An error occurred initializing Bluetooth");
        }else{
          if(DEBUG==true)SerialBT.println("info:Bluetooth initialized");
        }
        BT_ON=true;
      }
    }
    delay(10);
}

void inner_loop()
{
    

    
    for (char exciter_nmb=0;exciter_nmb<6;exciter_nmb++){
        exciter(exciter_nmb);
    }
    
      while(SerialBT.available()){
      
      command= SerialBT.readString();
      SerialBT.println("info: full command="+command);
      
      if(command.indexOf("snap") > 0 || command.substring(0,4) == "snap" || command=="snap"){
          
          take_picture();
          
      }else if(command.indexOf("d")>0 || command.substring(0,1) == "d"){
          String s1=split(command,'d',1);
          String inDirection=split(s1,'e',0);
          
          aqi_direction[0]=split(inDirection,',',0).toFloat();
          aqi_direction[1]=split(inDirection,',',1).toFloat();
          aqi_direction[2]=split(inDirection,',',2).toFloat();
          aqi_direction[3]=split(inDirection,',',3).toFloat();
          float minAQI=aqi_direction[0];
          float maxAQI=aqi_direction[0];
          min_dir=0;
          for(char i=0;i<aqi_arraysize;i++){
            if(aqi_direction[i]<minAQI){
              minAQI=aqi_direction[i];
              min_dir=i;
            }
            if(aqi_direction[i]>maxAQI){
              maxAQI=aqi_direction[i];
             }
          }
          for(char j=0;j<aqi_arraysize;j++){
            aqi_direction[j]-=minAQI;
            aqi_direction[j]=aqi_direction[j]/(maxAQI-minAQI);
          }
      }else if( command.indexOf("h")>0|| command.substring(0,1) =="h" ){
        
        if(DEBUG==true)SerialBT.println("info: got heading");
        String sHeading1=split(command,'h',1);
        String sHeading=split(sHeading1,'e',0);
        if(DEBUG==true)SerialBT.println("info: Sheading"+sHeading);
        float bearingRaw=sHeading.toFloat();
        if(DEBUG==true)SerialBT.println("info: bearing"+String(bearing));
        float bearing_rad=bearingRaw*PI/180;
        last_bearing[o]=bearing_rad;
        o++;
        if(o == bearing_size) o = 0;

        float x=0;
        float y=0;
        for(char l=0;l<bearing_size;l++){
          if (last_bearing[l]){
            x+=cos(last_bearing[l]);
            y+=sin(last_bearing[l]);
          }
        }
        bearing=atan2(y,x)*180/PI;

        if(DEBUG==true)SerialBT.println("info: bearingAvg"+String(bearing));
        
      }else{
        if(DEBUG==true)SerialBT.println("info: wrong command= "+command);
      }
  }

  if(i==10){
    SerialBT.println("getHeading");
    i=0;
  }
  i++;
}
//functions ---------------------------------------------------------------------------------------------
void take_picture(){
    
    camera_fb_t * frameBuffer = NULL;
    
    frameBuffer = esp_camera_fb_get();
    
    if (!frameBuffer) {
        return;
    }

    String buffer = base64::encode((uint8_t *) frameBuffer->buf, frameBuffer->len);
    //Serialln(buffer);
    SerialBT.println(buffer);

    esp_camera_fb_return(frameBuffer);
    ;
  }


void camera_setup(){
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 10;
    config.fb_count = 1;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        while (true) {}
    }
}

void exciter(unsigned char exciter_nmb){
    if(DEBUG==true && exciter_nmb==0)SerialBT.println("info: bearing"+String(bearing));
    int direct=exciter_deg[exciter_nmb]+bearing;
    if(direct>360)direct-=360;
    if(direct<0)direct+=360;
    if(min_dir!=4){
      if(min_dir==0){
        if(direct<=360 && direct>315){
          if(exciter_strength[exciter_nmb]<1){
            UP(exciter_nmb);
            
            //exciter_strength[exciter_nmb]=1;
            //ledcWrite(exciter_nmb+2,exciter_strength[exciter_nmb]*baseStrength);
          }
        }
        else if(direct<45 && direct >= 0){
          if(exciter_strength[exciter_nmb]<1){
            UP(exciter_nmb);
            
            //exciter_strength[exciter_nmb]=1;
            //ledcWrite(exciter_nmb+2,exciter_strength[exciter_nmb]*baseStrength);
          }
        }
        
        else if(direct>270 && direct<315){
          if(exciter_strength[exciter_nmb]!=0.5){
            if(exciter_strength[exciter_nmb]<0.5){
              UP(exciter_nmb);
            }
            else{
              DOWN(exciter_nmb);
            }
            
            //exciter_strength[exciter_nmb]=0.5;
            //ledcWrite(exciter_nmb+2,exciter_strength[exciter_nmb]*baseStrength);
          }
        }
        else if(direct>45 && direct<90){
          if(exciter_strength[exciter_nmb]!=0.5){
            if(exciter_strength[exciter_nmb]<0.5){
              UP(exciter_nmb);
            }
            else{
              DOWN(exciter_nmb);
            }
            
            //exciter_strength[exciter_nmb]=0.5;
            //ledcWrite(exciter_nmb+2,exciter_strength[exciter_nmb]*baseStrength);
          }
        }
        
        else{
          if(exciter_strength[exciter_nmb]>0){
            DOWN(exciter_nmb);
          }
          //exciter_strength[exciter_nmb]=0;
          //ledcWrite(exciter_nmb+2,0);
        }
      }else{
        int border1=90*min_dir-45;
        if(border1>360)border1-=360;
        if(border1<0)border1+=360;
        int border2=90*min_dir+45;
        if(border2>360)border2-=360;
        if(border2<0)border2+=360;
  
        int a_border1=border1-45;
        if(a_border1>360)a_border1-=360;
        if(a_border1<0)a_border1+=360;
  
        int a_border2=border2+45;
        if(a_border2>360)a_border2-=360;
        if(a_border2<0)a_border2+=360;
  
        if(direct<border2 && direct>border1){
          if(exciter_strength[exciter_nmb]<1){
            UP(exciter_nmb);
            
            //exciter_strength[exciter_nmb]=1;
            //ledcWrite(exciter_nmb+2,exciter_strength[exciter_nmb]*baseStrength);
          }
        }
        
        else if(direct<border2 && direct>a_border2){
          if(exciter_strength[exciter_nmb]!=0.5){
            if(exciter_strength[exciter_nmb]<0.5){
              UP(exciter_nmb);
            }
            else{
              DOWN(exciter_nmb);
            }
            
            //exciter_strength[exciter_nmb]=0.5;
            //ledcWrite(exciter_nmb+2,exciter_strength[exciter_nmb]*baseStrength);
          }
        }
        else if(direct>a_border1 && direct<border1){
          if(exciter_strength[exciter_nmb]!=0.5){
            if(exciter_strength[exciter_nmb]<0.5){
              UP(exciter_nmb);
            }
            else{
              DOWN(exciter_nmb);
            }
            
            //exciter_strength[exciter_nmb]=0.5;
            //ledcWrite(exciter_nmb+2,exciter_strength[exciter_nmb]*baseStrength);
          }
        }
        
        else{
          if(exciter_strength[exciter_nmb]>0){
            DOWN(exciter_nmb);
          }
          //ledcWrite(exciter_nmb+2,0);
        }
      }
    }
}

//void up and down
void UP(char exciter_nmb){
  exciter_strength[exciter_nmb]+=stepsize;
  ledcWrite(exciter_nmb+2,exciter_strength[exciter_nmb]*baseStrength);
}

void DOWN(char exciter_nmb){
  exciter_strength[exciter_nmb]-=stepsize;
  ledcWrite(exciter_nmb+2,exciter_strength[exciter_nmb]*baseStrength);
}
    
String split(String s, unsigned char parser, unsigned char index) {
  String rs = "";
  unsigned char parserIndex = index;
  unsigned char parserCnt = 0;
  char rFromIndex = 0, rToIndex = -1;
  while (index >= parserCnt) {
    rFromIndex = rToIndex + 1;
    rToIndex = s.indexOf(parser, rFromIndex);
    if (index == parserCnt) {
      if (rToIndex == 0 || rToIndex == -1) return "";
      return s.substring(rFromIndex, rToIndex);
    } else parserCnt++;
  }
  return rs;
}
