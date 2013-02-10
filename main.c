/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4x7_eth.h"
#include "netconf.h"
#include "main.h"
//#include "tcp_echoserver.h"
#include "telnetserver.h"
#include "serial_debug.h"
#include "ws2812.h"
#include "delay.h"
#include "stm32f4xx_it.h"
#include <stdio.h>
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
	uint32_t pos_r = 40;
	uint32_t pos_g = 20;
	uint32_t pos_b = 0;

	uint32_t random =0;

/* Private function prototypes -----------------------------------------------*/
void LED_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

	SystemInit();
	init_systick();

#ifdef SERIAL_DEBUG
  DebugComPort_Init();  
#endif

  /*Initialize LCD and Leds */ 
  LED_Init();
  
  /* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
  ETH_BSP_Config();
    
  /* Initilaize the LwIP stack */
  LwIP_Init();
  
  /* tcp echo server Init */
 // tcp_echoserver_init();
  telnetserver_init();


  ws2812_init();
  /* Initialize variables for ws2812 */

	delay_ms(20);




  /* Infinite loop */
  while (1)
  {  
    /* check if any packet received */

	  if (ETH_CheckFrameReceived())
    {
      /* process received ethernet packet */
      LwIP_Pkt_Handle();
    }
    /* handle periodic timers for LwIP */
    LwIP_Periodic_Handle(system_time);

    DRAW_LED();
  }   
}


void LED_Init(void)
{


  /* Initialize STM324xG-EVAL's LEDs */
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Leds on */
  STM_EVAL_LEDOn(LED5);
  STM_EVAL_LEDOn(LED6);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);
  

}


void DRAW_LED(void)
{
    uint8_t decision = 0;
       if (system_time%25000 <3000) {
           decision = 1;
       } else if (system_time%25000<6000) {
           decision = 2;
       } else  if (system_time%25000<9000) {
           decision = 3;
       } else  if (system_time%25000<19000) {
    	   decision = 4;
    	   srand(system_time);
       } else decision=5;
       switch (decision) {
           case 1:
               for (int i=0;i<FRAMEBUFFER_SIZE;i+=30) {
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].red=255;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].blue=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].green=0;
               }
               for (int i=10;i<FRAMEBUFFER_SIZE;i+=30) {
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].green=255;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].red=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].blue=0;
               }
               for (int i=20;i<FRAMEBUFFER_SIZE;i+=30) {
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].blue=255;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].red=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].green=0;
               }
               break;
           case 2:
               for (int i=0;i<FRAMEBUFFER_SIZE;i+=30) {
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].red=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].blue=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].green=255;
               }
               for (int i=10;i<FRAMEBUFFER_SIZE;i+=30) {
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].green=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].red=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].blue=255;
               }
               for (int i=20;i<FRAMEBUFFER_SIZE;i+=30) {
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].blue=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].red=255;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].green=0;
               }
               break;
           case 3:
               for (int i=0;i<FRAMEBUFFER_SIZE;i+=30) {
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].red=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].blue=255;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].green=0;
               }
               for (int i=10;i<FRAMEBUFFER_SIZE;i+=30) {
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].green=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].red=255;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].blue=0;
               }
               for (int i=20;i<FRAMEBUFFER_SIZE;i+=30) {
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].blue=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].red=0;
                   for (int j=0;j<10;j++) ws2812_framebuffer[i+j].green=255;
               }
               break;
           case 4:
        	   	  for(int i=0;i<FRAMEBUFFER_SIZE;i++) {
        	   		  uint8_t randnumber1=rand()%3;

        	   		  switch(randnumber1) {
        	   		  case 0:
        	   		  	  ws2812_framebuffer[i].red=255;
        	   		  	  ws2812_framebuffer[i].blue=0;
        	   		  	  ws2812_framebuffer[i].green=0;
        	   		  	  break;
        	   		  case 1:
    	   		  	  	  ws2812_framebuffer[i].red=0;
    	   		  	  	  ws2812_framebuffer[i].blue=255;
    	   		  	  	  ws2812_framebuffer[i].green=0;
    	   		  	  	  break;
        	   		  case 2:
    	   		  	  	  ws2812_framebuffer[i].red=0;
    	   		  	  	  ws2812_framebuffer[i].blue=0;
    	   		  	  	  ws2812_framebuffer[i].green=255;
    	   		  	  	  break;
        	   		  }
           	   		  delay_ms(2);
        	   	  }
        	   	  break;
           default:
               for (int i=0;i<FRAMEBUFFER_SIZE;i++) {
                   ws2812_framebuffer[i].red=ws2812_framebuffer[i].green=ws2812_framebuffer[i].blue=0;
               }

               //code for running light
               pos_r %= FRAMEBUFFER_SIZE;
               pos_g %= FRAMEBUFFER_SIZE;
               pos_b %= FRAMEBUFFER_SIZE;
               ws2812_framebuffer[pos_r].red = 255;
               if(pos_r>0) ws2812_framebuffer[pos_r-1].red = 64;
               if(pos_r>1)ws2812_framebuffer[pos_r-2].red = 10;

               ws2812_framebuffer[pos_g].green = 255;
               if(pos_g>0) ws2812_framebuffer[pos_g-1].green = 64;
               if(pos_g>1) ws2812_framebuffer[pos_g-2].green = 10;

               ws2812_framebuffer[pos_b].blue = 255;;
               if(pos_b>0) ws2812_framebuffer[pos_b-1].blue = 64;
               if(pos_b>1) ws2812_framebuffer[pos_b-2].blue = 10;

               delay_ms(20);

               ws2812_framebuffer[pos_r].red = 0;
               if(pos_r>0) ws2812_framebuffer[pos_r-1].red = 0;
               if(pos_r>1) ws2812_framebuffer[pos_r-2].red = 0;
               pos_r++;

               ws2812_framebuffer[pos_g].green = 0;
               if(pos_g>0) ws2812_framebuffer[pos_g-1].green = 0;
               if(pos_g>1) ws2812_framebuffer[pos_g-2].green = 0;
               pos_g++;

               ws2812_framebuffer[pos_b].blue = 0;
               if(pos_b>0) ws2812_framebuffer[pos_b-1].blue = 0;
               if(pos_b>1) ws2812_framebuffer[pos_b-2].blue = 0;
               pos_b++;

       }




}




/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
