#include "stm32l476xx.h"
#include <stdlib.h>
#define circle_delay 200
#define led_A 4
#define led_B 5
#define led_C 6
#define led_D 7
#define led_E 8
#define led_F 9
#define led_G 10
#define led_H 11
#define led_I 12
#define breathe_time 12
//遊戲相關設定
#define perfect 200
#define good 2000
#define total_btn 97
#define max_score 1000
//#define add_score_speed 5000

extern void max7219_init();
extern void max7219_send(unsigned char address, unsigned char data);
extern void Delay_1ms(); //1ms
extern void Delay_1s(); //1s

void Delay(int time);
void show_score(int score);
void init_GPIO();
void display(int data);
int button_press(int pin);
void light(int led);
void dark(int led);
void light_all();
void dark_all();
void breathe_light(int led);
void breathe_light_all();

void play_1(int led);
void play_2(int led_1, int led_2);
void play_3(int led_1, int led_2, int led_3);
void play_4(int led_1, int led_2, int led_3, int led_4);
void play_5(int led_1, int led_2, int led_3, int led_4, int led_5);

void breathe_light_play(int led);
void breathe_light_play_2(int led_1, int led_2);
void breathe_light_play_3(int led_1, int led_2, int led_3);
void breathe_light_play_4(int led_1, int led_2, int led_3, int led_4);
void breathe_light_play_5(int led_1, int led_2, int led_3, int led_4, int led_5);
void run(int led_1, int led_2, int led_3,int delay_time);
int breathe_light_start();
int breathe_light_end();

int score_all;
int score_time[11];
void start();
void one_game();
void end();
void init_UART();
void Timer_init();
void Timer_start(int sound_value);
//void add_score_anime(int ori_score,int add_score);

void run_8();
void running();


int main()
{
	init_GPIO();
	init_UART();
	max7219_init();
	Timer_init();
	Timer_start(good);
	score_all=0;
	display(score_all);
	while(1){
		start();



		one_game();
		end();
	}
	return 0;
}

/*
ABC
DEF
GHI
*/

void end(){
	int i=0;
	while(i<2){
		breathe_light_all();
		i++;
	}
	while(1)
	if(breathe_light_end()==1)
		break;

}

void running()
{
	int i=0;
	int delay_time = 1;
	while(i<2){
	run(led_C,led_B,led_A,delay_time);
	run(led_F,led_C,led_B,delay_time);
	run(led_E,led_F,led_C,delay_time);
	run(led_D,led_E,led_F,delay_time);
	run(led_G,led_D,led_E,delay_time);
	run(led_H,led_G,led_D,delay_time);
	run(led_I,led_H,led_G,delay_time);
	run(led_A,led_I,led_H,delay_time);
	run(led_B,led_A,led_I,delay_time);
	i++;
	}
}

void run_8()
{
	int i=0;
	int delay_time = 2;
	while(i<2){
	run(led_I,led_E,led_A,delay_time);
	run(led_F,led_I,led_E,delay_time);
	run(led_C,led_F,led_I,delay_time);
	run(led_E,led_C,led_F,delay_time);
	run(led_G,led_E,led_C,delay_time);
	run(led_D,led_G,led_E,delay_time);
	run(led_A,led_D,led_G,delay_time);
	run(led_E,led_A,led_D,delay_time);
	i++;
	}
}

void start(){
	int i=0;
	int delay_time = 1;
	running();
	//run_8();
	while(i<2){
	run(led_C,led_B,led_A,delay_time);
	run(led_F,led_C,led_B,delay_time);
	run(led_I,led_F,led_C,delay_time);
	run(led_H,led_I,led_F,delay_time);
	run(led_G,led_H,led_I,delay_time);
	run(led_D,led_G,led_H,delay_time);
	run(led_A,led_D,led_G,delay_time);
	run(led_B,led_A,led_D,delay_time);
	i++;
	}
	while(1)
	if(breathe_light_start()==1)
		break;

}
void init_UART() {
/*USART_init*/
	RCC->APB2ENR |=   RCC_APB2ENR_USART1EN;

	USART1->CR1 &= ~(USART_CR1_M|USART_CR1_PS|USART_CR1_PCE|USART_CR1_TE|USART_CR1_RE|USART_CR1_OVER8);
	USART1->CR1 |= (USART_CR1_TE|USART_CR1_RE);

	USART1->CR2 &= ~USART_CR2_STOP; /*!< 0x00003000 */

	USART1->CR3 &= ~(USART_CR3_RTSE|USART_CR3_CTSE|USART_CR3_ONEBIT);

	USART1->BRR &= ~(0xFFFF);
	USART1->BRR = (4000000/9600L);

	USART1->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
	USART1->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
	// Enable UART
	USART1->CR1 |= (USART_CR1_UE);

}

