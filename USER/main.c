#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"	 
#include "stdio.h"
#include "stdlib.h"
#include "rtc.h"
#include "string.h"
#include "math.h"
#define FAILURE 0
#define SUCCESS 1
#define  SUPPORT_PPM  1
//#define  SUPPORT_88165_TEMP  1

#define  HEAT_ON   GPIO_SetBits(GPIOA,GPIO_Pin_3)
#define  HEAT_OFF  GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define  SET_TEMP  400

extern unsigned char Buf1[16];
extern unsigned char Buf2[16];
extern unsigned char Buf3[16];
extern unsigned char Buf4[16];
extern unsigned char Buf5[16];
u8 Bufa[16]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
u8 Bufn[16]={0};
extern u8 wordptr;
//u16  printcnt;
//u16  loopcnt,keycnt,calcnt;	
u8   testbyte,updatebyte,updatesecond,iscal,isProg;
u8   key1count,key2count,Islowvolt,adcstate,disptype,keyrepcnt,ismenu;
//extern  u16   LEDwritecnt;
u16  U1timeout,U2timeout; 
u8   U1rxbuf[256]; //,U2rxbuf[265];
u8   U1rxptr;      //,U2rxptr;

u8   U2rxbuf[32],U2rptr;
u8   U3rxbuf[32],U3rptr;

u16   TestCnt=0,keycnt1,ledcnt,keycnt2,Slvcnt,testSlv1;
u16   B10msTimer,B1secTimer,B1msTimer,Isflashcnt,B100msTimer,B1sTimer,BlinkTimer;
u32   SystemTimer,system1sec;
u32   AdcVolt,AdcCurrent1,AdcCurrent2,AdcCurrent3,AdcCurrent4;
u32   DispVolt,DispC1,DispC2,DispC3,DispC4;
u32   AdcVcnt,AdcC1,AdcC2,AdcC3,AdcC4;

u32   adcavg1,adcavg2,SystemStatus,checksum,BlockClearCnt,RelayCnt,BlockClearCnt1;
//u16   AdcBuffer[8],Adcptr;

u32   HeatDutyCnt; 
u16   TempNow,heatcnt; 
u8 test=0;
u8 test1=0;
u8 key1short,key2short,key3short,key4short;
u8 key1long,key2long,key3long,key4long;

u16 key1cnt,key2cnt,key3cnt,key4cnt=0;
u8 finish_sentence=0;
u8 state;
u8 STATE=0;
u8 delay,runbarenable=1;
u8 menu,option,result;
u8 count10=10;
u8 keycnt=0;
u8 blink=0;
u8 Control_;
int cnt_m;
int cnt_h;
int cnt_d;
int cnt_w;
u8 m=16,h=16,d=16,w=16;
short Targetup;
short Targetdown;
signed short centline;
u16 calcnt=25;
u8 buzzing=0;
u8 buzcycle=0;
u8 add_cnt=0;
u8 minus_cnt=0;
int Bufco2[2]={0};
u16 light;
u8 cutoff=0;
int flashptr_D=0;
int flashptr_W=0;
//////////////////////////////////////////
int co2record_m[16],co2record_h[16],co2record_d[16],co2record_w[16],maxco2_m,maxco2_h,maxco2_d,maxco2_w,minco2_m,minco2_h,minco2_d,minco2_w;
signed char co2bar_m[16],co2bar_h[16],co2bar_d[16],co2bar_w[16];
u8 caled;
u8 cont_co2=1,cont_temp=1,cont_rh=1;
u16 co2=761,temp=223,rh=77;
u16 cent_co2=1200,cent_co2_;
u16 zone_co2=400,zone_co2_;
u8 Mode=0,Mode_;
u8 buz=0,buz_;
u8 blevel=4,blevel_;
u8 unit=0,unit_;
u8 timespc=0;
u8 display=1;
u8 relay=0;
u16 calpoint=400;
short offset=0;
float altitude=0,altitude_;
float press;
//////////////////////////////////////////
char sentence_A[]="AZ7531WarmUp        ";
char sentence_B[]="WarmUp        ";
char sentence_C[]="Set CO2 center value        ";
char sentence_D[]="Set CO2 zone value        ";
char sentence_E[]="Press UP to change, ENTER to confirm, MENU to quit.        ";
char sentence_F[]="Hold ENTER to calibration, press MENU to quit.        ";
char sentence_G[]="Done: Center Set.         ";
char sentence_H[]="Done: zone Set.         ";
char sentence_I[]="Buzzer beep setting.        ";
char sentence_J[]="Done: Control Set.        ";
char sentence_K[]="Plant mode        ";
char sentence_L[]="Human mode        ";
char sentence_M[]="on        ";
char sentence_N[]="off        ";
char sentence_O[]="Done: alarm is off.        ";
char sentence_P[]="Done: alarm is on.        ";
char sentence_Q[]="Brightness setting.        ";
char sentence_R[]="Done: Human mode.        ";
char sentence_S[]="Done: Plant mode.        ";
char sentence_T[]="Done: factory setting restored        ";
char sentence_U[]="Restore factory setting        ";
char sentence_V[]="hold ENTER to confirm, press MENU to quit.        ";
char sentence_W[]="Quit menu        ";
char sentence_X[]="Center        ";
char sentence_Y[]="Zone        ";
char sentence_Z[]="CO2 calibration.        ";
char sentence_AA[]="Mode        ";
char sentence_AB[]="Done: brightness set.        ";
char sentence_AC[]="Unit        ";
char sentence_AD[]="Done: unit set.        ";
char sentence_AE[]="Advanced        ";
char sentence_AF[]="Altitude setting        ";
char sentence_AG[]="calibrating,   press MENU to quit.        ";
char sentence_AH[]="Done: altitude set        "; 
char sentence_AI[]="save        ";
char sentence_AJ[]="fail        ";
char sentence_AK[]="Press MENU to quit.        ";

char sentence1[80]={"AZ 7531 WarmUp        "};// Text you want to display
char sentence2[80]={"AZ 7531 WarmUp        "};// Text you want to display
char sentence3[80]={"AZ 7531 WarmUp        "};// Text you want to display
char b=' ';//Scan the letter
u8 Buf0[16]={0};
u8 C=0;
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd

#define    ADCx                          ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC1_PORT                     GPIOA
#define    ADC_CHANNEL1                  ADC_Channel_1    
#define    ADC_CHANNEL2                  ADC_Channel_4    
#define    ADC_CHANNEL3                  ADC_Channel_5    
#define    ADC_CHANNEL4                  ADC_Channel_6    
#define    ADC_CHANNEL5                  ADC_Channel_7    
#define    ADC_NUMOFCHANNEL              5
uint16_t   ADC_ConvertedValue[ADC_NUMOFCHANNEL];


//======
enum 
{
	HOME,
	MENU,
	CENTER,
	ZONE,
	CALI,
	ADV,
	MODE,
	BUZZ,
	BL,
	UNIT,
	RESTORE,
	ALTITUDE,
	CALING
};
enum
{
	center=1,
	zone,
	cali,
	adv
};
void Barcount_Human(void);
void Barcount_Plant(void);

/*void Writeflash(void)
{
	  uint32_t  StartAddress = 0x0800f000;  //0x0807f800;
	  int32_t t;
	  volatile  FLASH_Status FLASHStatus = FLASH_COMPLETE;
	  
	  FLASH_Unlock();
	  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
    FLASHStatus = FLASH_ErasePage(StartAddress);

	 if(FLASHStatus == FLASH_COMPLETE)
	 {
	 	  SystemStatus = Alarmlevel << 16 | tmptype << 15 | timetype << 14 | bzmode << 13 |abcmode << 12 | blmode;
	    t = 0 - (adcavg1 + adcavg2 + SystemStatus);
		  FLASHStatus = FLASH_ProgramWord(StartAddress, adcavg1);
		  FLASHStatus = FLASH_ProgramWord(StartAddress + 4, adcavg2);
		  FLASHStatus = FLASH_ProgramWord(StartAddress + 8, SystemStatus);
		  FLASHStatus = FLASH_ProgramWord(StartAddress + 12, t);
		  printf("Write intenal OK\r\n");
	 }
}*/

