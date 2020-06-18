#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "nvs_flash.h"
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
//
//
//
////////////////////////////////////////////////
//
//   DEFINITIONS
//
////////////////////////////////////////////////
#define     LED_GREEN_PIN           27
#define     LED_RED_PIN             26



//
//
//
////////////////////////////////////////////////
//
//   TYPE VDEFINITIONS
//
////////////////////////////////////////////////
typedef     struct {
  int       led_pin_number ;
  int       Toggle_time ;  
  uint8_t   Led_Status ;
} LED_selection ;  
//
//
//
////////////////////////////////////////////////
//
//   ROUTINE PROTOTYPES 
//
////////////////////////////////////////////////
void     Toggle_one_LED( LED_selection *Current_Led );
//
//
//
////////////////////////////////////////////////
//
//   GLOBAL VARIABLES 
//
////////////////////////////////////////////////
int                 flag = 0 ;
TaskHandle_t        my_Task_Handle = NULL ; 
//
//
//
//
//
////////////////////////////////////
//
//   TASK 1 
//
////////////////////////////////////
void     Task_Toggle_LEDs( void *Current_Led ) 
{
  int  counter = 0 ;
  LED_selection  *led = ( LED_selection* )Current_Led  ;
  //
  while (1) {  
    printf(" Task_1 Counter = %d \n " , counter++ );       
    /*
    printf("  Current_Led->led_pin_number = %d \n"   , ( int )led->led_pin_number );
    printf("  Current_Led->Led_Status     = %d \n"   , ( uint8_t )led->Led_Status );
    printf("  Current_Led->Toggle_time    = %d \n\n" , ( int )led->Toggle_time );
    */
    /////////////////////////////////////////////////////////////////////////
    //
    //  pass the current parameters to the actual toggle led routine 
    //
    /////////////////////////////////////////////////////////////////////////
    //
    // Toggle_one_LED( ( LED_selection* )led );     OK SUCCESS
    //
    Toggle_one_LED( led );
    //
    vTaskDelay(  1000 / portTICK_RATE_MS );
  }
}
//
//
//
////////////////////////////////////
//
//   TOGGLE LEDs
//
////////////////////////////////////
void     Toggle_one_LED( LED_selection *Current_Led  ) 
{ //////////////////////////////////////////
  // Retreive the current led status 
  // and change it to ther opposite value 
  //////////////////////////////////////////
  if ( Current_Led->Led_Status == 0 ) {
    Current_Led->Led_Status = 1 ; 
    flag = 1 ; 
  }
  else {
    Current_Led->Led_Status = 0 ; 
    flag = 0 ;
  }
  //
  //////////////////////////////////////
  //   TOGGLE the selected LED PIN 
  //////////////////////////////////////
  if        ( Current_Led->led_pin_number == LED_GREEN_PIN ) {
    gpio_set_level( ( gpio_num_t )LED_GREEN_PIN , ( gpio_mode_t )flag );     
  }
  else if   ( Current_Led->led_pin_number == LED_RED_PIN ) {
    gpio_set_level( ( gpio_num_t )LED_RED_PIN , ( gpio_mode_t )flag );  
  }
  //
  printf("  Current_Led->led_pin_number = %d \n"   , ( int )Current_Led->led_pin_number );
  printf("  Current_Led->Led_Status     = %d \n"   , ( uint8_t )Current_Led->Led_Status );
  printf("  Current_Led->Toggle_time    = %d \n\n" , ( int )Current_Led->Toggle_time );
}
//
//
//
//
//
////////////////////////////////////
//
//   MAIN LOOP  
//
////////////////////////////////////
void app_main(void)
{
  ////////////////////////////////////
  //
  // STARTUP DELAY TIME AND INDICATION 
  //
  ////////////////////////////////////
  vTaskDelay(  1500 / portTICK_RATE_MS ); 
  printf("Hello world!\n");
  //
  ////////////////////////////////////
  //
  //  SETUP THE LED PINs AS PUTPUT
  //
  ////////////////////////////////////
  gpio_pad_select_gpio( ( gpio_num_t )LED_GREEN_PIN );
  gpio_set_direction( ( gpio_num_t )LED_GREEN_PIN , GPIO_MODE_OUTPUT );
  //
  gpio_pad_select_gpio( ( gpio_num_t )LED_RED_PIN );
  gpio_set_direction( ( gpio_num_t )LED_RED_PIN , GPIO_MODE_OUTPUT );
  //
  /////////////////////////////////////////////////
  //
  //  create the led selection parameters
  //
  /////////////////////////////////////////////////
  //
  //  GREEN LED SETUP
  //
  LED_selection GREEN_Led_selection = {
    .led_pin_number  = LED_GREEN_PIN ,
    .Toggle_time     = 1000 ,
    .Led_Status      = 0 
  };
  //
  //  RED LED SETUP
  //
  LED_selection RED_Led_selection = {
    .led_pin_number  = LED_RED_PIN ,
    .Toggle_time     = 1000 ,
    .Led_Status      = 1 
  };
  //  
  //  BELOW OK
  // xTaskCreate( &Task_Toggle_LEDs , "Task_GREEN"   , 2048 , ( void *)( ( LED_selection *)&RED_Led_selection ) , tskIDLE_PRIORITY , NULL ) ;      // &my_Task_Handle );
  //
  //  BELOW OK
  // xTaskCreate( &Task_Toggle_LEDs , "Task_GREEN"   , 2048 , ( LED_selection *)&RED_Led_selection , tskIDLE_PRIORITY , NULL ) ;
  //
  //  BELOW OK
  xTaskCreate( &Task_Toggle_LEDs , "Task_GREEN"   , 2048 , &GREEN_Led_selection , tskIDLE_PRIORITY , NULL ) ;
  xTaskCreate( &Task_Toggle_LEDs , "Task_RED"     , 2048 , &RED_Led_selection   , tskIDLE_PRIORITY , NULL ) ;
  //
  //
  //
  ////////////////////////////////////////////
  //
  //   MAIN LOOP TICK DELAY TIME 
  //
  ////////////////////////////////////////////
  while (1)
  {
    vTaskDelay( 1000 / portTICK_RATE_MS );
  }
}






