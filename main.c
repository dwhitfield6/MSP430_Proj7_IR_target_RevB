#include "msp430.h"


#define LED0 BIT3
#define LED1 BIT4
#define IRLED1 BIT5
#define IRrec BIT1
#define IRrec1 BIT0
#define debug BIT4


void tx_send(long irdata);

int count =0;
int loopcount =0;
int loopcountmax =0;
int resetcount =0;
int one =0;
int two =0;
int three =0;
int four =0;
int five =0;
int six =0;
int seven =0;
int eight =0;
int checksum =0;
int time1 = 0;
int time2 = 0;
int time3 = 0;
int time4 = 0;
int bitt =0;
int timercount =990;
int ledcount =990;
int sendcount =990;
int odd =0;

int i=0;
int ii=0;
int iii=0;
int iiii =0;
int bit =0;

unsigned long data = 0;
unsigned long irdata = 0;
unsigned long realdata = 0;
unsigned long rxdata = 0;

void main(void){

  WDTCTL = WDTPW + WDTHOLD;                            // Stop watchdog timer

  P1DIR |= (debug + IRLED1);                            // Set all pins but RXD to output
  P2DIR |= (LED0 + LED1);
  P1OUT =0;
  P2OUT=0;
  P2IE |= IRrec; // P1.3 interrupt enabled
  P2IES &= ~IRrec; // P1.3 interrupt enabled
  P2IFG &= ~IRrec; // P1.3 IFG cleared
  P2IE |= IRrec1; // P1.3 interrupt enabled
  P2IES |= IRrec1; // P1.3 interrupt enabled
  P2IFG &= ~IRrec1; // P1.3 IFG cleared

  TA1CCTL0 |= CCIE;                             // CCR0 interrupt enabled
  TA1CTL = TASSEL_2 + MC_1 + ID_2;           // SMCLK/8, upmode
  TA1CCR0 =  20;



  __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts



  while(1)
  {
    // Wait for incoming character

    // Enter low poser mode

    _delay_cycles(100);

    if(resetcount == 200)
    {
    	loopcountmax=loopcount;
      loopcount =0;
      realdata = rxdata >> 1;

    }

    resetcount++;
    if(resetcount >1000)
    {
      resetcount =990;
    }
    if(realdata == 0xFF17943 || realdata == 0xFF46731)
    {
      ledcount = 0;
      P2OUT |=LED1;
      P2OUT &=~LED0;
      realdata =0;
    }
    if(ledcount == 5 && sendcount > 700)
    {
        P2OUT |=LED1;
        P2OUT &=~LED0;
    	_delay_cycles(100000);
      sendcount=0;
      tx_send(0x1926F5B9);
    }
    if(ledcount==500)
    {

      P2OUT |=LED0;
      P2OUT &=~LED1;
    }
    if(ledcount>900)
    {

      P2OUT &=~LED0;
      P2OUT &=~LED1;
    }

    ledcount++;
    if(ledcount>1000)
    {
      ledcount =990;
    }
    sendcount++;
    if(sendcount>1000)
    {
      sendcount =990;
    }
    if(count >1000)
    {
      count=0;
    }

  }
}




#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  P1OUT ^= debug;
  TA1CTL |= TACLR;

  if(loopcount == 0)
  {
    rxdata =0;
  }
  if (loopcount == 50)
  {
    one = count;
  }
  else if (loopcount ==51)
  {
    two = count;
  }
  else if(loopcount ==52)
  {
    three = count;
  }
  else if(loopcount ==53)
  {
    four = count;
  }
  else if(loopcount == 54)
  {
    five = count;

  }
  else if(loopcount == 55)
  {
    six = count;

  }
  else if(loopcount == 56)
  {
    seven = count;

  }
  else if(loopcount == 57)
  {
    eight = count;

  }

  odd = (loopcount & 0x1);
  if(odd == 0)
  {
    	if(count >13 && count < 24)
    		{
    			bit = 1;
    		}
    	else
    	{
			bit = 0;
    	}

    		    rxdata += bit;
    		    rxdata<<=1;
  }

loopcount++;
  count=0;
  resetcount=0;
  P2IFG &= ~IRrec; // P1.3 IFG cleared
  P2IFG &= ~IRrec1; // P1.3 IFG cleared
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
  count++;
}

void tx_send(long irrealdata)
{


  __disable_interrupt(); // enable all interrupts
//  irdata = irrealdata;

for(iii=32;iii>0;iii--)
{
	bitt = irrealdata & 1;
	irdata |= bitt;
	irrealdata>>=1;
	irdata<<=1;
}

  for (i = 322;i>0;i--)
  {
    P1OUT |= IRLED1;
    __delay_cycles(4);
    P1OUT &= ~IRLED1;
    __delay_cycles(4);

  }

    __delay_cycles(3830);


  for(ii = 32; ii>0;ii--)
  {
    bit = irdata & 1;
    if(bit == 0)
    {
        __delay_cycles(500);
      for (i = 21;i>0;i--)
      {
        P1OUT |= IRLED1;
        __delay_cycles(4);
        P1OUT &= ~IRLED1;
        __delay_cycles(4);

      }



    }
    else
    {

        __delay_cycles(1590);

      for (i = 21;i>0;i--)
      {
        P1OUT |= IRLED1;
        __delay_cycles(4);
        P1OUT &= ~IRLED1;
        __delay_cycles(4);


      }



    }
    irdata >>= 1;
  }

  for (i = 5;i>0;i--)
  {
    P1OUT |= IRLED1;
    __delay_cycles(4);
    P1OUT &= ~IRLED1;
    __delay_cycles(4);


  }


    __delay_cycles(1590);

    for (i = 20;i>0;i--)
    {
      P1OUT |= IRLED1;
      __delay_cycles(4);
      P1OUT &= ~IRLED1;
      __delay_cycles(4);

    }

  __enable_interrupt(); // enable all interrupts
}
