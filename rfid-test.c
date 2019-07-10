#include<pic.h>
#include<htc.h>
#define _XTAL_FREQ 4000000

void delay();
void delay1();
void lcd_data(unsigned char);
void lcd_com(unsigned char);
void comn_data(unsigned char,unsigned char*);
void lcd_init();
void uart_init();

void InitPWM();
void SetPWMDutyCycle(unsigned int DutyCycle);


void UART_Puts(unsigned char *);
void UART_Putc(unsigned char);
int ch;

#define lcd PORTB
#define rs RD6
#define en RD7

//#define relay1 RC0
//#define relay2 RC1

int flag,flag1,k,r,x,c2,y;
//int flag,flag1,k,r,x,a,b,c,c1,c2,y;
int s[12];

void main()
{
flag1=0;
PORTB=0X00;
TRISB=0X00;
PORTD=0X00;
TRISD=0X00;
PORTC=0X00;
TRISC=0X80;
PORTE=0X00;
TRISE=0X00;
ADCON1=0X0F;

lcd_init();
comn_data(0x80,"  RFID BASED  ");
comn_data(0xC0," SPEED CONTROL ");
delay1();
delay1();
delay1();
lcd_com(0x01);
comn_data(0x80,"SHOW RFID CARD");
uart_init();
InitPWM();

while(1)
{
if(flag==1)
{
lcd_init();
if(s[0]=='0' && s[1]=='9' && s[2]=='0' && s[3]=='0' && s[4]=='1' && s[5]=='5' && s[6]=='0' && s[7]=='F'&& s[8]=='0'&& s[9]=='9' && s[10]=='1' && s[11]=='A')
{
//lcd_com(0x01);

comn_data(0x80,"ENG. SPEED @ 30%");
//comn_data(0xC0,"ACCEPT          ");

__delay_ms(10);
UART_Puts("\r \n ");
UART_Puts("30");
__delay_ms(10);

SetPWMDutyCycle(300);   //20% duty cycle

k=0;
flag=0;
RCIF=0;
delay1();
}

else if(s[0]=='0' && s[1]=='9' && s[2]=='0' && s[3]=='0' && s[4]=='1' && s[5]=='5' && s[6]=='0' && s[7]=='F'&& s[8]=='5'&& s[9]=='1' && s[10]=='4' && s[11]=='2')
{
comn_data(0x80,"ENG. SPEED @ 70%");
//comn_data(0xC0,"ACCEPT          ");

__delay_ms(10);
UART_Puts("\r \n ");
UART_Puts("70");
__delay_ms(10);

SetPWMDutyCycle(600);   //20% duty cycle


k=0;
flag=0;
RCIF=0;
delay1();
}

else if(s[0]=='0' && s[1]=='9' && s[2]=='0' && s[3]=='0' && s[4]=='1' && s[5]=='5' && s[6]=='0' && s[7]=='E'&& s[8]=='C'&& s[9]=='8' && s[10]=='D' && s[11]=='A')
{
comn_data(0x80,"ENG. SPEED @ 92%");
//comn_data(0xC0,"ACCEPT          ");

__delay_ms(10);
UART_Puts("\r \n ");
UART_Puts("92");
__delay_ms(10);

SetPWMDutyCycle(999);   //20% duty cycle

k=0;
flag=0;
RCIF=0;
delay1();
}

//for(y=0;y<=8;y++)
//{
//s[y]=0;
//}

}
}
}

void InitPWM()
{
	TRISC2  = 0;            // Make CCP1 pin as output
	CCP1CON = 0x0C;         // Configure CCP1 module in PWM mode

	PR2   = 0xFF;           // Configure the Timer2 period
	T2CON = 0x01;           // Set Prescaler to be 4, hence PWM frequency is set to 4.88KHz.

	SetPWMDutyCycle(0);     // Intialize the PWM to 0 duty cycle

	T2CON |= 0x04;          // Enable the Timer2, hence enable the PWM.
}

void SetPWMDutyCycle(unsigned int DutyCycle) // Give a value in between 0 and 1024 for DutyCycle
{
	CCPR1L   = DutyCycle>>2;        	// Put MSB 8 bits in CCPR1L
	CCP1CON &= 0xCF;                	// Make bit4 and 5 zero
	CCP1CON |= (0x30&(DutyCycle<<4));   // Assign Last 2 LSBs to CCP1CON
}

void UART_Puts(unsigned char *str)             
{
    while(*str)
        UART_Putc(*str++);
}

void UART_Putc(unsigned char Ch)
{
    TXREG = Ch;
    while(TXIF == 0);                       
}

void lcd_init()
{
lcd_com(0x38);
lcd_com(0x0c);
lcd_com(0x06);
lcd_com(0x80);
lcd_com(0x01);
}

void uart_init()
{
TXSTA=0X24;
RCSTA=0X90;
TXREG=0X00;
RCREG=0X00;
SPBRG=0X19;
INTCON=0XC0;
PIE1=0X20;
}

void interrupt ISR(void)
{
if(RCIF==1)
{
s[k]=RCREG;
k++;
if(k>8)
{
flag=1;
//flag1=1;
}
}
}

void lcd_com(unsigned char com)
{
lcd=com;
rs=0;
en=1;
delay();
en=0;
delay();
}
void lcd_data(unsigned char dat)
{
lcd=dat;
rs=1;
en=1;
delay();
en=0;
delay();
}
void comn_data(unsigned char com,unsigned char *dat)
{
lcd_com(com);
while(*dat)
{
lcd_data(*dat++);
}
}
void delay()
{
unsigned char i;
for(i=0;i<255;i++);
}
void delay1()
{
unsigned int i,j;
for(i=0;i<65535;i++);
}