void Writedata(void)
{
		uint32_t  StartAddress = 0x0801f000;  //0x0807f800;
	  volatile  FLASH_Status FLASHStatus = FLASH_COMPLETE;
	  
	  FLASH_Unlock();
	  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
    FLASHStatus = FLASH_ErasePage(StartAddress);
	
		if(FLASHStatus == FLASH_COMPLETE)
		{
			SystemStatus= buz<<24|Mode<<16|unit<<8|blevel;
			FLASHStatus = FLASH_ProgramWord(StartAddress, cent_co2);
			FLASHStatus = FLASH_ProgramWord(StartAddress+4, zone_co2);
			FLASHStatus = FLASH_ProgramWord(StartAddress+8, altitude);
			FLASHStatus = FLASH_ProgramWord(StartAddress+12, SystemStatus);
			FLASHStatus = FLASH_ProgramWord(StartAddress+16, offset);
		}
}

void Write_DAY_Data(void)
{
		uint32_t  StartAddress = 0x08020000;  //0x0807f800;
		volatile  FLASH_Status FLASHStatus = FLASH_COMPLETE;
	  
	  FLASH_Unlock();
	  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
	
		if(FLASHStatus == FLASH_COMPLETE)
		{
			flashptr_D++;
			FLASHStatus = FLASH_ProgramWord(StartAddress+16, offset);
		}
}
void Readflash(void)
{
	uint32_t  StartAddress = 0x0801f000;
	int *p = (int*)(StartAddress);
	cent_co2=*p;

	if(cent_co2<3000)
	{
		zone_co2=*(p+1);
		altitude=*(p+2);
		SystemStatus=*(p+3);
		offset=*(p+4);
		blevel=SystemStatus&0xff;
		unit=SystemStatus>>8&0xff;
		Mode=SystemStatus>>16&0xff;
		buz=SystemStatus>>24&0xff;
	}
	else
	{
		cent_co2=1200;
		Writedata();
	}
	
}
void BuzzerFreq(u8 c)
{
	  if(c == 2)
	  {
	  	   TIM2_PWM_Init(999, 33); 
	  	   TIM2->CCR3 = 500;
         TIM2->CCR4 = 500;	   	
	  }
	  else if(c == 1)
	  {
	  	   TIM2_PWM_Init(999, 38); 
         TIM2->CCR3 = 500;
         TIM2->CCR4 = 500;	   		  	
	  }	
	  else
	  {
	  	   TIM2_PWM_Init(999, 33); 
	  	   TIM2->CCR3 = 0;
         TIM2->CCR4 = 1500;
	  }		
}
void A2P(void)
{
	press=1013.6*pow((1-altitude/44300),5.256);
}
void Restore(void)
{
	u8 i;
	cent_co2=1200;
	zone_co2=400;
	buz=0;
	unit=0;
	blevel=4;
	Mode=1;
	altitude=0;
	A2P();
	timespc=2;
	for(i=0;i<16;i++)
	{
		co2record_m[i]=co2record_h[i]=co2record_d[i]=co2record_w[i]=0;
	}
}
void KeyScan(void)
{
	//printf("%d,%d,%d,%d\n",KEY1,KEY2,KEY3,KEY4);
	if(KEY1 == 0)
	{
		key1cnt++;
	}
	else
	{
		if(key1cnt>2)
		{
			key1short=1;
			result=0;
			cutoff=0;
			keycnt=5;
			BuzzerFreq(2);
		}
		key1cnt=0;
	}
	if(KEY2 == 0)
	{
		key2cnt++;
		if(key2cnt==300)
		{
			key2long=1;
			keycnt=5;
			BuzzerFreq(2);
		}
	}
	else
	{
		if(key2cnt>2&&key2cnt<300)
		{
			key2short=1;
			result=0;
			cutoff=0;
			keycnt=5;
			BuzzerFreq(2);
		}
		key2cnt=0;
	}
	if(KEY3 == 0)
	{
		key3cnt++;
	}
	else
	{
		if(key3cnt>2)
		{
			key3short=1;
			result=0;
			cutoff=0;
			keycnt=5;
			BuzzerFreq(2);
		}
		key3cnt=0;
	}
	if(KEY4 == 0)
	{
		key4cnt++;
	}
	else
	{
		if(key4cnt>2)
		{
			key4short=1;
			result=0;
			cutoff=0;
			keycnt=5;
			BuzzerFreq(2);
		}
		key4cnt=0;
	}
	if(keycnt)
	{
		keycnt--;
		if(keycnt==0)
			BuzzerFreq(0);
	}
}
void RCC_Configuration(uint8_t HSEx)
{  
//	ErrorStatus HSEStartUpStatus;
	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;
	
	if(HSEx==2)
	{
//		printf("RCC High\n\r");
		// Select External High-speed clock------------
  	RCC_DeInit();//clock management reset
		/* Enable HSE */    
		RCC_HSEConfig(RCC_HSE_ON); //Set the high-speed external clock signal to turn on the external crystal		 
		/* Wait till HSE is ready and if Time out is reached exit */
  	HSEStatus = RCC_WaitForHSEStartUp(); //Wait for HSE ready (waiting for external crystal oscillator ready)
  	if(HSEStatus == SUCCESS)
		{
	     FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //flash read buffer, speed up / / enable prefetch cache	
				// 0-24MHz, then select FLASH_Latency_0;
				//24-48MHz, then select FLASH_Latency_1;
				//48-72MHz, then select FLASH_Latency_2。
			 FLASH_SetLatency(FLASH_Latency_0); //Set the code delay value //The delay of the flash operation
			
	     RCC_HCLKConfig(RCC_SYSCLK_Div2); 	//Set AHB (system bus) clock (HCLK) //AHB uses system clock
	     RCC_PCLK1Config(RCC_HCLK_Div1); 		//Set low speed AHB clock (PCLK1) // APB1 is HCLK/2
	     RCC_PCLK2Config(RCC_HCLK_Div1); 		//Set high speed AHB clock (PCLK2) //APB2 is HCLK/1
				
				// Configure PLLCLK = HSE * RCC_PLLMul_x
	     RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_2); //Set the PLL clock source and multiplication factor PLLCLK = 8MHz * 9 = 72 MHz
	     RCC_PLLCmd (ENABLE); // enable PLL
				// Wait for the PLL to stabilize
			 while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}; // Wait for the PLL to be ready
    	 RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK); // set the system clock / / set the PLL as the system clock source
    	 //RCC_SYSCLKConfig (RCC_SYSCLKSource_HSE); // set the system clock / / set the PLL as the system clock source
			 //  0x00：HSI As system clock source   
			 //  0x04：HSE As system clock source   
			 //  0x08：PLL As system clock source    
    	 while(RCC_GetSYSCLKSource() != 0x08){}; // //Wait for the start of the system clock source Wait till PLL is used as system clock source 
    	 //while(RCC_GetSYSCLKSource() != 0x04){}; // //Wait for the start of the system clock source Wait till PLL is used as system clock source 
		}
		else
		{ /* If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */
		}			
	}	
	else if(HSEx==1)
	{
//		printf("RCC High\n\r");
		// Select External High-speed clock------------
  	RCC_DeInit();//clock management reset
		/* Enable HSE */    
		RCC_HSEConfig(RCC_HSE_ON); //Set the high-speed external clock signal to turn on the external crystal		 
		/* Wait till HSE is ready and if Time out is reached exit */
  	HSEStatus = RCC_WaitForHSEStartUp(); //Wait for HSE ready (waiting for external crystal oscillator ready)
  	if(HSEStatus == SUCCESS)
		{
	     FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //flash read buffer, speed up / / enable prefetch cache	
				// 0-24MHz, then select FLASH_Latency_0;
				//24-48MHz, then select FLASH_Latency_1;
				//48-72MHz, then select FLASH_Latency_2。
			 FLASH_SetLatency(FLASH_Latency_2); //Set the code delay value //The delay of the flash operation
			
	     RCC_HCLKConfig(RCC_SYSCLK_Div1); 	//Set AHB (system bus) clock (HCLK) //AHB uses system clock
	     RCC_PCLK1Config(RCC_HCLK_Div2); 		//Set low speed AHB clock (PCLK1) // APB1 is HCLK/2
	     RCC_PCLK2Config(RCC_HCLK_Div1); 		//Set high speed AHB clock (PCLK2) //APB2 is HCLK/1
				
				// Configure PLLCLK = HSE * RCC_PLLMul_x
	     RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //Set the PLL clock source and multiplication factor PLLCLK = 8MHz * 9 = 72 MHz
	     RCC_PLLCmd (ENABLE); // enable PLL
				// Wait for the PLL to stabilize
			 while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}; // Wait for the PLL to be ready
    	 RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK); // set the system clock / / set the PLL as the system clock source
    	 //RCC_SYSCLKConfig (RCC_SYSCLKSource_HSE); // set the system clock / / set the PLL as the system clock source
			 //  0x00：HSI As system clock source   
			 //  0x04：HSE As system clock source   
			 //  0x08：PLL As system clock source    
    	 while(RCC_GetSYSCLKSource() != 0x08){}; // //Wait for the start of the system clock source Wait till PLL is used as system clock source 
    	 //while(RCC_GetSYSCLKSource() != 0x04){}; // //Wait for the start of the system clock source Wait till PLL is used as system clock source 
		}
		else
		{ /* If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */
		}			
	}else{
		//---------------------------
				 
#if 1			 
			// Select Internal Low-speed clock------------		
	  //RCC_AdjustHSICalibrationValue(0);	
  	RCC_DeInit();//clock management reset
  	RCC_HSICmd(ENABLE); //Set the low-speed internal clock signal to turn on the external crystal
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){};//Wait for HSI ready (waiting for internal crystal oscillator ready)

			 //FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //flash read buffer, speed up / / enable prefetch cache	
				// 0-24MHz, then select FLASH_Latency_0;
				//24-48MHz, then select FLASH_Latency_1;
				//48-72MHz, then select FLASH_Latency_2。
			 //FLASH_SetLatency(FLASH_Latency_0); //Set the code delay value //The delay of the flash operation
			 
			 RCC_HCLKConfig(RCC_SYSCLK_Div2); 	//Set AHB (system bus) clock (HCLK) //AHB uses system clock
	     RCC_PCLK1Config(RCC_HCLK_Div2); 		//Set low speed AHB clock (PCLK1) // APB1 is HCLK/2
	     RCC_PCLK2Config(RCC_HCLK_Div2); 		//Set high speed AHB clock (PCLK2) //APB2 is HCLK/1
				
				// Configure PLLCLK = HSE * RCC_PLLMul_x
	     //RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	     //RCC_HCLKConfig(RCC_SYSCLK_Div16); 	//Set AHB (system bus) clock (HCLK) //AHB uses system clock
	     //RCC_PCLK1Config(RCC_HCLK_Div8); 		//Set low speed AHB clock (PCLK1) // APB1 is HCLK/2
	     //RCC_PCLK2Config(RCC_HCLK_Div2); 		//Set high speed AHB clock (PCLK2) //APB2 is HCLK/1
	     //RCC_HCLKConfig(RCC_SYSCLK_Div8); 	//Set AHB (system bus) clock (HCLK) //AHB uses system clock
	    // RCC_PCLK1Config(RCC_HCLK_Div2); 		//Set low speed AHB clock (PCLK1) // APB1 is HCLK/2
	    // RCC_PCLK2Config(RCC_HCLK_Div2); 		//Set high speed AHB clock (PCLK2) //APB2 is HCLK/1
				#if 1
				// Configure PLLCLK = HSI * RCC_PLLMul_x
	     RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12); //Set the PLL clock source and multiplication factor PLLCLK = 8MHz * 9 = 72 MHz
	     RCC_PLLCmd (ENABLE); // enable PLL
				// Wait for the PLL to stabilize
			 while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}; // Wait for the PLL to be ready
    	 RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK);  //RCC_SYSCLKSource_HSI); // set the system clock // set the HSI as the system clock source
			 //  0x00：HSI As system clock source   
			 //  0x04：HSE As system clock source   
			 //  0x08：PLL As system clock source    
    	 while(RCC_GetSYSCLKSource() != 0x08){}; // //Wait for the start of the system clock source Wait till HSI is used as system clock source 			 
    	 #endif
  	} 			
