
#include "arduino_secrets.h"
/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/bcf5a8a5-cc86-42a5-977c-da7255417f1e 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float room_temp;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
#include <FreeRTOS_SAMD21.h>

void update_cloud( void *pvParameters );
void set_value( void *pvParameters );

void do_blink(int val){
    vTaskDelay( val / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( val / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( val / portTICK_PERIOD_MS ); // wait for one second
}

void fastBlink(){
Serial.println("fast");
  do_blink(100);
}

void slowBlink(){
  Serial.println("slow");
  do_blink(300);
}

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  //------------------------Cloud--------------------------------
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  
  // set built in LED pin to output mode
  pinMode(LED_BUILTIN, OUTPUT);
  
  room_temp = 0;
    xTaskCreate(
    set_value
    ,  (const char *)"Set value"   // A name just for humans
    ,  10000  // Stack size
    ,  NULL
    ,  1  // priority
    ,  NULL );

  xTaskCreate(
    cloud_update
    ,  (const char *) "UpdateCloud"
    ,  10000 // This stack size can be checked & adjusted by reading Highwater
    ,  NULL
    ,  5  // priority
    ,  NULL );
    
  vTaskStartScheduler();
  
  fastBlink();
  fastBlink();
  slowBlink();
  slowBlink();
  fastBlink();
  fastBlink();
}

void loop()
{
  //See tasks
}


void cloud_update(void *pvParameters)  // This is a task.
{
  Serial.println("Cloud update");
  fastBlink();
  (void) pvParameters;
  ArduinoCloud.update();
  
  vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
}

void set_value(void *pvParameters)  // This is a task.
{
  room_temp += 1;
  if(room_temp > 100){
    room_temp = 0;
  }
  vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
}