void Timer_init()
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	TIM2->ARR = (uint32_t)8;//Reload value
	TIM2->PSC = (uint32_t)40000;//Prescalser 1911 Do
	TIM2->EGR = TIM_EGR_UG;//Reinitialize the counter. CNT takes the auto-reload value.
	TIM2->CR1 |= TIM_CR1_CEN;//start timer
}

void Timer_start(int sound_value)
{
	TIM2->PSC = (uint32_t)sound_value;//Prescalser 1911 Do
	int i;
	for(i=0 ; i < 20000; i++)
	{
		int timerValue = TIM2->CNT;//polling the counter value
		if(timerValue < 4)
		{
			GPIOA->BSRR = (1<<1);
		}
		else
		{
			GPIOA->BRR = (1<<1);
		}
	}
}

void run(int led_1,int led_2,int led_3,int delay_time){

	long int count=0;
	int time;
	int breathe;

	int brightness_A=10;
	int brightness_B=4;
	int brightness_C=1;
	for(time = 0;time<delay_time ; time++)
	for(breathe = 0;breathe<10 ; breathe++)
	for(count=0;count< 10;count++)
	{
		if(count<brightness_A)
			light(led_1);
		else
			dark(led_1);

		if(count<brightness_B)
			light(led_2);
		else
			dark(led_2);

		if(count<brightness_C)
			light(led_3);
		else
			dark(led_3);

		Delay(1);
	}
	dark(led_1);
	dark(led_2);
	dark(led_3);

}



/*void add_score_anime(int ori_score,int add_score)
{
	while(add_score == 0)
	{
		if(add_score <= add_score_speed)
		{
			ori_score += add_score;
			add_score = 0;
		}
		else
		{
			ori_score += add_score_speed;
			add_score -= add_score_speed;
		}
	}
}*/




void one_game()
{
/**************
ABC
DEF
GHI
**************/
	int i;
	Delay(600); //修正時差
	Delay(1500);
	play_1(led_A);//Don
	Delay(1000);
	play_1(led_C);//Don
	Delay(1500);
	play_1(led_G);//Don
	Delay(1000);
	play_1(led_I);//Don
	Delay(1500);
	play_2(led_B,led_D);//為
	Delay(1000);
	play_2(led_F,led_H);//為
	Delay(1000);
	play_1(led_A);//為
	play_1(led_I);//何
	play_1(led_B);//為
	play_1(led_D);//何
	Delay(300);
	play_1(led_E);
	play_2(led_B,led_F);
	Delay(250);
	play_4(led_A,led_C,led_D,led_H); //放
	Delay(600);
	play_4(led_A,led_C,led_D,led_H); //放
	play_1(led_E);
	Delay(100);
	play_5(led_A,led_C,led_E,led_G,led_I); //X
	Delay(1000);
	play_2(led_D,led_H);
	Delay(1000);
	play_1(led_E);
	Delay(1000);
	play_1(led_A);
	Delay(1000);
	play_4(led_B,led_C,led_E,led_F);
	play_4(led_A,led_D,led_H,led_I);
	play_4(led_B,led_F,led_E,led_A);
	play_3(led_D,led_G,led_I);
	play_3(led_C,led_D,led_F);
	play_3(led_D,led_G,led_H);
	play_5(led_B,led_D,led_E,led_F,led_H); //+
	play_4(led_A,led_C,led_G,led_I); //口
	play_4(led_B,led_D,led_E,led_F);
	play_4(led_A,led_G,led_H,led_I);
	play_4(led_B,led_D,led_F,led_C);
	play_1(led_G);
	play_2(led_B,led_D);
	play_2(led_F,led_H);
	play_5(led_A,led_C,led_E,led_G,led_F);
	play_4(led_B,led_D,led_F,led_H); //菱形
	play_1(led_C);
	play_4(led_A,led_B,led_E,led_F);
	play_3(led_D,led_G,led_H);

	for(i=0;i<5;i++)
		breathe_light_all();

}

//Delay
void Delay(int time)
{
	for(int i=0;i<time;i++)
		Delay_1ms();
}