#endif
}	

/*void Readflash(void)
{
	  uint32_t  StartAddress = 0x0800f000;  //0x0807f800;
	  int *p = (int*)(StartAddress);
    adcavg1 = *p;
    adcavg2 =  *(p + 1);
    SystemStatus = *(p + 2);
    checksum =  *(p + 3);
}*/


void  Adc_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	//create DMA structure
  DMA_InitTypeDef DMA_InitStructure;//
	#if 1
	/* Open ADC IO clock */
	//ADC1_GPIO_APBxClock_FUN(ADC2_GPIO_CLK, ENABLE );
	// Analog I/O Definitions
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//Analog pin
	GPIO_Init(ADC1_PORT, &GPIO_InitStructure);	

	#endif
 /* Enable ADC1 clock so that we can talk to them */
	ADC_APBxClock_FUN(ADC_CLK,ENABLE );		
	//----DMA Configuration --
	//enable DMA1 clock
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
  
  //reset DMA1 channe1 to default values; 
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	//source and destination start addresses
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(ADCx->DR));	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue;	
	//Location assigned to peripheral register will be source
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
	//chunk of data to be transfered
	DMA_InitStructure.DMA_BufferSize = ADC_NUMOFCHANNEL;	
	//source address increment disable
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//automatic memory destination increment enable. 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 	
	//source and destination data size word=32bit
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	//setting circular mode
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	//medium priority
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	
	//channel will be used for memory to memory transfer
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	
	//send values to DMA registers
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// Enable DMA1 Channel
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	//----DMA	
	//printf("ADC Initialize ... !!\n\r");

  /* Put everything back to power-on defaults */
	ADC_DeInit(ADCx);  

  /* ADC1 Configuration ------------------------------------------------------*/
  /* ADC1 and ADC2 operate independently */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
  /* inhitbit the scan conversion mode */
	//ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 
  /* Don't do contimuous conversions - do them on demand */
	//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

  /* Start conversin by software, not an external trigger */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
  /* Conversions are 12 bit - put them in the lower 12 bits of the result */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
  /* Say how many channels would be used by the sequencer */
	//ADC_InitStructure.ADC_NbrOfChannel = 1;	
	ADC_InitStructure.ADC_NbrOfChannel = ADC_NUMOFCHANNEL;	
  /* Now do the setup */
  ADC_Init(ADCx, &ADC_InitStructure);	   
	
  /* PCLK2 is the APB2 clock */
  /* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  

  /* ADC1 regular channel configuration */ 
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL1, 1, ADC_SampleTime_28Cycles5 );
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL2, 2, ADC_SampleTime_28Cycles5 );
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL3, 3, ADC_SampleTime_28Cycles5 );
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL4, 4, ADC_SampleTime_28Cycles5 );
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL5, 5, ADC_SampleTime_28Cycles5 );

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADCx, ENABLE);	

  /* Enable ADC1 */
	ADC_Cmd(ADCx, ENABLE);	
	//ADC_SoftwareStartConvCmd(ADCx, ENABLE);
	//printf("ADC Initialize 1... !!\n\r");
#if 1	
{  
	u16 i;
  /* Enable ADC1 reset calibaration register */
  ADC_ResetCalibration(ADCx);
  /* Check the end of ADC1 reset calibration register */
  i = 0;
  while(ADC_GetResetCalibrationStatus(ADCx))
  {
  	 i++;
  	 if(i > 0xfff)
  	    break;
  	 //printf("+");
  }	
	//printf("ADC Initialize 2... !!\n\r");
	
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADCx);
  /* Check the end of ADC1 calibration */
  i = 0;
  while(ADC_GetCalibrationStatus(ADCx))
  {
  	 i++;
  	 if(i > 0xfff)
  	    break;
  	  //printf("-");   
  }	  
	//printf("ADC Initialize OK !!\n\r");
}	
#endif
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}	

	

void InitReadGain(u8 c)
{
 
   GPIO_InitTypeDef  GPIO_InitStructure;
 	
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PB.5 端口配置
   if(c) //Gain on
   {
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
      GPIO_Init(GPIOC, &GPIO_InitStructure);		
      GPIO_ResetBits(GPIOC,GPIO_Pin_13);		
   }
   else
   {
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
      GPIO_Init(GPIOC, &GPIO_InitStructure);		   	
   } 
}



