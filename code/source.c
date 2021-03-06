/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
� Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 7/6/2020
Author  : 
Company : 
Comments: 


Chip type               : ATmega64
Program type            : Application
AVR Core Clock frequency: 16.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 1024
*******************************************************/

#include <mega64.h>
#include <stdlib.h>
#include <delay.h>
#include <stdio.h>

// Alphanumeric LCD functions
#include <alcd.h>


// Declare your global variables here
unsigned char digit[] =
{
   0xfe,  //0
   0x4f,  //1
   0x12,  //2
   0x30,  //3
   0x68,  //4
   0x24,  //5
   0x04,  //6
   0x71,  //7
   0x00,  //8
   0x20   //9
};

int red_timer = 5;
int green_timer = 5;
int i ,input;
int delay_time = 100;
char str[20];

void green(){
     PORTB.2 = 1;
     for (i= green_timer ; i>0 ;i--){
        PORTA = digit[i];
        delay_ms(delay_time);
     }
     PORTB.2 = 0;
}
void red(){
    PORTB.0 = 1;
    for (i= red_timer; i>0 ;i--){
        PORTA = digit[i];
        delay_ms(delay_time);
    }
    PORTB.0=0;
}

void yellow(){
    PORTB.1 = 1;
    PORTA = digit[0];
    delay_ms(delay_time);
    PORTB.1 = 0;
}

void set_lcd(){
    lcd_clear();
    lcd_gotoxy(4 , 0); // start writing in 4th column and 2th row
    lcd_putsf("Control Box");  
    lcd_gotoxy(0 , 2);
    lcd_putsf("set              set");
    lcd_gotoxy(0 , 3);
    lcd_putsf("green            red");
}

int pressed_key()
{
    PORTD.2 = 0;PORTD.3 = 1;PORTD.4 = 1;     
    if(!PIND.5) return 1;
    if(!PIND.6) return 4;
    if(!PIND.7) return 7;

    PORTD.2=1; PORTD.3=0; PORTD.4=1;
    if(!PIND.5) return 2;
    if(!PIND.6) return 5;
    if(!PIND.7) return 8;
   
    PORTD.2=1; PORTD.3=1; PORTD.4=0;
    if(!PIND.5) return 3;
    if(!PIND.6) return 6;
    if(!PIND.7) return 9;
    
    return -1; //nothing has pressed      
}

// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void) // set red timer
{
    delay_ms(20);
    lcd_clear();
    lcd_gotoxy(0 , 0);
    lcd_putsf("Enter a number(1-9)");  
    lcd_gotoxy(0 , 1);
    lcd_putsf("for red timer");
    do{
        input = pressed_key();  
        if(input != -1){  
            lcd_clear();
        }
    }while(input == -1);   
    
    red_timer = input;
    lcd_gotoxy(0 ,0);
    lcd_puts("red timer is set");
    lcd_gotoxy(0 , 1);
    sprintf(str , "for %us" , input) ;
    lcd_puts(str);
    delay_ms(delay_time); 
    set_lcd();
}

// External Interrupt 1 service routine
interrupt [EXT_INT1] void ext_int1_isr(void)  //set green timer
{
    delay_ms(20);
    lcd_clear();
    lcd_gotoxy(0 , 0);
    lcd_putsf("Enter a number(1-9)");
    lcd_gotoxy(0 , 1);
    lcd_putsf("for green timer");
    do{
        input = pressed_key();  
        if(input != -1){  
            lcd_clear();
        }
    }while(input == -1);
    
    green_timer = input;
    lcd_gotoxy(0 ,0);
    lcd_puts("green timer is set");
    lcd_gotoxy(0 , 1);
    sprintf(str , "for %us" , input) ;
    lcd_puts(str);
    delay_ms(delay_time); 
    set_lcd(); 
}

void main(void)
{

DDRB = 0xFF;
PORTB = 0x00;

DDRE = 0xFF;
PORTE = 0xFF;

DDRA = 0xFF;
PORTA = 0xFF;

DDRD = 0x1C;
PORTD = 0xE3;


// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Low level
// INT1: On
// INT1 Mode: Low level
// INT2: Off
// INT3: Off
// INT4: Off
// INT5: Off
// INT6: Off
// INT7: Off

EICRA=(0<<ISC31) | (0<<ISC30) | (0<<ISC21) | (0<<ISC20) | (0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
EICRB=(0<<ISC71) | (0<<ISC70) | (0<<ISC61) | (0<<ISC60) | (0<<ISC51) | (0<<ISC50) | (0<<ISC41) | (0<<ISC40);
EIMSK=(0<<INT7) | (0<<INT6) | (0<<INT5) | (0<<INT4) | (0<<INT3) | (0<<INT2) | (1<<INT1) | (1<<INT0);
EIFR=(0<<INTF7) | (0<<INTF6) | (0<<INTF5) | (0<<INTF4) | (0<<INTF3) | (0<<INTF2) | (1<<INTF1) | (1<<INTF0);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTE Bit 0
// RD - PORTE Bit 1
// EN - PORTE Bit 2
// D4 - PORTE Bit 4
// D5 - PORTE Bit 5
// D6 - PORTE Bit 6
// D7 - PORTE Bit 7
// Characters/line: 20
lcd_init(20);

// Global enable interrupts
#asm("sei") 

set_lcd();

while (1)
      {
            green();
            yellow();
            red();
            yellow();

      }
}