//GPIO初始化
void init_GPIO()
{
	RCC->AHB2ENR = RCC->AHB2ENR|0xF; //PA,PB,PC
	//Set PA 5~12 as output mode
	GPIOA->MODER = 0xA96955F7;

	GPIOA->OTYPER &= ~(0xFFFF); //otype PP
	//GPIOA->OTYPER |= 0x0600; //otype PP

	GPIOA->PUPDR &= ~(0xFFFFFFFF); //PUpd nopull
	//GPIOA->PUPDR = 0xEBFFF7; //PUpd nopull

	GPIOA->OSPEEDR &= ~(0xFFFFFFFF); //speed low
	//GPIOA->OSPEEDR = 0xEBFFF7; //speed low

	GPIOA->AFR[1] = (GPIOA->AFR[1] & ~(0x00000FF0)) | 0x00000770; // AF7 for pin

	//Set PB 0-8 as input mode
	GPIOB->MODER = 0xFF140000;

	//set PB 0-15 is Pull-up output
	GPIOB->PUPDR = GPIOB->PUPDR | 0x55555555;
	//Set PC 0~2 as output mode
	GPIOC->MODER = GPIOC->MODER & 0xFFFFFFD5;
}


//顯示分數
void display(int data)
{
	if(data == 0)
	{
		max7219_send(1,0);
		for(int i=2 ; i<=8 ; i++) //從第一個數字開始印
			max7219_send(i,15);
	}
	else
	{
		for(int i=1 ; i<=8 ; i++) //從第一個數字開始印
		{
			if(data==0)
				max7219_send(i,15);
			else
				max7219_send(i,data%10);
			data = data/10;
		}
	}
}

int button_press(int pin){
	int debounce_done = 0;
	if(!(GPIOB->IDR&1<<(pin-4))) {
		debounce_done=1;
	}
	return debounce_done;
}


void light(int led){
	if(led==9 || led==10)
			GPIOB->BSRR |= (1<<led);
	else
		GPIOA->BSRR |= (1<<led);
}

void light_all(){
	GPIOA->BSRR |= (1<<led_A);
	GPIOA->BSRR |= (1<<led_B);
	GPIOA->BSRR |= (1<<led_C);
	GPIOA->BSRR |= (1<<led_D);
	GPIOA->BSRR |= (1<<led_E);
	GPIOB->BSRR |= (1<<led_F);
	GPIOB->BSRR |= (1<<led_G);
	GPIOA->BSRR |= (1<<led_H);
	GPIOA->BSRR |= (1<<led_I);
}

void dark(int led){
	if(led==9 || led==10)
		GPIOB->BRR |= (1<<led);
	else
		GPIOA->BRR |= (1<<led);
}

void dark_all(){
	GPIOA->BRR |= (1<<led_A);
	GPIOA->BRR |= (1<<led_B);
	GPIOA->BRR |= (1<<led_C);
	GPIOA->BRR |= (1<<led_D);
	GPIOA->BRR |= (1<<led_E);
	GPIOB->BRR |= (1<<led_F);
	GPIOB->BRR |= (1<<led_G);
	GPIOA->BRR |= (1<<led_H);
	GPIOA->BRR |= (1<<led_I);
}

void uu(){

		int a[]={98,98,98,98,98,13,10};
		int i;
		for(i=0;i<7;i++)
		{
			while (!(USART1->ISR & USART_ISR_TXE) &&(USART1->ISR & USART_ISR_TC));
			USART1->TDR = a[i]; //H
			Delay_1s();
		}
		for(i=0;i<100;i++)
			Delay_1s();
}

int breathe_light_start(){
	long int count=0;
	int breathe;
	int brightness;
	int play=0;
	for(brightness = 0; brightness<12 ; brightness++)
		for(breathe = 0;breathe<5 ; breathe++)
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play ==0)
					light(led_E);
				else
					dark(led_E);

				if(button_press(led_E)&& play==0){
					Timer_start(perfect);
					uu();
					play=1;
					dark(led_E);
					return 1;
				}
				Delay(1);
			}
	for(brightness = 12; brightness>0 ; brightness--)
		for(breathe = 0;breathe<5 ; breathe++)//0.1
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play ==0)
					light(led_E);
				else
					dark(led_E);

				if(button_press(led_E)&&play==0 ){
					uu();
					play=1;
					Timer_start(perfect);
					dark(led_E);
					return 1;
				}
				Delay(1);
			}
	return play;
}
int breathe_light_end(){
	long int count=0;
	int breathe;
	int brightness;
	int play=0;
	for(brightness = 0; brightness<12 ; brightness++)
		for(breathe = 0;breathe<5 ; breathe++)
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play ==0)
					light(led_E);
				else
					dark(led_E);

				if(button_press(led_E)&& play==0){
					play=1;
				}
				Delay(1);
			}
	for(brightness = 12; brightness>0 ; brightness--)
		for(breathe = 0;breathe<5 ; breathe++)//0.1
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play ==0)
					light(led_E);
				else
					dark(led_E);

				if(button_press(led_E)&&play==0 ){
					play=1;
				}
				Delay(1);
			}



	return play;
}