#define   SET_UART_RATE    19200 //9880
void Init_Sentence(u8 rgb)
{
	u8 i;
	C=0;
	wordptr=0;
	finish_sentence=0;
	for(i=0;i<15;i++)
	{
		b=sentence1[C];
		Run_Bar(b);
	}
	delay=15;
	switch(rgb)
	{
		case 1:
			TM1629_WriteBlock(2,0,16,Buf2);
			break;
		case 2:
			STB3transfer();
			TM1629_WriteBlock(3,0,16,Buf3);
			break;
		case 3:
			TM1629_WriteBlock(2,0,16,Buf2);
	    STB3transfer();
	    TM1629_WriteBlock(3,0,16,Buf3);
			break;
	}
}
void Display_Bl(u8 b)
{
	u8 i;
	C=0;
	wordptr=0;
	finish_sentence=0;
	runbarenable=0;
	LED_Show_Zone(b,1);
	switch(b)
	{
		case 1:
			for(i=1;i<5;i++)
				TM1629_SendCmd(0x88,i);
			break;
		case 2:
			for(i=1;i<5;i++)
				TM1629_SendCmd(0x8a,i);
			break;
		case 3:
			for(i=1;i<5;i++)
				TM1629_SendCmd(0x8b,i);
			break;
		case 4:
			for(i=1;i<5;i++)
				TM1629_SendCmd(0x8f,i);
			break;
	}
  TM1629_WriteBlock(2,0,16,Buf2);
	STB3transfer();
	TM1629_WriteBlock(3,0,16,Buf3);
}
void Display_Result(void)
{
	u8 i;
	C=0;
	wordptr=0;
	finish_sentence=0;
	result=1;
	delay=0;
	for(i=0;i<16;i++)
	{
		Buf2[i]=0;
	}
	switch(state)
	{
		case CENTER:
			strcpy(sentence3,sentence_G);
			strcpy(sentence1,sentence_X);
			strcpy(sentence2,sentence_X);
			break;
		case ZONE:
			strcpy(sentence3,sentence_H);
			strcpy(sentence1,sentence_Y);
			strcpy(sentence2,sentence_Y);
			break;
		case MODE:
			if(Mode)
				strcpy(sentence3,sentence_S);
			else
				strcpy(sentence3,sentence_R);
			strcpy(sentence1,sentence_AA);
			strcpy(sentence2,sentence_AA);
			break;
		case BUZZ:
			if(buz)
				strcpy(sentence3,sentence_P);
			else
				strcpy(sentence3,sentence_O);
			strcpy(sentence1,sentence_I);
			strcpy(sentence2,sentence_I);
			break;
		case BL:
			strcpy(sentence3,sentence_AB);
			strcpy(sentence1,sentence_Q);
			strcpy(sentence2,sentence_Q);
			break;
		case UNIT:
			strcpy(sentence3,sentence_AD);
			strcpy(sentence1,sentence_AC);
			strcpy(sentence2,sentence_AC);
			break;
		case ALTITUDE:
			strcpy(sentence3,sentence_AH);
			strcpy(sentence1,sentence_AF);
			strcpy(sentence2,sentence_AF);
			break;
		case RESTORE:
			strcpy(sentence3,sentence_T);
			break;
	}
}
void Display_ONOFF(u8 o)
{
	runbarenable=0;
	TM1629_WriteBlock(2,0,16,Buf0);
	TM1629_WriteBlock(3,0,16,Buf0);
	if(o)
	{
		strcpy(sentence1,sentence_M);
		Init_Sentence(2);
	}
	else
	{
		strcpy(sentence1,sentence_N);
		Init_Sentence(1);
	}
	
}
void Display_Calresult(u8 r)
{
	runbarenable=0;
	TM1629_WriteBlock(2,0,16,Buf0);
	TM1629_WriteBlock(3,0,16,Buf0);
	if(r)
	{
		strcpy(sentence1,sentence_AI);
		Init_Sentence(2);
	}
	else
	{
		strcpy(sentence1,sentence_AJ);
		Init_Sentence(1);
	}
}
void Display_Mode(u8 m)
{
	if(m)
	{
		strcpy(sentence1,sentence_K);
		strcpy(sentence2,sentence_K);
	}
	else
	{
		strcpy(sentence1,sentence_L);
		strcpy(sentence2,sentence_L);
	}
	LED_Show_Mode(m,1);
	Init_Sentence(3);
	TM1629_WriteBlock(5,0,16,Buf5);
}
void Display_menu()
{
	switch(menu)
	{
		case 1:
			strcpy(sentence1,sentence_X);
			strcpy(sentence2,sentence_X);	  
			break;
		case 2:
			strcpy(sentence1,sentence_Y);
			strcpy(sentence2,sentence_Y);	  
			break;
		case 3:
			strcpy(sentence1,sentence_Z);
			strcpy(sentence2,sentence_Z);	  
			break;
		case 4:
			strcpy(sentence1,sentence_AE);
			strcpy(sentence2,sentence_AE);	 				
			break;
	}
	switch(state)
	{
		case CALI:
			strcpy(sentence1,sentence_F);
			strcpy(sentence2,sentence_F);
			break;
		case CALING:
			strcpy(sentence1,sentence_AG);
			strcpy(sentence2,sentence_AG);
			break;
	}
	LED_Show_Page(menu*10);
	Init_Sentence(3);
	TM1629_WriteBlock(4,0,16,Buf4);
}
void Display_Setting(void)
{
	switch(state)
	{
		case CENTER:
			strcpy(sentence1,sentence_C);
			strcpy(sentence2,sentence_E);	
			break;
		case ZONE:
			strcpy(sentence1,sentence_D);
			strcpy(sentence2,sentence_E);	
			break;
		case ALTITUDE:
			strcpy(sentence1,sentence_AF);
			strcpy(sentence2,sentence_E);
			break;
		case RESTORE:
			strcpy(sentence1,sentence_U);
			strcpy(sentence2,sentence_V);	
			break;
	}
	Init_Sentence(3);
}
void Display_option(void)
{
	switch(option)
	{
		case 1://buzzer
			strcpy(sentence1,sentence_I);
			strcpy(sentence2,sentence_I);
			break;
		case 2://unit
			strcpy(sentence1,sentence_AC);
			strcpy(sentence2,sentence_AC);
			break;
		case 3://backlight
			strcpy(sentence1,sentence_Q);
			strcpy(sentence2,sentence_Q);
			break;
		case 4://restore factory
			strcpy(sentence1,sentence_U);
			strcpy(sentence2,sentence_U);
			break;
		case 5://mode
			strcpy(sentence1,sentence_AA);
			strcpy(sentence2,sentence_AA);
			break;
		case 6://altitude
			strcpy(sentence1,sentence_AF);
			strcpy(sentence2,sentence_AF);
			break;
	}
	LED_Show_Page(menu*10+option);
	Init_Sentence(3);
	TM1629_WriteBlock(4,0,16,Buf4);
}
void Display_CF(u8 unit)
{
	runbarenable=0;
	TM1629_WriteBlock(2,0,16,Buf0);
	TM1629_WriteBlock(3,0,16,Buf0);
	
	LED_Show_CF(unit);
	STB3transfer();
	TM1629_WriteBlock(2,0,16,Buf2);
	TM1629_WriteBlock(3,0,16,Buf3);
}
void Display_Bar(void)
{
	Barcount_Human();
	switch(timespc)
	{
		case 0:
			LED_Show_Bar_Human(co2bar_m,co2record_m,m);
			if(display)
			{
				if(m<16)
						LED_Show_MAX(maxco2_m,minco2_m);
				else
						LED_Show_Dash();
			}
			break;
		case 1:
			LED_Show_Bar_Human(co2bar_h,co2record_h,h);
			if(display)
			{
				if(h<16)
						LED_Show_MAX(maxco2_h,minco2_h);
				else
						LED_Show_Dash();
			}
			break;
		case 2:
			LED_Show_Bar_Human(co2bar_d,co2record_d,d);
			if(display)
			{
				if(d<16)
						LED_Show_MAX(maxco2_d,minco2_d);
				else
						LED_Show_Dash();
			}
			break;
		case 3:
			LED_Show_Bar_Human(co2bar_w,co2record_w,w);
			if(display)
			{
				if(w<16)
						LED_Show_MAX(maxco2_w,minco2_w);
				else
						LED_Show_Dash();
			}
			break;
	}
}
void Display_Home(void)
{
	LED_Show_CO2(co2);
	LED_Show_Temp(temp,unit);
	LED_Show_RH(rh);
	LED_Show_Timespc(timespc);
	LED_Show_Display(display);
	Display_Bar();
	LED_Show_Mode(Mode,1);
	LED_Show_Buz(buz,1);
	LED_Show_Light(light);
	LED_Show_Relay(RELAY);
	//LED_Show_Time();
			  
	
	TM1629_WriteBlock(1,0,16,Buf1);
	TM1629_WriteBlock(2,0,16,Buf2);
	TM1629_WriteBlock(3,0,16,Buf3);
	TM1629_WriteBlock(4,0,16,Buf4);
	TM1629_WriteBlock(5,0,16,Buf5);
}
void Display_Info(void)
{
	LED_Show_CO2(co2);
	
	TM1629_WriteBlock(1,0,16,Buf1);
}
void Key1s(void)
{
	if(key1short)/////key menu
	{
		key1short=0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
		switch(state)
		{
			case HOME:
				state=MENU;
				menu=1;
				runbarenable=1;
				Display_menu();
			  Clear_Zone();
			  TM1629_WriteBlock(1,0,16,Buf1);
				break;
			case MENU:
				menu++;
				if(menu>4)
				{
					menu=center;
					state=HOME;
					runbarenable=0;
					LED_Clear();
					Display_Home();
				}
				else
					Display_menu();
				break;
			case CENTER:case ZONE:case CALI:case ADV:
				state=MENU;
				Display_menu();
			  Clear_Zone();
			  runbarenable=1;
				break;
			case CALING:
				menu=center;
				state=HOME;
				runbarenable=0;
				LED_Clear();
				Display_Home();
				calcnt=250;
				caled=0;
				break;		
			case BUZZ:case UNIT:
				state=ADV;
				runbarenable=1;
				Display_option();
				LED_Show_Temp(temp,1);
				LED_Show_Buz(buz,1);
				TM1629_WriteBlock(1,0,16,Buf1);
				break;
			case MODE:
				state=ADV;
				Display_option();
				LED_Show_Mode(Mode,1);
				TM1629_WriteBlock(5,0,16,Buf5);
				break;
			case BL:
			{
				u8 i;
				state=ADV;
				Display_option();
				switch(blevel)
				{
					case 1:
						for(i=1;i<5;i++)
							TM1629_SendCmd(0x88,i);
						break;
					case 2:
						for(i=1;i<5;i++)
							TM1629_SendCmd(0x8a,i);
						break;
					case 3:
						for(i=1;i<5;i++)
							TM1629_SendCmd(0x8b,i);
						break;
					case 4:
						for(i=1;i<5;i++)
							TM1629_SendCmd(0x8f,i);
						break;
				}
			}
				break;
			case ALTITUDE:
				state=ADV;
				Clear_Zone();
				Display_option();
				break;
		}
	}
}
void Key2s(void)
{
	if(key2short)/////key ENTER
	{
		key2short=0;
		switch(state)
		{
			case HOME:
				Readflash();
				break;
			case MENU:
				switch(menu)
				{
					case center:
						state=CENTER;
						cent_co2_=cent_co2;
						LED_Show_Zone(cent_co2_,1);
						Display_Setting();
						break;
					case zone:
						state=ZONE;
						zone_co2_=zone_co2;
						LED_Show_Zone(zone_co2,1);
						Display_Setting();
						break;
					case cali:
						state=CALI;
						//LED_Show_Zone(calcnt,1);
						LED_Show_CO2(calpoint);
						TM1629_WriteBlock(1,0,16,Buf1);
					  Display_menu();
						break;
					case adv:
						state=ADV;
						option=1;
						Display_option();
						break;
				}		
				break;
			case ADV:
				switch(option)
				{
					case 1:
						state=BUZZ;
						buz_=buz;
						Display_ONOFF(buz);
						break;
					case 2:
						state=UNIT;
						unit_=unit;
						Display_CF(unit);
						break;
					case 3:
						state=BL;
						blevel_=blevel;
						Display_Bl(blevel_);
						LED_Show_Bar_All();
						break;
					case 4:
						state=RESTORE;
						Display_Setting();
						break;
					case 5:
						state=MODE;
						Mode_=Mode;
						Display_Mode(Mode_);
						break;
					case 6:
						state=ALTITUDE;
						altitude_=altitude;
						LED_Show_Zone(altitude_,1);
						Display_Setting();
						break;
				}
				break;
			case CENTER:
				Clear_Zone();
				Display_Result();
				cent_co2=cent_co2_;
				Writedata();
				state=MENU;
				break;
			case ZONE:
				Clear_Zone();
				Display_Result();
				zone_co2=zone_co2_;
				Writedata();
				state=MENU;
				break;
			case BUZZ:
				runbarenable=1;
				buz=buz_;
				Display_Result();
				Writedata();
				LED_Show_Buz(buz,1);
				TM1629_WriteBlock(1,0,16,Buf1);
				state=ADV;
				break;
			case MODE:
				Mode=Mode_;
				Display_Result();
				Writedata();
				LED_Show_Mode(Mode,1);
				TM1629_WriteBlock(5,0,16,Buf5);
				state=ADV;
				break;
			case BL:
				runbarenable=1;
				blevel=blevel_;
				Display_Result();
				Writedata();
				Clear_Zone();
				state=ADV;
				break;
			case UNIT:
				runbarenable=1;
				unit=unit_;
				Display_Result();
				Writedata();
				state=ADV;
				break;
			case ALTITUDE:
				A2P();
				Clear_Zone();
				runbarenable=1;
				altitude=altitude_;
				Display_Result();
				Writedata();
				state=ADV;
				break;
		}
	}
}
void Key3s(void)/////key up
{
	if(key3short)
	{
		key3short=0;
		switch(state)
		{
			case HOME:
				display++;
				if(display>1)
					display=0;
				LED_Show_Display(display);
				break;
			case MENU:
				menu--;
				if(menu<1)
				{
					menu=center;
					state=HOME;
					runbarenable=0;
					LED_Clear();
					Display_Home();
				}
				else
					Display_menu();
				break;
			case CENTER:
				cent_co2_+=50;
				if(cent_co2_>3000)
					cent_co2_=400;
				LED_Show_Zone(cent_co2_,1);
				break;
			case ZONE:
				zone_co2_+=10;
				if(zone_co2_>2*cent_co2||zone_co2_>990)
					zone_co2_=50;
				LED_Show_Zone(zone_co2_,1);
			break;
			case CALI:
				calpoint+=10;
				if(calpoint>600)
					calpoint=300;
				LED_Show_CO2(calpoint);
				TM1629_WriteBlock(1,0,16,Buf1);
				break;
			case ADV:
				option--;
				if(option<1)
					option=6;
				Display_option();
				break;
			case MODE:
				Mode_++;
				if(Mode_>1)
					Mode_=0;
				Display_Mode(Mode_);
				TM1629_WriteBlock(5,0,16,Buf5);
				break;
			case BUZZ:
				buz_++;
				if(buz_>1)
					buz_=0;
				Display_ONOFF(buz_);
				LED_Show_Buz(buz_,1);
				TM1629_WriteBlock(1,0,16,Buf1);
				break;
			case BL:
				blevel_++;
				if(blevel_>4)
					blevel_=1;
				Display_Bl(blevel_);
				break;
			case UNIT:
				unit_++;
				if(unit_>1)
					unit_=0;
				LED_Show_Temp(temp,unit_);
				Display_CF(unit_);
				TM1629_WriteBlock(1,0,16,Buf1);
				break;
			case ALTITUDE:
				altitude_+=50;
				if(altitude_>5000)
					altitude_=0;
				LED_Show_Zone(altitude_,1);
				break;
		}
	}
}
void Key4s(void)/////key down
{
	if(key4short)
	{
		key4short=0;
		switch(state)
		{
			case HOME:
				timespc++;
				if(timespc>3)
					timespc=0;
				Display_Bar();
				LED_Show_Timespc(timespc);
				TM1629_WriteBlock(5,0,16,Buf5);
				TM1629_WriteBlock(2,0,16,Buf2);
				TM1629_WriteBlock(3,0,16,Buf3);
				TM1629_WriteBlock(4,0,16,Buf4);
				
				break;
			case MENU:
				menu++;
				if(menu>4)
				{
					menu=center;
					state=HOME;
					runbarenable=0;
					LED_Clear();
					Display_Home();
				}
				else
					Display_menu();
				break;
			case CENTER:
				cent_co2_-=50;
				if(cent_co2_<400)
					cent_co2_=3000;
				LED_Show_Zone(cent_co2_,1);
				break;
			case ZONE:
				zone_co2_-=10;
				if(zone_co2_<50)
					zone_co2_=2*cent_co2;
				if(zone_co2_>990)
					zone_co2_=990;
				LED_Show_Zone(zone_co2_,1);
				break;
			case CALI:
				calpoint-=10;
				if(calpoint<300)
					calpoint=600;
				LED_Show_CO2(calpoint);
				TM1629_WriteBlock(1,0,16,Buf1);
				break;
			case ADV:
				option++;
				if(option>6)
					option=1;
				Display_option();
				break;
			case BUZZ:
				buz_++;
				if(buz_>1)
					buz_=0;
				Display_ONOFF(buz_);
				LED_Show_Buz(buz_,1);
				TM1629_WriteBlock(1,0,16,Buf1);
				break;
			case MODE:
				Mode_++;
				if(Mode_>1)
					Mode_=0;
				Display_Mode(Mode_);
				TM1629_WriteBlock(5,0,16,Buf5);
				break;
			case BL:
				blevel_--;
				if(blevel_<1)
					blevel_=4;
				Display_Bl(blevel_);
				break;
			case UNIT:
				unit_++;
				if(unit_>1)
					unit_=0;
				Display_CF(unit_);
				LED_Show_Temp(temp,unit_);
				TM1629_WriteBlock(1,0,16,Buf1);
				break;
			case ALTITUDE:
				altitude_-=50;
				if(altitude_<0)
					altitude_=5000;
				LED_Show_Zone(altitude_,1);
				break;
		}
	}
}
void Key2L(void)
{
	if(key2long)
	{
		key2long=0;
		switch(state)
		{
			case CALI:
				state=CALING;
			  Display_menu();
				break;
			case RESTORE:
				Display_Result();
				Restore();
				state=MENU;
				break;
		}
	}
}
int Average(int x[])
{
	u8 i;
	u8 a=0;
	int sum=0;
	for(i=0;i<16;i++)
	{
		if(x[i])
		{
			sum+=x[i];
			a++;
		}
	}
	sum/=a;
	return sum;
}
void CO2Data_Insert(void)
{
	signed char i;
	u32 c;
	c=RTC_GetCounter();
	if(c-cnt_m>=4)
	{
		cnt_m=c;
		if(m)
			m--;
		for(i=0;i<15;i++)
		{
			co2record_m[i]=co2record_m[i+1];
		}
		co2record_m[15]=co2;
		
		minco2_m=maxco2_m=co2record_m[15];
		
		for(i=15;i>=m;i--)
		{
			if(co2record_m[i]<minco2_m)
				minco2_m=co2record_m[i];
			
			if(co2record_m[i]>maxco2_m)
				maxco2_m=co2record_m[i];
		}
	}
	if(c-cnt_h>=240)
	{	
		cnt_h=c;
		if(h)
			h--;
		for(i=0;i<15;i++)
		{
			co2record_h[i]=co2record_h[i+1];
		}
		co2record_h[15]=Average(co2record_m);
		minco2_h=maxco2_h=co2record_h[15];
		
		for(i=15;i>=h;i--)
		{
			if(co2record_h[i]<minco2_h)
				minco2_h=co2record_h[i];
			
			if(co2record_h[i]>maxco2_h)
				maxco2_h=co2record_h[i];
		}
	}
	if(c-cnt_d>=5760)
	{	
		cnt_d=c;
		if(d)
			d--;
		for(i=0;i<15;i++)
		{
			co2record_d[i]=co2record_d[i+1];
		}
		co2record_d[15]=Average(co2record_h);
		
		minco2_d=maxco2_d=co2record_d[15];
		
		for(i=15;i>=d;i--)
		{
			if(co2record_d[i]<minco2_d)
				minco2_d=co2record_d[i];
			
			if(co2record_d[i]>maxco2_d)
				maxco2_d=co2record_d[i];
		}
	}
	if(c-cnt_w>=40320)
	{
		cnt_w=c;
		if(w)
			w--;
		for(i=0;i<15;i++)
		{
			co2record_w[i]=co2record_w[i+1];
		}
		co2record_w[15]=Average(co2record_d);
		
		minco2_w=maxco2_w=co2record_w[15];
		
		for(i=15;i>=w;i--)
		{
			if(co2record_w[i]<minco2_w)
				minco2_w=co2record_w[i];
			
			if(co2record_w[i]>maxco2_w)
				maxco2_w=co2record_w[i];
		}
	}
}
void Barcount_Plant(void)
{
	int unit;

	signed char i;
	
	/*unit=(maxco2_m-minco2_m)/8;
	if(unit<5)
		unit=5;
	centline=(cent_co2-minco2_m)/unit+1;
	if(centline<1)
		centline=1;
	if(centline>8)
		centline=8;
	printf("C=%d ,",centline);
	
	for(i=15;i>=m;i--)
	{
		if(co2bar_m[i]>cent_co2)
			co2bar_m[i]=(co2record_m[i]-cent_co2)/unit+1;
		else if(co2bar_m[i]<cent_co2)
			co2bar_m[i]=(co2record_m[i]-cent_co2)/unit-1;
		else
			co2bar_m[i]=(co2record_m[i]-cent_co2)/unit;
		
		if(co2bar_m[i]>7)
			co2bar_m[i]=7;
		if(co2bar_m[i]<-7)
			co2bar_m[i]=-7;
		
		if(co2bar_m[i]+centline>8)
			co2bar_m[i]--;
		if(co2bar_m[i]+centline<1)
			co2bar_m[i]++;
	}
	
	for(i=0;i<16;i++)
	{
		printf("%d ,",co2bar_m[i]);
	}
	printf("\n");*/
	switch(timespc)
	{
		case 0:
			switch(display)
			{
				case 0:
					unit=zone_co2/6;
					for(i=15;i>=m;i--)
					{
						co2bar_m[i]=(co2record_m[i]-cent_co2)/unit;
						if(co2bar_m[i]>3)
							co2bar_m[i]=3;
						if(co2bar_m[i]<-3)
							co2bar_m[i]=-3;
						if(co2bar_m[i]==3&&co2record_m[i]<cent_co2+0.5*zone_co2)
							co2bar_m[i]=2;
						if(co2bar_m[i]==-3&&co2record_m[i]>cent_co2-0.5*zone_co2)
							co2bar_m[i]=-2;
					}
					break;
			}
			break;
		case 1:
			switch(display)
			{
				case 0:
					unit=zone_co2/6;
					for(i=15;i>=h;i--)
					{
						co2bar_h[i]=(co2record_h[i]-cent_co2)/unit;
						if(co2bar_h[i]>3)
							co2bar_h[i]=3;
						if(co2bar_h[i]<-3)
							co2bar_h[i]=-3;
						if(co2bar_h[i]==3&&co2record_h[i]<cent_co2+0.5*zone_co2)
							co2bar_h[i]=2;
						if(co2bar_h[i]==-3&&co2record_h[i]>cent_co2-0.5*zone_co2)
							co2bar_h[i]=-2;
					}
					break;
			}
			break;
		case 2:
			switch(display)
			{
				case 0:
					unit=zone_co2/6;
					for(i=15;i>=d;i--)
					{
						co2bar_d[i]=(co2record_d[i]-cent_co2)/unit;
						if(co2bar_d[i]>3)
							co2bar_d[i]=3;
						if(co2bar_d[i]<-3)
							co2bar_d[i]=-3;
						if(co2bar_d[i]==3&&co2record_d[i]<cent_co2+0.5*zone_co2)
							co2bar_d[i]=2;
						if(co2bar_d[i]==-3&&co2record_d[i]>cent_co2-0.5*zone_co2)
							co2bar_d[i]=-2;
					}
					break;
			}
			break;
		case 3:
			switch(display)
			{
				case 0:
					unit=zone_co2/6;
					for(i=15;i>=w;i--)
					{
						co2bar_w[i]=(co2record_w[i]-cent_co2)/unit;
						if(co2bar_w[i]>3)
							co2bar_w[i]=3;
						if(co2bar_w[i]<-3)
							co2bar_w[i]=-3;
						if(co2bar_w[i]==3&&co2record_w[i]<cent_co2+0.5*zone_co2)
							co2bar_w[i]=2;
						if(co2bar_w[i]==-3&&co2record_w[i]>cent_co2-0.5*zone_co2)
							co2bar_w[i]=-2;
					}
					break;
			}
			break;
	}
}
void Barcount_Human(void)
{
	int unit;
	signed char i;
	u8 t=0;
	switch(timespc)
	{
		case 0:
					unit=(maxco2_m-minco2_m)/8;
					if(unit<2)
						unit=2;
					for(i=15;i>=m;i--)
					{
						co2bar_m[i]=(co2record_m[i]-minco2_m)/unit+1;
						if(co2bar_m[i]>8)
							co2bar_m[i]=8;
						if(co2bar_m[i]==1)
						{
							t++;
						}
					}
					if(Mode)
					{
						Targetdown=(cent_co2-0.5*zone_co2-minco2_m)/unit+1;
						Targetup=(cent_co2+0.5*zone_co2-minco2_m)/unit+1;
					}
					else
					{
						Targetdown=0;
						Targetup=(cent_co2+zone_co2-minco2_m)/unit+1;
					}
					if(Targetdown<0)
						Targetdown=0;
					if(Targetdown>8)
						Targetdown=9;
					if(Targetup>8)
						Targetup=9;
					if(Targetup<0)
						Targetup=0;
					printf("%d,%d,%d\n",Targetup,Targetdown,unit);
			break;
		case 1:
					unit=(maxco2_h-minco2_h)/8;
					if(unit<2)
						unit=2;
					for(i=15;i>=h;i--)
					{
						co2bar_h[i]=(co2record_h[i]-minco2_h)/unit+1;
						if(co2bar_h[i]>8)
							co2bar_h[i]=8;
						if(co2bar_h[i]==1)
						{
							t++;
						}
					}
					if(Mode)
					{
						Targetdown=(cent_co2-0.5*zone_co2-minco2_h)/unit+1;
						Targetup=(cent_co2+0.5*zone_co2-minco2_h)/unit+1;
					}
					else
					{
						Targetdown=0;
						Targetup=(cent_co2+zone_co2-minco2_h)/unit+1;
					}
					if(h==16)
						Targetdown=Targetup=0;
					if(Targetdown<0)
						Targetdown=0;
					if(Targetdown>8)
						Targetdown=9;
					if(Targetup>8)
						Targetup=9;
					if(Targetup<0)
						Targetup=0;
					//printf("%d,%d,%d\n",Targetup,Targetdown,unit);
			break;
		case 2:
				  unit=(maxco2_d-minco2_d)/8;
					if(unit<2)
						unit=2;
					for(i=15;i>=d;i--)
					{
						co2bar_d[i]=(co2record_d[i]-minco2_d)/unit+1;
						if(co2bar_d[i]>8)
							co2bar_d[i]=8;
						if(co2bar_d[i]==1)
						{
							t++;
						}
					}
					if(Mode)
					{
						Targetdown=(cent_co2-0.5*zone_co2-minco2_d)/unit+1;
						Targetup=(cent_co2+0.5*zone_co2-minco2_d)/unit+1;
					}
					else
					{
						Targetdown=0;
						Targetup=(cent_co2+zone_co2-minco2_d)/unit+1;
					}
					if(d==16)
						Targetdown=Targetup=0;
					if(Targetdown<0)
						Targetdown=0;
					if(Targetdown>8)
						Targetdown=9;
					if(Targetup>8)
						Targetup=9;
					if(Targetup<0)
						Targetup=0;
					//printf("%d,%d,%d\n",Targetup,Targetdown,unit);
			break;
		case 3:
					unit=(maxco2_w-minco2_w)/8;
					if(unit<2)
						unit=2;
					for(i=15;i>=w;i--)
					{
						co2bar_w[i]=(co2record_w[i]-minco2_w)/unit+1;
						if(co2bar_w[i]>8)
							co2bar_w[i]=8;
						if(co2bar_w[i]==1)
						{
							t++;
						}
					}
					if(Mode)
					{
						Targetdown=(cent_co2-0.5*zone_co2-minco2_w)/unit+1;
						Targetup=(cent_co2+0.5*zone_co2-minco2_w)/unit+1;
					}
					else
					{
						Targetdown=0;
						Targetup=(cent_co2+zone_co2-minco2_w)/unit+1;
					}
					if(w==16)
						Targetdown=Targetup=0;
					if(Targetdown<0)
						Targetdown=0;
					if(Targetdown>8)
						Targetdown=9;
					if(Targetup>8)
						Targetup=9;
					if(Targetup<0)
						Targetup=0;
					//printf("%d,%d,%d\n",Targetup,Targetdown,unit);
					break;
			}
}
void EXE_Cal(void)
{
	short offset_;
	
	if(state==CALING)
	{
		if(calcnt)
			calcnt--;
		LED_Show_Zone(calcnt,1);
		if(calcnt==0&&!caled)
		{
			offset_=calpoint-co2;
			caled=1;
			if(offset_<200&&offset_>-200)
			{
				offset=offset_;
				Display_Calresult(1);
			}
			else
			{
				Display_Calresult(0);
			}
			
		}
	}
}
void EXE_Countdown(void)
{
	if(count10)
	{
		LED_Show_CO2(count10);
		count10--;
		TM1629_WriteBlock(1,0,16,Buf1);
		if(count10==0)
		{
			LED_Clear();
			runbarenable=0;
			delay_ms(1000);
		}
	}
}
void EXE_Relay(void)
{
	if(!count10)
	{
		if(Mode)
		{
			if(light)
			{
			if(co2>cent_co2+0.25*zone_co2)
			{
				Bufco2[0]=Bufco2[1];
				Bufco2[1]=co2;
							
				if(Bufco2[1]>=Bufco2[0])
					add_cnt++;
				else
					add_cnt=0;
							
				if(add_cnt>30)
				{
					add_cnt=0;
					RELAY=0;
				}
			}
			if(co2<cent_co2-0.25*zone_co2)
			{
				Bufco2[0]=Bufco2[1];
				Bufco2[1]=co2;
							
				if(Bufco2[1]<=Bufco2[0])
					minus_cnt++;
				else
					minus_cnt=0;
							
				if(minus_cnt>=30)
				{
					minus_cnt=0;
					RELAY=1;
				}
			}
			if(co2>cent_co2+0.5*zone_co2)
				RELAY=0;
			if(co2<cent_co2-0.5*zone_co2)
				RELAY=1;
			}
			else
			{
				RELAY=0;
				minus_cnt=0;
				add_cnt=0;
			}
		}
		else
		{
			if(co2>cent_co2+zone_co2)
				RELAY=1;
			else
				RELAY=0;
		}
		LED_Show_Relay(RELAY);
		TM1629_WriteBlock(5,0,16,Buf5);
	}
}
void EXE_Buzzer(void)
{
	if(buz&&!count10)
	{
		if(Mode)
		{
			if(co2>cent_co2+0.5*zone_co2||co2<cent_co2-0.5*zone_co2)
			{
				buzzing=1;
				if(state==HOME)
					BuzzerFreq(2);
			}
			else
			{
				buzzing=0;
				BuzzerFreq(0);
			}
		}
		else
		{
			if(co2>cent_co2+zone_co2)
			{
				buzzing=1;
				if(state==HOME)
					BuzzerFreq(2);
			}
			else
			{
				buzzing=0;
				BuzzerFreq(0);
			}
		}
	}
}

