/*
 * UART_ECHO.c
 *
 * Created: 2020-06-17 오후 4:07:28
 *  Author: 김태관
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

unsigned char Menu[]="\r\n\t ++++ Menu ++++ \n L : LED \n C : CDS \n 0 ~ 7 : PIANO \n S : STEP MOTOR \n push button : stop \n\n";
unsigned int DoReMi[9]={5230,5870,6590,6980,7830,8800,9870,10460,11050};
int Stop=0;
	
void putch(unsigned char data);
unsigned char getch();
void Menu_print();
void LED_left_right();
void Step_Motor();
void Cds();
void Piano(unsigned char num);

int main()
{
	//---------------------------------------------------PORT 입출력 설정

	DDRA=0xFF;	 //LED
	
	DDRB=0xF0;   //Step Motor
	
	DDRF=0x02;   //Cds
	PORTF=0x02;  //Cds
	
	DDRE = 0x08; //Buzzer
	
	DDRD = 0x00; //Interrupt
	
	//-----------------------------------------------------------------
	
	//--------------------------------------------------------통신 설정

	UCSR0A=0x00;
	UCSR0B=0x18;
	UCSR0C=0x06;
	
	UBRR0H=0x00;
	UBRR0L=0x03;

	//-----------------------------------------------------------------
	
	//---------------------------------------------------------ADC 설정
	
	ADMUX = 0x00; 
	ADCSRA = 0x86;
	
	//-----------------------------------------------------------------
	
	//---------------------------------------------------------Fast PWM
	
	TCCR3A = 0x00;
	TCCR3B = 0x19;
	TCCR3C = 0x00;
	TCNT3 =0x00;
	
	//-----------------------------------------------------------------
	
	//----------------------------------------------------인터럽트 설정
	EIMSK = 0x01;
	EICRA = 0x03;
	EIFR = 0x01;
	sei();
	
	unsigned char Select;
	Menu_print(); // 터미널 메뉴 출력
	
	while(1)
	{
		Select=getch();
		if(Select =='l' || Select =='L') 
			LED_left_right();
			
		else if(Select == 'c' || Select == 'C') 
			Cds();
		
		else if(Select == '0' || Select == '1' || Select == '2' || Select == '3' || Select == '4' || Select == '5' || Select == '6' || Select == '7' || Select == '8')
			Piano(Select);
			
		else if(Select == 's' || Select == 'S')
			Step_Motor();
		else continue;	
	}
	return 0;
}

SIGNAL(INT0_vect)
{
	cli();
	if(Stop==0) Stop=1;
	sei();
}

void putch(unsigned char data)// 송신 함수
{
	while((UCSR0A & 0x20)==0);
	UDR0=data;
	UCSR0A |=0x20;
}

unsigned char getch() // 수신 함수
{
	unsigned char data;
	while((UCSR0A & 0x80)==0);
	data=UDR0;
	UCSR0A |= 0x80;
	return data;
}

void Menu_print() // 메뉴 출력 함수
{
	unsigned char i = 0;
	while(Menu[i]!='\0')
	{
		putch(Menu[i++]);
	}
}

void LED_left_right()//LED 좌우 이동.
{
	unsigned char Led_Num;
	unsigned char str[]="***********LED Play************\n\n";
	unsigned char str2[]="***********LED Off************\n\n";
	unsigned char i = 0;
	while(str[i]!='\0')
	{
		putch(str[i++]);
	}
	i=0;
	
	while(1)
	{
		if(Stop==1) //인터럽트 종료
		{
			Stop=0;
			while(str2[i]!='\0')
			{
				putch(str2[i++]);
			}
			i=0;
			Menu_print();
			break;
		}
		
		Led_Num=0x01;
		for(int i=0;i<7;i++)
		{
			if(Stop==1) //인터럽트 종료
			{
				PORTA=0x00;
				break;
			}
			
			PORTA=Led_Num;
			Led_Num<<=1;
			_delay_ms(500);
		}
		for(int i=0;i<8;i++)
		{
			if(Stop==1)
			{
				PORTA=0x00;
				break;
			}
			PORTA=Led_Num;
			Led_Num>>=1;
			_delay_ms(500);
		}
		
	}

}

void Step_Motor()//Step Motor
{
	unsigned char Step_Motor_rf[]="Step Motor Run(CW : f, CCW : r, Quit : x)\n\n";
	unsigned char str[]="************** Step Mortor Off **************\n\n";
	
	unsigned char Direction;
	int M=0;
	
	// 스텝 모터 함수 진입 알림.
	unsigned char i = 0;
	while(Step_Motor_rf[i]!='\0')
	{
		putch(Step_Motor_rf[i++]);
	}
	i=0;
	//-------------------------
	
	while(1)
	{
		if(Stop == 1) //인터럽트 종료
		{
			Stop=0;
			PORTB=0x00;
			
			while(str[i]!='\0')
			{
				putch(str[i++]);
			}
			i=0;
			Menu_print();
			break;
		}
		Direction=getch();
		if(Direction=='f') M=0; //정방향
		else if(Direction=='r') M=1; //역방향
		else if(Direction=='x') M=2;
		else continue;
		
		if(M==0) //정방향
		{
			for(int i=0;i<12;i++)
			{
				if(Stop==1) break; //인터럽트 종료
				PORTB=0x30;
				_delay_ms(10);
				PORTB=0x90;
				_delay_ms(10);
				PORTB=0xC0;
				_delay_ms(10);
				PORTB=0x60;
				_delay_ms(10);
			}
		}
		else if(M==1) //역방향
		{
			
			for(int i=0;i<12;i++)
			{
				if(Stop==1) break; //인터럽트 종료
				PORTB=0x60;
				_delay_ms(10);
				PORTB=0xC0;
				_delay_ms(10);
				PORTB=0x90;
				_delay_ms(10);
				PORTB=0x30;
				_delay_ms(10);
			}
		}
		else if(M==2)
		{
			
			while(str[i]!='\0')
			{
				putch(str[i++]);
			}
			i=0;
			
			Menu_print();
			break;
		}
		
		
	}
	
	
	
}

void Cds() //Cds 센서 ADC변환 함수
{
	unsigned char AdData=0;
	unsigned int i = 0;
	char Cds[255];
	
	unsigned char str[]="***********Cds Play************\n\n";
	unsigned char str2[]="***********Cds Play************\n\n";
	unsigned char j = 0;
	while(str[j]!='\0')
	{
		putch(str[j++]);
	}
	
	while(1)
	{
		if(Stop==1)
		{
			Stop=0;
			while(str2[i]!='\0')
			{
				putch(str2[i++]);
			}
			i=0;
			Menu_print();
			break;
		}
		ADCSRA |=0x40;
		while((ADCSRA & 0x10)==0x00);
		
		AdData = ADC;
		sprintf(Cds,"Cds : %d\n",AdData);
		
		while(Cds[i]!='\0')
		{
			putch(Cds[i++]);
		}
		i=0;
		_delay_ms(500);
	}

}

void Piano(unsigned char num)// 음계발생 함수
{
	unsigned char eum=0;
	
	if(num=='0')	  eum=0;
	else if(num=='1') eum=1; //도
	else if(num=='2') eum=2; //레
	else if(num=='3') eum=3; //미
	else if(num=='4') eum=4; //파
	else if(num=='5') eum=5; //솔
	else if(num=='6') eum=6; //라
	else if(num=='7') eum=7; //시
	else if(num=='8') eum=8; //도
	
	if(num!='\0')
	{
		ICR3 = 7372800/DoReMi[eum]/2;
		TCCR3A = 0x40;
		_delay_ms(500);
		TCCR3A=0x00;
	}
}