void breathe_light(int led){
	long int count=0;
	int breathe;
	int brightness;
	for(brightness = 0; brightness<10 ; brightness++)
	for(breathe = 0;breathe<8 ; breathe++)//0.1
	for(count=0;count< 10;count++)//0.01
	{
		if(count<brightness)
			light(led);
		else
			dark(led);
		Delay(1);
	}
	for(brightness = 10; brightness>0 ; brightness--)
	for(breathe = 0;breathe<8 ; breathe++)//0.1
	for(count=0;count< 10;count++)//0.01
	{
		if(count<brightness)
			light(led);
		else
			dark(led);
		Delay(1);
	}
}

void breathe_light_all(){
	long int count=0;
	int breathe;
	int brightness;
	for(brightness = 2; brightness<10 ; brightness++)
	for(breathe = 0;breathe<5 ; breathe++)//0.1
	for(count=0;count< 10;count++)//0.01
	{
		if(count<brightness)
			light_all();
		else
			dark_all();
		Delay(1);
	}
	for(brightness = 10; brightness>0 ; brightness--)
	for(breathe = 0;breathe<5 ; breathe++)//0.1
	for(count=0;count< 10;count++)//0.01
	{
		if(count<brightness)
			light_all();
		else
			dark_all();
		Delay(1);
	}
}

void show_score(int score){
	int add_score=0;

	if(score>12){
		Timer_start(perfect);
		add_score = max_score*100/100/total_btn;
	}else if(score>8){
		Timer_start(good);
		add_score= max_score*70/100/total_btn;
	}else if(score>5){
		add_score= max_score*40/100/total_btn;
	}

	//add_score_anime(score_all,add_score);
	score_all += add_score;
	display(score_all);
}

void play_1(int led){
	breathe_light_play(led);
}

void play_2(int led_1, int led_2){
	breathe_light_play_2(led_1, led_2);
}

void play_3(int led_1, int led_2, int led_3){
	breathe_light_play_3(led_1, led_2, led_3);
}

void play_4(int led_1, int led_2, int led_3, int led_4){
	breathe_light_play_4(led_1, led_2, led_3, led_4);
}

void play_5(int led_1, int led_2, int led_3, int led_4, int led_5){
	breathe_light_play_5(led_1, led_2, led_3, led_4, led_5);
}

void breathe_light_play(int led){
	long int count=0;
	int breathe;
	int brightness;
	int play=0;
	int score=0;
	for(brightness = 0; brightness<12 ; brightness++)
		for(breathe = 0;breathe<5 ; breathe++)
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play ==0)
					light(led);
				else
					dark(led);

				if(button_press(led)&& play==0){
					play=1;
					score = brightness;
					show_score(score);
					score_time[brightness]++;
				}
				Delay(1);
			}
	for(brightness = 12; brightness>0 ; brightness--)
		for(breathe = 0;breathe<5 ; breathe++)//0.1
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play ==0)
					light(led);
				else
					dark(led);

				if(button_press(led)&&play==0 ){
					play=1;
					score = brightness;
					show_score(score);
					score_time[brightness]++;
				}
				Delay(1);
			}
}