int  main(void)
{	 
  //u32 t;
	u16  Res;
	delay_init();	    	 //延时函数初始化	  
	//SystemInit1();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	SystemInit();
	RCC_Configuration(1);
	LED_Init();		 
	uart_init(SET_UART_RATE);	 	//串口初始化为115200
	
	//Disable_USART2();
	USB_Port_Set(0); 	    //USB先断开
	USART3_Init(SET_UART_RATE);
	Readflash();
	#ifndef SUPPORT_88165_TEMP 	
	//USART2_Init(SET_UART_RATE);
	#endif
	//LED_Init();		  		  //初始化与LED连接的硬件接口
 	//URESET = 0;
 	//Set_USBClock();   
//USB_Interrupts_Config();    
 	//USB_Init();	    
	//USB_Port_Set(1);	    //USB再次连接
	//SHT3X_Init();
	TIM4_Init(1000,10); //250us interval
	RTC_Init();
	//Adc_Init();
  //LEDwritecnt = 0;	
  //updatebyte = updatesecond = 0;
  //u2_printf("UART2 INIT..\r\n");
  
  //InitReadGain(0);
	//Islowvolt = 0;
	//VSET = 1;
  
  //adcavg1 = 100;
  //Writeflash();
  
  
  //printf(",READ= %x , %x, %x, %x,\r\n",adcavg1,adcavg2,SystemStatus,checksum);   
  TM1629_Config(1);
	TM1629_Config(2);
	TM1629_Config(3);
	TM1629_Config(4);
	TM1629_Config(5);
	
	switch(blevel)
	{
		case 1:
			TM1629_SendCmd(0x88,1);
			TM1629_SendCmd(0x88,2);
			TM1629_SendCmd(0x88,3);
			TM1629_SendCmd(0x88,4);
			TM1629_SendCmd(0x88,5);
			break;
		case 2:
			TM1629_SendCmd(0x8a,1);
			TM1629_SendCmd(0x8a,2);
			TM1629_SendCmd(0x8a,3);
			TM1629_SendCmd(0x8a,4);
			TM1629_SendCmd(0x8a,5);
			break;
		case 3:
			TM1629_SendCmd(0x8b,1);
			TM1629_SendCmd(0x8b,2);
			TM1629_SendCmd(0x8b,3);
			TM1629_SendCmd(0x8b,4);
			TM1629_SendCmd(0x8b,5);
			break;
		case 4:
			TM1629_SendCmd(0x8f,1);
			TM1629_SendCmd(0x8f,2);
			TM1629_SendCmd(0x8f,3);
			TM1629_SendCmd(0x8f,4);
			TM1629_SendCmd(0x8f,5);
			break;
	}

  TM1629_WriteBlock(1,0,16,Bufa);
	TM1629_WriteBlock(2,0,16,Bufa);
	TM1629_WriteBlock(3,0,16,Bufa);
	TM1629_WriteBlock(4,0,16,Bufa);
	TM1629_WriteBlock(5,0,16,Bufa);
	BuzzerFreq(2);
	state=0;
	delay_ms(500);
	BuzzerFreq(0);
	TM1629_WriteBlock(1,0,16,Buf1);
	TM1629_WriteBlock(2,0,16,Buf2);
	TM1629_WriteBlock(3,0,16,Buf3);
	TM1629_WriteBlock(4,0,16,Buf4);
	TM1629_WriteBlock(5,0,16,Buf5);
	//RTC_Set(2023,4,11,10,10,23);
	//Readflash();
	cnt_m=cnt_h=cnt_d=cnt_w=RTC_GetCounter();
	A2P();
	while(1)
	{
		//B10msTimer++;
		 if(B1sTimer>700)
		 {
				B1sTimer=0;
				STATE++;
				if(STATE>9)
					STATE=0;
				switch(STATE)
				{
					case 0:
						RTC_Get();
						if(!count10)
						{
							CO2Data_Insert();
						}
						break;
					case 1:
						EXE_Countdown();
						break;
					case 2:
						if(!count10&&state==HOME)
						{
							Display_Home();
						}
						break;
					case 3:
						EXE_Cal();
						break;
					case 4:
						EXE_Relay();
						break;
					case 5:
						EXE_Buzzer();
						break;
					case 6:
						if(state!=HOME&&state!=CALI&&state!=CALING)
							Display_Info();
						break;
					case 7:
						if(state!=HOME&&state!=CALING)
							cutoff++;
						if(cutoff>60)
						{
							cutoff=0;
							menu=center;
							state=HOME;
							runbarenable=0;
							LED_Clear();
							Display_Home();
						}
						break;
					case 8:
						if(buzzing)
						{
							BuzzerFreq(0);
						}
						break;
				}
		 }
		 if(B10msTimer > 50) /////for key scan
		 {
			 if(!count10)
			 {
					KeyScan();
					Key1s();
					Key2s();
					Key3s();
					Key4s();
					Key2L();
			 }
		    B10msTimer = 0;
		    //DEBUGP = 1;
		 }
		 
		 if(B100msTimer>480)/////for run bar
		 {
			 B100msTimer=0;			 
			 if(!delay&&runbarenable)
			 {		
					if(result)
					{
						b=sentence3[C];
						if(finish_sentence)
						{
							result=0;
							finish_sentence=2;
							wordptr=0;
						}
						else
							Run_Bar(b);
					}						
					else switch(finish_sentence)
					{
						case 0:b=sentence1[C];Run_Bar(b);break;
						case 1:b=sentence2[C];Run_Bar(b);break;	
						case 2:b=sentence1[C];Run_Bar(b);break;
					}				
					STB3transfer();
					TM1629_WriteBlock(2,0,16,Buf2);					
					TM1629_WriteBlock(3,0,16,Buf3);
					
			 }
			 if(delay)
				delay--;
		 }
		if(BlinkTimer>4500)//for blink
		{
			BlinkTimer=0;
			blink++;
			if(blink>1)
				blink=0;
			
			switch(state)
			{
				case HOME:
					/*if(!count10)
					Display_Bar();
					if(blink)
					{
						u8 i;
						for(i=0;i<15;i+=2)
						{
							Buf3[i]&=0xfe;
						}
					}
				  TM1629_WriteBlock(3,0,16,Buf3);*/
					break;
				case CENTER:
					LED_Show_Zone(cent_co2_,blink);
					break;
				case ZONE:
					LED_Show_Zone(zone_co2_,blink);
					break;
				case BL:
					LED_Show_Zone(blevel_,blink);
					break;
				case MODE:
					LED_Show_Mode(Mode_,blink);
					TM1629_WriteBlock(5,0,16,Buf5);
					break;
				case BUZZ:
					LED_Show_Buz(buz_,blink);
					TM1629_WriteBlock(1,0,16,Buf1);
					break;
				case ALTITUDE:
					LED_Show_Zone(altitude_,blink);
					break;
			}
		}

		Res = Polling_Uart3();
		if(Res < 0x100)
		{
			 
			DO_UART_CMD(Res);
			/* if(Res =='#')// && testSlv1==0)
			 {
			 	   U3rptr = 0;

			 }	
			 else
			 {
			 	  U3rptr &= 0x0f;
			 	  U3rxbuf[U3rptr] = Res;
			 	  U3rptr++;
			 	  
			 #ifndef SUPPORT_88165_TEMP 	    
			 	  #ifndef SUPPORT_PPM 
			 	  if(U3rptr > 6)
			 	  {
			 	  	 u32 x;
		 	  	   if(U3rxbuf[0] =='C' && U3rxbuf[1] =='A' &&U3rxbuf[2] =='U')
			 	  	 {
			 	  	 	  x = (U3rxbuf[3] & 0x0f)* 1000;
			 	  	 	  x += (U3rxbuf[4] & 0x0f)* 100;
			 	  	 	  x += (U3rxbuf[5] & 0x0f)* 10;
			 	  	 	  x += (U3rxbuf[6] & 0x0f);
			 	  	 	  Fill_Second_Block(x); 
			 	  	 	  printf("%d",x);
			 	  	 	  U3rptr = 0;
			 	  	 	  BlockClearCnt1 = 8 * 4;
			 	  	 }	
			 	  	
			 	  }	
			 	  #else  
			 	  if(U3rptr > 7)
			 	  {
			 	  	 u32 x;
		 	  	   if(U3rxbuf[0] =='P' && U3rxbuf[1] =='P' &&U3rxbuf[2] =='M')
			 	  	 {
							  x = (U3rxbuf[3] & 0x0f)* 10000;
			 	  	 	  x += (U3rxbuf[4] & 0x0f)* 1000;
			 	  	 	  x += (U3rxbuf[5] & 0x0f)* 100;
			 	  	 	  x += (U3rxbuf[6] & 0x0f)* 10;
			 	  	 	  x += (U3rxbuf[7] & 0x0f);
							  co2=x;
							  //LED_Show_CO2(x);
							  //LED_Show_Temp(253,0);
							  //LED_Show_RH(74);
			 	  	 	  //Fill_Third_Block(x); 
			 	  	 	  //printf("U3=%d\r\n",x);
			 	  	 	  U3rptr = 0;
			 	  	 	  BlockClearCnt1 = 8 * 4;
			 	  	 }	
			 	  	
			 	  }				 	  
			 	  #endif
			#else
			#if 1
			    if(U3rptr > 6)
			 	  {
			 	  	 u32 x;
		 	  	   if(U3rxbuf[4] =='.' && U3rxbuf[6] =='C') // &&U3rxbuf[2] =='U')
			 	  	 {
			 	  	 	  if(U3rxbuf[0] == '+')
			 	  	 	  {
			 	  	 	  	  x = (U3rxbuf[1] & 0x0f)* 1000;
			 	  	 	      x += (U3rxbuf[2] & 0x0f)* 100;
			 	  	 	      x += (U3rxbuf[3] & 0x0f)* 10;
			 	  	 	      x += (U3rxbuf[5] & 0x0f);
			 	  	 	      Fill_TempP(x); 
			 	  	 	      TempNow = x;
			 	  	 	  }
			 	  	 	  else if(U3rxbuf[0] == '-')
			 	  	 	  {
			 	  	 	  	  x = (U3rxbuf[1] & 0x0f)* 1000;
			 	  	 	      x += (U3rxbuf[2] & 0x0f)* 100;
			 	  	 	      x += (U3rxbuf[3] & 0x0f)* 10;
			 	  	 	      x += (U3rxbuf[5] & 0x0f);
			 	  	 	      Fill_TempN(x);
			 	  	 	      TempNow = 500; 
			 	  	   	}		
			 	  	 	  
			 	  	 	  //printf(" { %d }",TempNow);
			 	  	 	  U3rptr = 0;
			 	  	 	  BlockClearCnt1 = 8 * 4;
			 	  	 }	
			 	  	//printf("%c",x);
			 	  }	 	  
			#endif
			 	  if(0)//Res >0x02 && Res < 0x80)
			 	  {
			 	  	 printf("%c",Res);
			 	  	 usb_printf("%c",Res);
			 	 
			 	  }	
			#endif 	  
			 	 
			 }	*/
		}	

		
    //Polling_ReadCO2();
    
	}
}

