// Robot eyes using the 0.71inch DualEye LCD Module from Waveshare connected to ESP32-C3-Zero board 

// created by upir, 2025
// youtube channel: https://www.youtube.com/upir_upir

// YouTube Video: https://youtu.be/5uTRdr3TxKE
// Source Files: https://github.com/upiir/dual_lcd_robot_eyes/

// Links from the video:
//  Do you like this video? You can buy me a coffee ☕: https://www.buymeacoffee.com/upir
//  Double EYE 0.71 inch Round LCD Display: https://s.click.aliexpress.com/e/_olZD5lr
//  ESP32C3 Zero: https://s.click.aliexpress.com/e/_omPm1vJ
//  LCD Image Converter: https://lcd-image-converter.riuson.com/en/about/
//  Image2cpp (convert array to image): https://javl.github.io/image2cpp/
//  Photopea (online graphics editor like Photoshop): https://www.photopea.com/
//  Display Documentation: https://www.waveshare.com/wiki/0.71inch_DualEye_LCD_Module
//  AI Image Generator: https://deepai.org/machine-learning-model/text2img
//  Interesting plugin for creating human eyes: https://tinynocky.gumroad.com/l/tinyeye
// RGB565 Color Picker: https://rgbcolorpicker.com/565

// Related videos:
// Dasai Mochi Robot: https://youtu.be/QOoszpg0BsM
// Robot Eyes with LED Matrix Display: https://youtu.be/If-hiuwsQh0


   #include <SPI.h>
   #include <TFT_eSPI.h> 
   #include "test_image.h"
   #include "robot_eyes.h"
   
   TFT_eSPI tft = TFT_eSPI();    
    
   #define device_A_CS  2              
   #define device_B_CS  6 
                         
   
   #define BL1 1
   #define BL2 3

// some extra colors
   #define BLACK        0x0000
   #define BLUE         0x001F
   #define RED          0xF800
   #define GREEN        0x07E0
   #define CYAN         0x07FF
   #define MAGENTA      0xF81F
   #define YELLOW       0xFFE0
   #define WHITE        0xFFFF
   #define ORANGE       0xFBE0
   #define GREY         0x84B5
   #define BORDEAUX     0xA000
   #define DINOGREEN    0x2C86
   #define WHITE        0xFFFF

   int frameTime = 70;
   int j;

//extern void user_setup(void); // Functions in the user*.cpp files
//extern void user_loop(void);

#define SCREEN_X_START 0
#define SCREEN_X_END   SCREEN_WIDTH   // Badly named, actually the "eye" width!
#define SCREEN_Y_START 0
#define SCREEN_Y_END   SCREEN_HEIGHT  // Actually "eye" height

// A simple state machine is used to control eye blinks/winks:
#define NOBLINK 0       // Not currently engaged in a blink
#define ENBLINK 1       // Eyelid is currently closing
#define DEBLINK 2       // Eyelid is currently opening


#define BUFFER_SIZE 1024 // 128 to 1024 seems optimum

#ifdef USE_DMA
  #define BUFFERS 2      // 2 toggle buffers with DMA
#else
  #define BUFFERS 1      // 1 buffer for no DMA
#endif

uint16_t pbuffer[BUFFERS][BUFFER_SIZE]; // Pixel rendering buffer
                 // DMA buffer selection

uint32_t startTime; 

void pwm_init()
{
  //设置通道号，频率为1000HZ，分辨率为8位
  ledcSetup(1,1000,8);
  ledcSetup(2,1000,8);
  //将通道产生的PWM波映射到BL引脚上
  ledcAttachPin(BL1,1);
  ledcAttachPin(BL2,2);
}

void LED_PWM(unsigned char i)
{
  ledcWrite(1,i);
  ledcWrite(2,i);
  delay(10);
}