void breathe_light_play_2(int led_1, int led_2){
	long int count=0;
	int breathe;
	int brightness;
	int play_1=0;
	int play_2=0;
	int score_1=0;
	int score_2=0;
	for(brightness = 0; brightness<14 ; brightness++)
		for(breathe = 0;breathe<5 ; breathe++)
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play_1 ==0)
					light(led_1);
				else
					dark(led_1);

				if(button_press(led_1)&& play_1==0){
					play_1=1;
					score_1 = brightness;
					show_score(score_1);
					score_time[brightness]++;
				}

				if(count<brightness && play_2 ==0)
					light(led_2);
				else
					dark(led_2);

				if(button_press(led_2)&& play_2==0){
					play_2=1;
					score_2 = brightness;
					show_score(score_2);
					score_time[brightness]++;
				}
				Delay(1);
			}

	for(brightness = 14; brightness>0 ; brightness--)
		for(breathe = 0;breathe<5 ; breathe++)
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play_1 ==0)
					light(led_1);
				else
					dark(led_1);

				if(button_press(led_1)&& play_1==0){
					play_1=1;
					score_1 = brightness;
					show_score(score_1);
					score_time[brightness]++;
				}
				if(count<brightness && play_2 ==0)
					light(led_2);
				else
					dark(led_2);
				if(button_press(led_2)&& play_2==0){
					play_2=1;
					score_2 = brightness;
					show_score(score_2);
					score_time[brightness]++;

				}
				Delay(1);
			}

}

void breathe_light_play_3(int led_1, int led_2, int led_3){
	long int count=0;
	int breathe;
	int brightness;
	int play_1=0;
	int play_2=0;
	int play_3=0;
	int score_1=0;
	int score_2=0;
	int score_3=0;
	for(brightness = 0; brightness<14 ; brightness++)
		for(breathe = 0;breathe<5 ; breathe++)
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play_1 ==0)
					light(led_1);
				else
					dark(led_1);

				if(button_press(led_1)&& play_1==0){
					play_1=1;
					score_1 = brightness;
					show_score(score_1);
					score_time[brightness]++;
				}

				if(count<brightness && play_2 ==0)
					light(led_2);
				else
					dark(led_2);

				if(button_press(led_2)&& play_2==0){
					play_2=1;
					score_2 = brightness;
					show_score(score_2);
					score_time[brightness]++;
				}

				if(count<brightness && play_3 ==0)
					light(led_3);
				else
					dark(led_3);

				if(button_press(led_3)&& play_3==0){
					play_3=1;
					score_3 = brightness;
					show_score(score_3);
					score_time[brightness]++;
				}
				Delay(1);
			}

	for(brightness = 14; brightness>0 ; brightness--)
		for(breathe = 0;breathe<5 ; breathe++)
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play_1 ==0)
					light(led_1);
				else
					dark(led_1);

				if(button_press(led_1)&& play_1==0){
					play_1=1;
					score_1 = brightness;
					show_score(score_1);
					score_time[brightness]++;
				}
				if(count<brightness && play_2 ==0)
					light(led_2);
				else
					dark(led_2);
				if(button_press(led_2)&& play_2==0){
					play_2=1;
					score_2 = brightness;
					show_score(score_2);
					score_time[brightness]++;

				}
				if(count<brightness && play_3 ==0)
					light(led_3);
				else
					dark(led_3);

				if(button_press(led_3)&& play_3==0){
					play_3=1;
					score_3 = brightness;
					show_score(score_3);
					score_time[brightness]++;
				}
				Delay(1);
			}

}


void breathe_light_play_4(int led_1, int led_2, int led_3, int led_4){
	long int count=0;
	int breathe;
	int brightness;
	int play_1=0;
	int play_2=0;
	int play_3=0;
	int play_4=0;


	int score_1=0;
	int score_2=0;
	int score_3=0;
	int score_4=0;


	for(brightness = 0; brightness<14 ; brightness++)
		for(breathe = 0;breathe<5 ; breathe++)
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play_1 ==0)
					light(led_1);
				else
					dark(led_1);

				if(button_press(led_1)&& play_1==0){
					play_1=1;
					score_1 = brightness;
					show_score(score_1);
					score_time[brightness]++;
				}

				if(count<brightness && play_2 ==0)
					light(led_2);
				else
					dark(led_2);

				if(button_press(led_2)&& play_2==0){
					play_2=1;
					score_2 = brightness;
					show_score(score_2);
					score_time[brightness]++;
				}

				if(count<brightness && play_3 ==0)
					light(led_3);
				else
					dark(led_3);

				if(button_press(led_3)&& play_3==0){
					play_3=1;
					score_3 = brightness;
					show_score(score_3);
					score_time[brightness]++;
				}

				if(count<brightness && play_4 ==0)
					light(led_4);
				else
					dark(led_4);

				if(button_press(led_4)&& play_4==0){
					play_4=1;
					score_4 = brightness;
					show_score(score_4);
					score_time[brightness]++;
				}
				Delay(1);
			}

	for(brightness = 14; brightness>0 ; brightness--)
		for(breathe = 0;breathe<5 ; breathe++)
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play_1 ==0)
					light(led_1);
				else
					dark(led_1);

				if(button_press(led_1)&& play_1==0){
					play_1=1;
					score_1 = brightness;
					show_score(score_1);
					score_time[brightness]++;
				}
				if(count<brightness && play_2 ==0)
					light(led_2);
				else
					dark(led_2);
				if(button_press(led_2)&& play_2==0){
					play_2=1;
					score_2 = brightness;
					show_score(score_2);
					score_time[brightness]++;

				}
				if(count<brightness && play_3 ==0)
					light(led_3);
				else
					dark(led_3);

				if(button_press(led_3)&& play_3==0){
					play_3=1;
					score_3 = brightness;
					show_score(score_3);
					score_time[brightness]++;
				}

				if(count<brightness && play_4 ==0)
					light(led_4);
				else
					dark(led_4);

				if(button_press(led_4)&& play_4==0){
					play_4=1;
					score_4 = brightness;
					show_score(score_4);
					score_time[brightness]++;
				}
				Delay(1);
			}

}

