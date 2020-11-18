/*
 * DCMoter1110.c
 *
 * Created: 2020-11-10 오후 7:53:12
 * Author : 82104
 */ 
#define F_CPU 16000000UL
#define FORWARD 0x05
#define BACKWARD 0x0A
#define STOP 0x00

#define F 70 // F
#define B 66 // B
#define L 76 // L
#define R 82 // R
#define S 83 // S

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t state;
volatile uint8_t speed = 0;
volatile uint8_t data = 0;
volatile uint8_t cnt = 0;

uint8_t nums[4];
char str_state[] = "State :         ";
char str_speed[] = "Speed :         ";

ISR(USART0_RX_vect)
{
   data = UDR0;
   
   if(data == F) // FORWARD
   {
      state = FORWARD;
      Lcd_Goto_Xy(8, 0);
      Lcd_PutStr("FORWARD");
      PORTA = state;
   }
   else if(data == B) // BACKWARD
   {
      state = BACKWARD;
      Lcd_Goto_Xy(8, 0);
      Lcd_PutStr("BACKWARD");
      PORTA=state;
   
   }
   else if(data == R)//RIGHT
   {
      state= FORWARD;
      Lcd_Goto_Xy(8, 0);
      Lcd_PutStr("RIGHT   ");
      PORTA = state;   

   }
   else if(data == L) // LEFT
   {
      state = FORWARD;
      Lcd_Goto_Xy(8, 0);
      Lcd_PutStr("LEFT    ");
      PORTA = state;
      

   }
   else if(data == S) // STOP
   {
      state = STOP;
      Lcd_Goto_Xy(8, 0);
      Lcd_PutStr("STOP    ");
      PORTA =state;
      
   }
   else
   {
	   if((data >= 48 && data <= 57) && cnt < 3) // 만약 data가 숫자라면
	   {
		   Lcd_Goto_Xy(8, 1);
		   Lcd_PutStr("   ");
		   Lcd_Goto_Xy(8, 1);
		   Lcd_Data_Write(data);
		   _delay_ms(2000);
		   nums[cnt++] = data;
	   }
	   else if(data == 'e' || cnt == 3) // 만약 data가 e라면
	   {
		   nums[cnt] = '\0';
		   CalcSpeed();
	   }
   }
   
}

void CalcSpeed()
{
	if(cnt == 0)
	{
		return;
	}
	
	/*
	Lcd_Goto_Xy(8, 1);
	Lcd_PutStr("   ");
	Lcd_Goto_Xy(8, 1);
	Lcd_PutStr(nums);
	*/
	
	switch (cnt)
	{
		case 1:
			speed = (nums[cnt-1] - 48);
			break;
		case 2:
			speed = ((nums[cnt-2] - 48) * 10) + nums[cnt-1] - 48;
			break;
		case 3:
			speed = ((nums[cnt-3] - 48) * 100) + ((nums[cnt-2] - 48) * 10) + (nums[cnt-1] - 48);
			break;
	}

	cnt = 0;
}

void serial_init(void){
   UBRR0H = 0x00;
   UBRR0L = 0x67;
   
   UCSR0A=0x00;
   UCSR0B=0x98;
   UCSR0C=0x06;
   sei();
}

void Lcd_Cmd_Write(unsigned char cmd)
{
   PORTB = 0x00; // RS=0, R/W=0, E=0
   _delay_us(1);
   PORTB ^= 0x04; // RS=0, R/W=0, E=1
   PORTC = cmd;
   _delay_us(1);
   PORTB ^= 0x04; // RS = 0, R/W = 0, E = 0
   _delay_ms(5);
}

void Lcd_Data_Write(unsigned char data)
{
   PORTB = 0x01; // RS=1, R/W=0, E=0
   _delay_us(1);
   PORTB ^= 0x04; // RS=1, R/W=0, E=1
   PORTC = data;
   _delay_us(1);
   PORTB ^= 0x04; // RS = 0, R/W = 0, E = 0
   _delay_ms(5);
}

void Lcd_Goto_Xy(uint8_t x, uint8_t y)
{
   if(y==0) Lcd_Cmd_Write(0x80 + x); // DDRAM의 주소는 0x80 + 각 자리의 주소값
   else if(y==1) Lcd_Cmd_Write(0xC0 + x);
}

void Lcd_PutStr(char *str)
{
   while(*str != '\0') { Lcd_Data_Write(*(str++)); }
}

void Lcd_Init()
{
   _delay_ms(100);
   Lcd_Cmd_Write(0x38); // Function set, 00111000
   _delay_us(1);
   Lcd_Cmd_Write(0x0C); // 00001100
   _delay_us(1);
   Lcd_Cmd_Write(0x01); // 00000001
   _delay_ms(3);
   Lcd_Cmd_Write(0x06); // 00000110
}

 int main(void)
 {   
	DDRA = 0xFF;   // 모터 방향제어   포트A의 8비트 전체를 출력으로 설정
	DDRB = 0xFF;   // PWM2개, LCD명령어   포트B의 8비트 전체를 출력으로 설정
	DDRC = 0xFF;   // LCD Data포트
	DDRF = 0xFF;   // LED 포트
   
	TCCR0 = 0x6D;   //Timer0, fast PWM, presclaler 128설정
	TCCR2 = 0x6D;   //Timer2, fast PWM, presclaler 128설정
   
	Lcd_Init();
	Lcd_Goto_Xy(0,0);
	Lcd_PutStr(str_state);
	Lcd_Goto_Xy(0,1);
	Lcd_PutStr(str_speed);
	serial_init();
   
	PORTF = 0b00000101;
	while(1){
      OCR0=speed;   OCR2=speed-20;
	}
   return 0;
 }