void led_breathing()
{
  for(int i = 0;i <= 255;i++)
  {
    ledcWrite(1,i);
    ledcWrite(2,i);
    delay(10);
  }
  for(int i = 255;i >= 0;i--)
  {
    ledcWrite(1,i);
    ledcWrite(2,i);
    delay(10);
  }
}

void PWM_test()
{
  digitalWrite (device_A_CS, LOW);
  tft.fillScreen (BLUE);                                             // we need to 'init' all displays
  digitalWrite (device_A_CS, HIGH);                                                                               
  //  tft.setRotation (3);

  digitalWrite (device_B_CS, LOW); 
  tft.fillScreen (RED);
  digitalWrite (device_B_CS, HIGH);   

}

void setup() {
 
   Serial.begin (9600); 
   for (j=0; j<10; j++)
      {
       Serial.println (""); 
       delay (20);
      }
    
   pinMode (BL1,OUTPUT);
   pinMode (BL2,OUTPUT);
   pinMode (device_A_CS, OUTPUT);
   pinMode (device_B_CS, OUTPUT);

   
   digitalWrite(BL1,HIGH);
   digitalWrite(BL2,HIGH);

   digitalWrite (device_A_CS, LOW);                                             // we need to 'init' all displays
   digitalWrite (device_B_CS, LOW);                                          
  //  digitalWrite (device_C_CS, LOW);                                          
   pwm_init();
   tft.init ();   
  //  tft.setRotation (3);
   tft.fillScreen (BLACK);
  //  tft.pushImage (0, 0,160, 160, bmp1);
   tft.setTextColor (YELLOW, BLACK); 

   digitalWrite (device_A_CS, HIGH);                                          
   digitalWrite (device_B_CS, HIGH);                                          

  // pwm_init(); 
  LED_PWM(255);                       
}


const uint16_t* robot_eyes_images[30] = {
	image_data_robot_eyes_01,
	image_data_robot_eyes_02,
	image_data_robot_eyes_03,
	image_data_robot_eyes_04,
	image_data_robot_eyes_05,
	image_data_robot_eyes_06,
	image_data_robot_eyes_07,
	image_data_robot_eyes_08,
	image_data_robot_eyes_09,      
	image_data_robot_eyes_10,        
	image_data_robot_eyes_11,
	image_data_robot_eyes_12,
	image_data_robot_eyes_13,
	image_data_robot_eyes_14,
	image_data_robot_eyes_15,
	image_data_robot_eyes_16,
	image_data_robot_eyes_17,
	image_data_robot_eyes_18,
	image_data_robot_eyes_19, 
	image_data_robot_eyes_20,   
	image_data_robot_eyes_21,
	image_data_robot_eyes_22,
	image_data_robot_eyes_23,
	image_data_robot_eyes_24,
	image_data_robot_eyes_25,
	image_data_robot_eyes_26,
	image_data_robot_eyes_27,
	image_data_robot_eyes_28,
	image_data_robot_eyes_29,             
	image_data_robot_eyes_30               
};


char i=1;
void loop (){
  
  for(int a=1;a<=4;a++){
    if(a==1){
    LED_PWM(255);
    digitalWrite (device_B_CS, LOW); 
    tft.setRotation (0);
    tft.fillScreen (BLUE);
    digitalWrite (device_B_CS, HIGH);

    digitalWrite (device_A_CS, LOW); 
    tft.setRotation (0);
    tft.fillScreen (RED);                                             
    digitalWrite (device_A_CS, HIGH);

    //Demo_1();
    delay(2000);
     
    }

    else if(a==2){
      for(i=0;i<30;i++){

        digitalWrite (device_A_CS, LOW);   
        tft.pushImage (0, 0,160,160,robot_eyes_images[i]);
        digitalWrite (device_A_CS, HIGH); 
        
        digitalWrite (device_B_CS, LOW);  
        tft.pushImage (0, 0,160,160,robot_eyes_images[i]);              
        digitalWrite (device_B_CS, HIGH);

        delay(2000);

      }
    }
  }
} 