void breathe_light_play_5(int led_1, int led_2, int led_3, int led_4, int led_5){
	long int count=0;
	int breathe;
	int brightness;
	int play_1=0;
	int play_2=0;
	int play_3=0;
	int play_4=0;
	int play_5=0;


	int score_1=0;
	int score_2=0;
	int score_3=0;
	int score_4=0;
	int score_5=0;


	for(brightness = 0; brightness<14 ; brightness++)
		for(breathe = 0;breathe<5 ; breathe++)
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play_1 ==0)
					light(led_1);
				else
					dark(led_1);

				if(button_press(led_1)&& play_1==0){
					play_1=1;
					score_1 = brightness;
					show_score(score_1);
					score_time[brightness]++;
				}

				if(count<brightness && play_2 ==0)
					light(led_2);
				else
					dark(led_2);

				if(button_press(led_2)&& play_2==0){
					play_2=1;
					score_2 = brightness;
					show_score(score_2);
					score_time[brightness]++;
				}

				if(count<brightness && play_3 ==0)
					light(led_3);
				else
					dark(led_3);

				if(button_press(led_3)&& play_3==0){
					play_3=1;
					score_3 = brightness;
					show_score(score_3);
					score_time[brightness]++;
				}

				if(count<brightness && play_4 ==0)
					light(led_4);
				else
					dark(led_4);

				if(button_press(led_4)&& play_4==0){
					play_4=1;
					score_4 = brightness;
					show_score(score_4);
					score_time[brightness]++;
				}

				if(count<brightness && play_5 ==0)
					light(led_5);
				else
					dark(led_5);

				if(button_press(led_5)&& play_5==0){
					play_5=1;
					score_5 = brightness;
					show_score(score_5);
					score_time[brightness]++;
				}
				Delay(1);
			}

	for(brightness = 14; brightness>0 ; brightness--)
		for(breathe = 0;breathe<5 ; breathe++)
			for(count=0;count< 10;count++)//0.01
			{
				if(count<brightness && play_1 ==0)
					light(led_1);
				else
					dark(led_1);

				if(button_press(led_1)&& play_1==0){
					play_1=1;
					score_1 = brightness;
					show_score(score_1);
					score_time[brightness]++;
				}
				if(count<brightness && play_2 ==0)
					light(led_2);
				else
					dark(led_2);
				if(button_press(led_2)&& play_2==0){
					play_2=1;
					score_2 = brightness;
					show_score(score_2);
					score_time[brightness]++;

				}
				if(count<brightness && play_3 ==0)
					light(led_3);
				else
					dark(led_3);

				if(button_press(led_3)&& play_3==0){
					play_3=1;
					score_3 = brightness;
					show_score(score_3);
					score_time[brightness]++;
				}

				if(count<brightness && play_4 ==0)
					light(led_4);
				else
					dark(led_4);

				if(button_press(led_4)&& play_4==0){
					play_4=1;
					score_4 = brightness;
					show_score(score_4);
					score_time[brightness]++;
				}

				if(count<brightness && play_5 ==0)
					light(led_5);
				else
					dark(led_5);

				if(button_press(led_5)&& play_5==0){
					play_5=1;
					score_5 = brightness;
					show_score(score_5);
					score_time[brightness]++;
				}
				Delay(1);
			}

}









