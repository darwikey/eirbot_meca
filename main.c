//  Code carte meca 2014  //
/*
  pour les assignations de sortie on utilisera PORT 
  pour les retours on utilisera PIN
  attention pour les conditions sur les retours il faudra penser a écrire if((PINB&0x0A)==0x0A) plutot que ((PINB&0x0A)==10 car possibilité d'écrire comme un débile ==1 
*/  

#include <aversive.h>
#include <time.h>
#include <scheduler.h>
#include <aversive/wait.h>
//#include "ax12_user.h"
#include <uart.h>
#include "../../test_unioc/com.h"
#include <multiservo.h>
//#include <gp2d12.h>
//#include <adc.h>
#include <i2cs.h>
 
void init_portAB(void);

int main(void)
{
  init_portAB();
  uart_init();
  time_init(128);
  scheduler_init();
  fdevopen(NULL, uart0_recv,1);
  sei();//autorise les interruptions  
  
  //init i2c
  i2cs_init(mecaADDR);
  wait_ms(10);
  i2cs_data[0]=mecaReady;
  i2cs_state=READY;  
  

  multiservo_init();
  int8_t servo_peigne = multiservo_add(&PORTB,0);
  int8_t servo_tiroir = multiservo_add(&PORTB,1);





  while(1)
    {
      if(i2cs_state==RECEIVED){
	//PORTA=0x0F;
	uint8_t action=i2cs_data[0];
	i2cs_data[0]=mecaBusy;
	i2cs_state= NONE;
	switch (action)
	  {
	  case TIROIR_FERMER:
	    multiservo_set(servo_tiroir, 1700);
	    break;
	  case TIROIR_OUVERT:
	    multiservo_set(servo_tiroir, 1000);
	    break;
	  case TIROIR_DEVERSER:
	    multiservo_set(servo_tiroir, 500);
	    break;
	  case PEIGNE_OUVERT:
	    multiservo_set(servo_peigne, 1700);
	    break;
	  case PEIGNE_FERMER:
	    multiservo_set(servo_peigne, 800);
	    break;
	  default:
	    break;
	  }
      }
      
   	i2cs_data[0]=mecaReady;
	i2cs_state=READY;
	wait_ms(50);
         

      /*
      multiservo_set(servo_tiroir, 1700);
      wait_ms(500);
      multiservo_set(servo_peigne, 800);
      wait_ms(18000);
      // peigne à 90° = 1700
      multiservo_set(servo_peigne, 1700);
      // position repliée du tiroir (angle à 90°) = 1700
      wait_ms(500);
      multiservo_set(servo_tiroir, 1200);

      wait_ms(10000);

      // peigne rentré = 800
      multiservo_set(servo_peigne, 800);
      wait_ms(20000);
      // tiroir ouvert = 1200
      multiservo_set(servo_tiroir, 500);

      */
      wait_ms(500);
      /*while(1)
	{
    
	// clignotement port A
	PORTA=0x00;
	wait_ms(100);

	PORTA=0xFF;
	wait_ms(100);
	   
	}
      */
      // position de deversement
      //multiservo_set(servo_tiroir, 500);

      
  

    }
  return 0;
}


void init_portAB(void)
{
  DDRA = 0xff;
  DDRB = 0xff;
}
