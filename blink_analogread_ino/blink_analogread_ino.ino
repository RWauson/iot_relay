#include <FreeRTOS_SAMD21.h>

// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskAnalogRead( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);
 
    vNopDelayMS(1000); // prevents usb driver crash on startup, do not omit this
    while(!Serial);  // Wait for Serial terminal to open port before starting program
 
    Serial.println("");
    Serial.println("******************************");
    Serial.println("        Program start         ");
    Serial.println("******************************");
  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  (const char *)"Blink"   // A name just for humans
    ,  128  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );

  xTaskCreate(
    TaskAnalogRead
    ,  (const char *) "AnalogRead"
    ,  128 // This stack size can be checked & adjusted by reading Highwater
    ,  NULL
    ,  1  // priority
    ,  NULL );
vTaskStartScheduler();
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

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

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  Serial.println("TaskBlink");
  // initialize digital pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    fastBlink();
    fastBlink();
    fastBlink();
    
    slowBlink();
    slowBlink();
    slowBlink();
    
    fastBlink();
    fastBlink();
    fastBlink();
    
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskAnalogRead(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  Serial.print("TaskAnalogRead");
  for (;;)
  {
   // Serial.print("read");
    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);
    // print out the value you read:
    //Serial.println(sensorValue);
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}
