#include "led.h"
#include "rtc.h"
#include "math.h"
#define R 1
#define G 2
#define Y 3
static const unsigned char BitReverseTable256[] = 
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};
u8 Word_a[]={0x02,0x45,0x45,0x45,0x87,0x00};
u8 Word_b[]={0xf7,0x84,0x44,0x44,0x83,0x00};
u8 Word_c[]={0x83,0x44,0x44,0x44,0x00};
u8 Word_d[]={0x83,0x44,0x44,0x42,0xf7,0x00};
u8 Word_e[]={0x83,0x45,0x45,0x45,0x84,0x00};
u8 Word_f[]={0x40,0xe7,0x50,0x00};
u8 Word_g[]={0x81,0x4a,0x4a,0x49,0xc7,0x00};
u8 Word_h[]={0xf7,0x80,0x40,0x40,0x87,0x00};
u8 Word_i[]={0xd7,0x00};
u8 Word_k[]={0xf7,0x01,0x82,0x44,0x00};
u8 Word_l[]={0xf7,0x00};
u8 Word_m[]={0xc7,0x40,0x81,0x40,0xc7,0x00};
u8 Word_n[]={0xc7,0x40,0x40,0x40,0x87,0x00};
u8 Word_o[]={0x83,0x44,0x44,0x44,0x83,0x00};
u8 Word_p[]={0xcf,0x41,0x42,0x42,0x81,0x00};
u8 Word_q[]={0x81,0x42,0x42,0x41,0xcf,0x00};
u8 Word_r[]={0xc7,0x80,0x40,0x40,0x00};
u8 Word_s[]={0x84,0x45,0x45,0x45,0x42,0x00};
u8 Word_t[]={0x40,0xe3,0x44,0x44,0x00};
u8 Word_u[]={0xc3,0x04,0x04,0x04,0xc7,0x00};
u8 Word_v[]={0xc1,0x02,0x04,0x02,0xc1,0x00};
u8 Word_y[]={0xc1,0x0a,0x0a,0x0a,0xc7,0x00};
u8 Word_z[]={0x44,0x46,0x45,0xc4,0x44,0x00};

u8 Word_A[]={0xc7,0x21,0x11,0x21,0xc7,0x00};
u8 Word_B[]={0xf7,0x94,0x94,0x94,0x63,0x00};
u8 Word_C[]={0xe3,0x14,0x14,0x14,0x22,0x00};
u8 Word_D[]={0xf7,0x14,0x14,0x14,0xe3,0x00};
u8 Word_E[]={0xf7,0x94,0x94,0x94,0x14,0x00};
u8 Word_F[]={0xf7,0x90,0x90,0x90,0x10,0x00};
u8 Word_H[]={0xf7,0x80,0x80,0x80,0xf7,0x00};
u8 Word_M[]={0xf7,0x20,0xc0,0x20,0xf7,0x00};
u8 Word_N[]={0xf7,0x20,0x40,0x80,0xf7,0x00};
u8 Word_O[]={0xe3,0x14,0x14,0x14,0xe3,0x00};
u8 Word_P[]={0xf7,0x90,0x90,0x90,0x60,0x00};
u8 Word_Q[]={0xe3,0x14,0x14,0x14,0xeb,0x00};
u8 Word_R[]={0xf7,0x90,0x90,0x90,0x67,0x00};
u8 Word_S[]={0x62,0x94,0x94,0x94,0x23,0x00};
u8 Word_T[]={0x10,0x10,0xf7,0x10,0x10,0x00};
u8 Word_U[]={0xf3,0x04,0x04,0x04,0xf3,0x00};
u8 Word_V[]={0xf1,0x02,0x04,0x02,0xf1,0x00};
u8 Word_W[]={0xf3,0x04,0x87,0x04,0xf3,0x00};
u8 Word_Z[]={0x16,0x15,0x94,0x54,0x34,0x00};

u8 Word_Com[]={0x0c};//,
u8 Word_Dot[]={0x04};//.
u8 Word_Spa[]={0x00,0x00};
u8 Word_Col[]={0x84};//:

u8 Word_0[]={0xe3,0x15,0x94,0x54,0xe3,0x00};//0
u8 Word_1[]={0x00,0x44,0xe7,0x04,0x00,0x00};//1
u8 Word_2[]={0x84,0x46,0x45,0x84,0x00};//2
u8 Word_3[]={0x22,0x24,0xa4,0x65,0x22,0x00};//3
u8 Word_5[]={0x62,0xa4,0xa4,0x25,0x22,0x00};//5
u8 Word_7[]={0x20,0x20,0xa7,0x60,0x20,0x00};//7

u8 back1[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x18,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
u8 back2[]={0xff,0xff,0xff,0xff,0xff,0xff,0xd8,0xda,0xda,0xda,0x1a,0xff,0xff,0xff,0xff,0xff};
u8 back3[]={0xff,0xff,0xff,0xff,0xff,0xff,0xdb,0x5b,0x5b,0x5b,0x18,0xff,0xff,0xff,0xff,0xff};
u8 back4[]={0xff,0xff,0xff,0xff,0xff,0xff,0x1e,0xfe,0xfe,0xfe,0x18,0xff,0xff,0xff,0xff,0xff};
u8 back5[]={0xff,0xff,0xff,0xff,0xff,0xff,0x1a,0xda,0xda,0xda,0xd8,0xff,0xff,0xff,0xff,0xff};
u8 back6[]={0xff,0xff,0xff,0xff,0xff,0xff,0x18,0xda,0xda,0xda,0xd8,0xff,0xff,0xff,0xff,0xff};
u8 back7[]={0xff,0xff,0xff,0xff,0xff,0xff,0x1f,0xdf,0xdf,0xdf,0x18,0xff,0xff,0xff,0xff,0xff};
u8 back8[]={0xff,0xff,0xff,0xff,0xff,0xff,0x18,0x5b,0x5b,0x5b,0x18,0xff,0xff,0xff,0xff,0xff};

u16  LedScanB1[4] = {0xfc00,0xf300,0xcf00,0x3f00};
u16  LedScanB2[6] = {0xf800,0xf400,0xec00,0xdc00,0xbc00,0x7c00};
u16  Leddg1,debug1;
u8   LedScanB1Cnt;
u8   LedScanD1[10];//,LedScanD2[6];
u16   LEDwritecnt;
extern  u16  B10msTimer,B1msTimer,B100msTimer,B1sTimer,BlinkTimer;
extern u8 C;
extern u8 finish_sentence;
extern u8 option;
extern short Targetup;
extern short Targetdown;

//extern int rhrecord_m[16],rhrecord_h[16],rhrecord_d[16],rhrecord_w[16],maxrh_m,maxrh_h,maxrh_d,maxrh_w,minrh_m,minrh_h,minrh_d,minrh_w;
//extern int temprecord_m[16],temprecord_h[16],temprecord_d[16],temprecord_w[16],maxtemp_m,maxtemp_h,maxtemp_d,maxtemp_w,mintemp_m,mintemp_h,mintemp_d,mintemp_w;
//extern int co2record_m[16],co2record_h[16],co2record_d[16],co2record_w[16],maxco2_m,maxco2_h,maxco2_d,maxco2_w,minco2_m,minco2_h,minco2_d,minco2_w;
extern u16 cent_co2;
extern u16 zone_co2;
extern u8 m,h,d,w;
extern u8 display;
extern u8 timespc;
extern int maxco2_m,maxco2_h,maxco2_d,maxco2_w,minco2_m,minco2_h,minco2_d,minco2_w;
extern short offset;
extern float press;
//static u8 D7segment[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f ,0x77,0x78,0x39,0x5e,0x79,0x71} ;

static u8 D7segment[10][7] = {{0,2,4,6,8,10},{2,4,99,99,99,99,99},{0,2,6,8,12},{0,2,4,6,12},{2,4,10,12,99,99,99},{0,4,6,10,12},{0,4,6,8,10,12},{0,2,4,10},{0,2,4,6,8,10,12},{0,2,4,6,10,12}} ;
static u8 D7segment1[9]={0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char Buf1[16]={0};
unsigned char Buf2[16]={0};
unsigned char Buf3[16]={0};
unsigned char Buf4[16]={0};
unsigned char Buf5[16]={0};

char Bar[17]={6,1,1,2,4,4,1,6,8,8,5,6,5,6,3,2,7};
signed char Bar1[16]={0,1,3,-2,0,-1,-3,1,2,3,0,-1,2,1,0,0};
unsigned char UartCmdBuf[16],UartCmdPtr,UartS8Timeout,UartAzTimeout,IsAkmCmd;
extern u16 co2,temp,rh;
extern u16 light;
u8 wordptr=0;
//初始化TIM2定时器及中断
//每次进入中断服务程序间隔时间为
//((1+arr )/72M)*(1+psc )=((1+1999)/72M)*(1+35999)=1秒

#if 0
void TIM2_Config(u16 arr,u16 psc)
{
    //定时器时间是1s
//RCC_APB2Periph_TIM1
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    //清除中断标志位
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//TIM_ClearFlag(TIM2,TIM_FLAG_Update);//两者作用相同

    //自动装载的周期值0-0xffff，72M/36000=2000
    TIM_TimeBaseStructure.TIM_Period = arr;
    //时钟除数预分频值0-oxffff，下面是36000分频
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    ////普通和高级才有下面两行
    //时钟分割，暂时为0，高级应用才用
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //计数模式，向上下，中央对齐123
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

    //开启中断
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    //开启外设
    TIM_Cmd(TIM2,ENABLE);
}

//初始化中断向量控制器NVIC

void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)//TIM1_IRQHandler(void)
{
    //判断TIM3更新中断是否发生
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
    {
        //必须清楚标志位
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        //状态取反
        Polling_7LED();
        //LEDwritecnt = 0;
        //GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)(!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13)));
    }
}
#endif

/**************************************************************************
函数功能：PWM初始化
入口参数：无
返回  值：无
**************************************************************************/
void DO_CO2_1MS(void )
{
     #ifdef   SUPPORT_S8_CMD
     if(UartS8Timeout)	
        UartS8Timeout--;
     #endif
     if(UartAzTimeout)
        UartAzTimeout--;  	
}
int  GetAkmData(u8 c,u8 *p)
{
	   int x = -9999;
     u8 j,a[6];
     u16 t;
     for(j = 0 ; j < 6 ; j++)
		 {
			   p++;
			   *p&=0x7f;
			   if(*p < 0x3A)     //0-9
		 	   {
		 	      a[j] = *p - 0x30;
		 	   }	
		 	   else              //A-F
		 	   {
		 	      a[j] = *p - 0x37;
		 	   }
		 }  
		 j = (a[0] << 4) | a[1];
		 t = (j << 8 ) | (( a[2] << 4 ) | a[3]);
		 j =  j + ((a[2] << 4) | a[3]) + c - ((a[4] << 4) | a[5]);
		 if(j == 0)    //checksum ok
		 {
		    x = t;
		 }
		 return x;    	          
}	
void DO_UART_CMD(u8 c)
{
	    #if 1
	    if(c == 0x02)
	    {
	    	  UartCmdPtr = 0;
	    	  UartAzTimeout = 40; // 20ms timeout	   
	    	  IsAkmCmd = 1; 
	    	  return;
	    }	
      else if(UartAzTimeout && IsAkmCmd)
      {
      	  UartCmdBuf[UartCmdPtr] = c;
      	  UartCmdPtr++;
	    	  UartCmdPtr &= 0x0f;
	    	  if(UartCmdPtr > 7)
	    	  {
	    	     int t; 
             switch(UartCmdBuf[0])
             {
             	   case 0x43:
             	        t = GetAkmData(0x43,UartCmdBuf);
             	        if(t != -9999)
             	        {
             	           co2 = t+offset;
												 co2=co2/(4.026*0.0001*press+5.78*0.0000001*pow(press,2));
             	           //sprintf(SBuffer,"CO2= %d\r\n",t);
             	           //PrintUartString(SBuffer);
												//printf("%d,",t);
		 	    	          }  
             	        break;
             	   case 0x52:
             	        t = GetAkmData(0x52,UartCmdBuf);
             	        if(t != -9999)
             	        {
             	           rh = t/10;
												//printf("%d,",t);
											}
             	        break;
             	   case 0x53:
             	        t = GetAkmData(0x53,UartCmdBuf);
             	        if(t != -9999)
             	        {
             	           light = t;
             	           //printf("light= %d\r\n",light);             	           
											}
             	        break;
             	   case 0x54:
             	        t = GetAkmData(0x54,UartCmdBuf);
             	        if(t != -9999)
             	        {
             	           temp = t;
												//printf("%d\n",t);
		 	    	          }              	        
             	        break;
             	   default:
											
             	        break;                   	                     	                     	         
             }	
             UartAzTimeout = 0;
             IsAkmCmd = 0;
          }  
          return;
      }
      	
      #endif

	    if(c =='#') //header
	    {
	    	  UartCmdPtr = 0;
	    	  UartAzTimeout = 40; // 20ms timeout
	    }	
	    else if(UartCmdPtr < 16 && UartAzTimeout)
	    {
	    	   UartCmdBuf[UartCmdPtr] = c;
	    	   if(UartCmdPtr == 7)
	    	   {
	    	   	   if(UartCmdBuf[0] == 'P' && UartCmdBuf[1]== 'P' && UartCmdBuf[2] == 'M') //CO2 data ppm
               {
               	    #ifdef  SUPPORT_AZCO2
               	    int HostPPM;
               	    HostPPM =  (UartCmdBuf[3] - '0') * 10000;
	    	   	   	    HostPPM += (UartCmdBuf[4] - '0') * 1000;
	    	   	   	    HostPPM += (UartCmdBuf[5] - '0') * 100;
	    	   	   	    HostPPM += (UartCmdBuf[6] - '0') * 10;
	    	   	   	    HostPPM += (UartCmdBuf[7] - '0') ;
               	    Do_AZ_CMD(HostPPM);
               	    #endif
               }	
               /*else if(UartCmdBuf[0] == 'D' && UartCmdBuf[1]== 'E' && UartCmdBuf[2] == 'F')
               {
               	    SysInfo.b_ISDefMode = 1;
               }
               else if(UartCmdBuf[0] == 'E' && UartCmdBuf[1]== 'N' && UartCmdBuf[2] == 'G')
               {
               	    SysInfo.b_IsEngMode = 1;
               }	*/	
	    	   }	
	    	   UartCmdPtr++;
	    	   UartCmdPtr&=0x0f;

	    }	
	    #ifdef     SUPPORT_S8_CMD
	    if(c == 0xfe) //S8 header
	    {
	    	  UartCmdPtr = 1;
	    	  UartS8Timeout = 40; // 20ms timeout
	    	  UartCmdBuf[0] = 0xfe;
	    }	
	    else if(UartCmdPtr < 16 && UartS8Timeout)
	    {
	    	  UartCmdBuf[UartCmdPtr] = c;
	    	  if(UartCmdPtr == 7)
	    	  {
	    	   	 u16 crcdata,crc;
	    	   	 crcdata = UartCmdBuf[6] | (UartCmdBuf[7] << 8);
	    	   	 crc = GetCRC16(UartCmdBuf,6);
	    	   	 if(crcdata == crc)
	    	   	 {
	    	   	   if(UartCmdBuf[1] == 0x03)
	    	   	   {
	    	   	   	  if(UartCmdBuf[3] == 0x1f)
	    	   	   	  {
	    	   	   	  	 S8_Cmd_Send(READ_ABC);
	    	   	   	  }
	    	   	   	  else
	    	   	   	  {
	    	   	   	  	 S8_Cmd_Send(READ_CAL_CO2);
	    	   	   	  }	
	    	   	   	  	
	    	   	   }
	    	   	   else if(UartCmdBuf[1] == 0x04)	
	    	   	   {
	    	   	   	    if(UartCmdBuf[5] == 0x04)
	    	   	   	    {
	    	   	   	    	 S8_Cmd_Send(READ_STATUS_ALL);
	    	   	   	    }
	    	   	   	    else 
	    	   	   	    {
	    	   	   	    	  if(UartCmdBuf[3] == 0x03) //read co2
	    	   	   	    	  {
	    	   	   	    	  	  S8_Cmd_Send(READ_CO2);
	    	   	   	    	  }
	    	   	   	    	  else
	    	   	   	    	  {
	    	   	   	    	  	  S8_Cmd_Send(READ_STATUS);
	    	   	   	    	  }		
	    	   	   	    }			
	    	   	   }	
	    	   	   else if(UartCmdBuf[1] == 0x06)	
	    	   	   {
	    	   	   	        if(UartCmdBuf[3] == 0x00) //read co2
	    	   	   	    	  {
	    	   	   	    	  	  S8_Cmd_Send(CLEAR_CAL_CO2);
	    	   	   	    	  }
	    	   	   	    	  else if(UartCmdBuf[3] == 0x01) //read co2
	    	   	   	    	  {
	    	   	   	    	  	  S8_Cmd_Send(ENABLE_CAL_CO2);
	    	   	   	    	  }	
	    	   	   	    	  else if(UartCmdBuf[3] == 0x1f) //read co2
	    	   	   	    	  {
	    	   	   	    	  	  if(UartCmdBuf[5] == 0xb4)
	    	   	   	            {
	    	   	   	    	          S8_Cmd_Send(ENABLE_ABC);
	    	   	   	            }
	    	   	   	            else
	    	   	   	            {
	    	   	   	    	  	      S8_Cmd_Send(DISABLE_ABC);
	    	   	   	    	  	  }  
	    	   	   	    	  }		
	    	   	   }	
	    	   	 
	    	   	 }  
	    	  }	
	    	  else if(UartCmdPtr == 6)
	    	  {
	    	  	   u16 crcdata,crc;
	    	   	   crcdata = UartCmdBuf[5] | (UartCmdBuf[6] << 8);
	    	   	   crc = GetCRC16(UartCmdBuf,5);
	    	   	   if(crcdata == crc)
	    	   	   {
	    	  	     if(UartCmdBuf[1] == 0x2b && UartCmdBuf[2] == 0x0e)	
	    	   	     {
	    	   	   	   S8_Cmd_Send(READ_ID);
	    	   	     }
	    	   	   }  	
	    	  }	
	    	  UartCmdPtr++;
	    	  UartCmdPtr&=0x0f;
	    }	
	    #endif
}	
void TIM2_PWM_Init(u16 arr,u16 psc)
{		 			
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //TIM2_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//TIM_OCPolarity_High; 
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  
 
  TIM_CtrlPWMOutputs(TIM2,ENABLE);		
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);   
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	TIM_ARRPreloadConfig(TIM2, ENABLE); 
	TIM_Cmd(TIM2, ENABLE);  
} 

void BuzzerSetup(u8 c)
{
	   if(c)
	   {
         TIM2->CCR3 = 500;
         TIM2->CCR4 = 500;	   	
	   }
	   else
	   {
	   	   TIM2->CCR3 = 0;
         TIM2->CCR4 = 1500;
	   }	
}	


void TIM4_Init(u16 period, u16 prescaler)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//使能TIM4时钟
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				//TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);								//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_TimeBaseStructure.TIM_Period = period-1;				//总的值设置为0xFFFF，设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;			//预分频器
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	TIM_Cmd(TIM4, ENABLE);										//开启定时器
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);					//开启定时器更新中断
}

//定时器4中断服务程序	 
void TIM4_IRQHandler(void)
{
	//static u16 tmr200ms = 0;
	//static u16 tmr1s =0;
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) 			//溢出中断
	{
		 TIM_ClearITPendingBit(TIM4, TIM_IT_Update);				//清除中断标志位
	   //Polling_7LED();
	   B10msTimer++;
		 B1msTimer++;
		 B100msTimer++;
		 B1sTimer++;
		 BlinkTimer++;
			
			
		 //B400msTimer++;
	   //GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)(!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_10)));
		 //GPIO_WriteBit(GPIOC,GPIO_Pin_15,(BitAction)(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_15)));
	}
}


//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	 //使能PB,PE端口时钟
	
	 PWR_BackupAccessCmd( ENABLE );/* 允许修改RTC和后备寄存器*/
   RCC_LSEConfig( RCC_LSE_OFF ); /* 关闭外部低速时钟,PC14+PC15可以用作普通IO*/
   BKP_TamperPinCmd(DISABLE);  /* 关闭入侵检测功能,PC13可以用作普通IO*/
   PWR_BackupAccessCmd(DISABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);   
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2| GPIO_Pin_3 |GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7 |GPIO_Pin_8| GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14| GPIO_Pin_15;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5

 GPIO_ResetBits(GPIOB,GPIO_Pin_0);						 //PB.5 输出高
 GPIO_ResetBits(GPIOB,GPIO_Pin_1);						 //PB.5 输出高
 GPIO_ResetBits(GPIOB,GPIO_Pin_2);						 //PB.5 输出高
 GPIO_ResetBits(GPIOB,GPIO_Pin_3);						 //PB.5 输出高
 GPIO_ResetBits(GPIOB,GPIO_Pin_4);						 //PB.5 输出高
 GPIO_ResetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高
 GPIO_ResetBits(GPIOB,GPIO_Pin_6);						 //PB.5 输出高
 GPIO_ResetBits(GPIOB,GPIO_Pin_7);						 //PB.5 输出高
 GPIO_ResetBits(GPIOB,GPIO_Pin_12);						 //PB.5 输出高
 GPIO_ResetBits(GPIOB,GPIO_Pin_13);						 //PB.5 输出高 
 GPIO_ResetBits(GPIOB,GPIO_Pin_14);						 //PB.5 输出高
 GPIO_ResetBits(GPIOB,GPIO_Pin_15);						 //PB.5 输出高 	

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);				

// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;				 //LED0-->PB.5 端口配置
 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;; 		 //推挽输出
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 //GPIO_Init(GPIOB, &GPIO_InitStructure);		
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);			 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);				
 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);	
 
 //GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5; //|GPIO_Pin_6|GPIO_Pin_7;	    		 //LED1-->PE.5 端口配置, 推挽输出
 
 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 //GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 //GPIO_ResetBits(GPIOA,GPIO_Pin_5); 						 //PE.5 输出高 

 //GPIO_ResetBits(GPIOA,GPIO_Pin_0); 	
 GPIO_ResetBits(GPIOA,GPIO_Pin_2);						 //PB.5 输出高 
 GPIO_ResetBits(GPIOA,GPIO_Pin_8); 	 
 GPIO_ResetBits(GPIOA,GPIO_Pin_11);						 //PB.5 输出高 
 GPIO_ResetBits(GPIOA,GPIO_Pin_12); 	
 GPIO_ResetBits(GPIOA,GPIO_Pin_15);						 //PB.5 输出高 

 GPIO_ResetBits(GPIOC,GPIO_Pin_13);						 //PB.5 输出高 
 GPIO_ResetBits(GPIOC,GPIO_Pin_14);						 //PB.5 输出高 
 GPIO_ResetBits(GPIOC,GPIO_Pin_15);						 //PB.5 输出高 
 //GPIO_ResetBits(GPIOC,GPIO_Pin_5);		
 GPIO_SetBits(GPIOC,GPIO_Pin_6);						 //PB.5 输出高 
 GPIO_SetBits(GPIOC,GPIO_Pin_7);						 //PB.5 输出高 
 GPIO_SetBits(GPIOC,GPIO_Pin_8);						 //PB.5 输出高  
 GPIO_SetBits(GPIOC,GPIO_Pin_9);						 //PB.5 输出高  
 
 GPIO_ResetBits(GPIOA,GPIO_Pin_3);						 //PB.5 输出高 
  
 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_1 ;				 //LED0-->PB.5 端口配置
 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 //GPIO_Init(GPIOA, &GPIO_InitStructure);				 
 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	    		 //LED1-->PE.5 端口配置, 推挽输出
 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
 //GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz 
 
 //POWER = 0;
 LedScanB1Cnt = 0;
 Leddg1 = 0;
 LEDwritecnt = 0x8000;	
}

/*u8 KeyScan(void)
{
	   if(KEY0 == 0)
	   {
	   	  return 1;
	   }
	   else if(KEY1 == 0)
	   {
	   	   return 2;
	   }
	   return 0;
}	*/
 
void LedScan(u16 topcnt)
{
	   GPIO_Write(GPIOB, topcnt);
}	


void	TM1629_OutByte(unsigned char dat)
{
	unsigned char i;
	
	for(i=0;i<8;i++)
	{
		//Delay(3);
		//TM1629_CLK_LOW();
		LED_CLK = 0;
		if((dat & 0x01) == 0x01)
		   LED_DAT = 1;
		    //TM1629_DIO_HIGH();
		else
		    LED_DAT = 0;
		    //TM1629_DIO_LOW();
		//Delay(3);									
		//TM1629_CLK_HIGH();	
		LED_CLK = 1;				
		dat >>= 1;
    }
}
//============数据命令设置===================
void	TM1629_SendCmd(unsigned char cmd,u8 i)
{
	//TM1629_STB_HIGH();								
	//TM1629_CLK_HIGH();
	//TM1629_STB_LOW();								
	//Delay(10);					
	switch(i)
	{
		case 1:
				LED_STB1 = 1;
				LED_CLK = 1;	
	      LED_STB1 = 0;			
				TM1629_OutByte(cmd);
				//Delay(10);
				LED_STB1 = 1;
				//TM1629_STB_HIGH();	
			break;
		case 2:
				LED_STB2 = 1;
				LED_CLK = 1;	
				LED_STB2 = 0;			
				TM1629_OutByte(cmd);
				//Delay(10);
				LED_STB2 = 1;
				//TM1629_STB_HIGH();	
			break;
		case 3:
				LED_STB3 = 1;
				LED_CLK = 1;	
				LED_STB3 = 0;			
				TM1629_OutByte(cmd);
				//Delay(10);
				LED_STB3 = 1;
				//TM1629_STB_HIGH();	
			break;
		case 4:
				LED_STB4 = 1;
				LED_CLK = 1;	
				LED_STB4 = 0;			
				TM1629_OutByte(cmd);
				//Delay(10);
				LED_STB4 = 1;
				//TM1629_STB_HIGH();	
			break;	
    case 5:
				LED_STB5 = 1;
				LED_CLK = 1;	
				LED_STB5 = 0;			
				TM1629_OutByte(cmd);
				//Delay(10);
				LED_STB5 = 1;
				//TM1629_STB_HIGH();	
			break;		
	}
}
//============地址命令设设置===================
void	TM1629_SendAddr(unsigned char addr,u8 i)
{
	switch(i)
	{
		case 1:
			//TM1629_STB_HIGH();										
			//TM1629_CLK_HIGH();
			LED_STB1 = 1;
			LED_CLK = 1;	
			//TM1629_STB_LOW();									
			//Delay(10);
			LED_STB1 = 0;										
			TM1629_OutByte(addr);
			//Delay(10);			
			break;
		case 2:
			//TM1629_STB_HIGH();										
			//TM1629_CLK_HIGH();
			LED_STB2 = 1;
			LED_CLK = 1;	
			//TM1629_STB_LOW();									
			//Delay(10);
			LED_STB2 = 0;										
			TM1629_OutByte(addr);
			//Delay(10);			
			break;
		case 3:
			//TM1629_STB_HIGH();										
			//TM1629_CLK_HIGH();
			LED_STB3 = 1;
			LED_CLK = 1;	
			//TM1629_STB_LOW();									
			//Delay(10);
			LED_STB3 = 0;										
			TM1629_OutByte(addr);
			//Delay(10);			
			break;
		case 4:
			//TM1629_STB_HIGH();										
			//TM1629_CLK_HIGH();
			LED_STB4 = 1;
			LED_CLK = 1;	
			//TM1629_STB_LOW();									
			//Delay(10);
			LED_STB4 = 0;										
			TM1629_OutByte(addr);
			//Delay(10);			
			break;
		case 5:
			//TM1629_STB_HIGH();										
			//TM1629_CLK_HIGH();
			LED_STB5 = 1;
			LED_CLK = 1;	
			//TM1629_STB_LOW();									
			//Delay(10);
			LED_STB5 = 0;										
			TM1629_OutByte(addr);
			//Delay(10);			
			break;
	}
}
//==============================================
void	TM1629_WriteByte( unsigned char addr, unsigned char dat ,u8 i)  
{
	TM1629_SendAddr( addr,i);
	TM1629_OutByte( dat );
	switch(i)
	{
		case 1:
			//TM1629_STB_HIGH();
			LED_STB1 = 1;
			break;
		case 2:
			LED_STB2 = 1;
			break;
		case 3:
			LED_STB3 = 1;
			break;
		case 4:
			LED_STB4 = 1;
			break;
		case 5:
			LED_STB5 = 1;
			break;		
	}
}
//===============================================
void	TM1629_Config(u8 i)
{
	//GPIO_InitTypeDef GPIO_InitStructure;
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//GPIO_Init(GPIOC, &GPIO_InitStructure);
	//TM1629_CLK_HIGH();
	//TM1629_DIO_HIGH();
	//TM1629_STB_HIGH();	
  
  LED_CLK = 1;		
  LED_DAT = 1;
	switch(i)
	{
		case 1:
			//TM1629_STB_HIGH();
			LED_STB1 = 1;
			break;
		case 2:
			LED_STB2 = 1;
			break;
		case 3:
			LED_STB3 = 1;
			break;
		case 4:
			LED_STB4 = 1;
			break;		
		case 5:
			LED_STB5 = 1;
			break;	
	}
	TM1629_SendCmd(0x03,i);										
	TM1629_SendCmd(0x40,i);		
	//TM1629_SendCmd(0x8a,i);
	TM1629_SendCmd(0xc0,i);
}
//=================================================
void	TM1629_WriteBlock( u8 i, unsigned char addr, unsigned char len, unsigned char* buf )
{
	TM1629_Config(i);
	TM1629_SendAddr( addr ,i);
	while ( len -- ) TM1629_OutByte( *buf++ );
	switch(i)
	{
		case 1:
			//TM1629_STB_HIGH();
			LED_STB1 = 1;
			break;
		case 2:
			LED_STB2 = 1;
			break;
		case 3:
			LED_STB3 = 1;
			break;
		case 4:
			LED_STB4 = 1;
			break;		
		case 5:
			LED_STB5 = 1;
			break;
	}
	//TM1629_STB_HIGH();
}
void LED_Clear(void)
{
	u8 i;
	for(i=0;i<16;i++)
	{
		Buf1[i]=0;
		Buf2[i]=0;
		Buf3[i]=0;
		Buf4[i]=0;
		Buf5[i]=0;
	}
}
void STB3transfer()
{
	u8 i;
	for(i=0;i<8;i++)
	{
	  Buf3[i*2]=BitReverseTable256[Buf2[i*2+1]];
	  Buf3[i*2+1]=BitReverseTable256[Buf2[i*2]];
	}

}
void Setnum_STB1(u8 led,u8 num)
{
	u8 i;
	if(led<9)
	{
		for(i=0;i<7;i++)
		{
			Buf1[D7segment[num][i]]|=D7segment1[led];
		}
  }
	else
	{
		for(i=0;i<sizeof(D7segment[num]);i++)
		{
			Buf1[D7segment[num][i]+1]|=D7segment1[led-8];
		}		
	}
	//TM1629_WriteBlock(1,0,16,Buf1);
}
void Setnum_STB4(u8 led,u8 num)
{
	u8 i;
	for(i=0;i<7;i++)
	{
		Buf4[D7segment[num][i]]|=D7segment1[led];
	}
}
void Setnum_STB5(u8 led,u8 num)
{
	u8 i;
	for(i=0;i<7;i++)
	{
		Buf5[D7segment[num][i]]|=D7segment1[led];
	}
}
void LED_Show_CO2(u16 n)
{
	u8 A,B,C,D,E,i;
	A=n/10000;
	B=(n/1000)%10;
	C=(n/100)%10;
	D=(n/10)%10;
	E=n%10;
	
	for(i=0;i<7;i++)
	{
		Buf1[2*i]&=0xe0;
	}
	if(A)
	{
		Setnum_STB1(1,A);
		Setnum_STB1(2,B);
		Setnum_STB1(3,C);
		Setnum_STB1(4,D);
		Setnum_STB1(5,E);
	}
	else if(B)
	{
		Setnum_STB1(2,B);
		Setnum_STB1(3,C);
		Setnum_STB1(4,D);
		Setnum_STB1(5,E);
	}
	else if(C)
	{
		Setnum_STB1(3,C);
		Setnum_STB1(4,D);
		Setnum_STB1(5,E);
	}
	else if(D)
	{
		Setnum_STB1(4,D);
		Setnum_STB1(5,E);
	}
	else
	{
		Setnum_STB1(5,E);
	}
	//TM1629_WriteBlock(1,0,16,Buf1);	
}
void LED_Show_Temp(short t,u8 u)//0=c  1=f
{
	u8 A,B,C,D,i;
	
	if(u)
		t=t*9/5+320;
	A=t/1000;
	B=(t/100)%10;
	C=(t/10)%10;
	D=t%10;
	
	for(i=0;i<7;i++)
	{
		Buf1[2*i]&=0x1f;		
	}
	Buf1[14]&=0xc0;
	

	if(A)
	{
		Buf1[14]|=0x03;
		Setnum_STB1(6,B);
		Setnum_STB1(7,C);
		Setnum_STB1(8,D);
	}
	else if(B)
	{
		Setnum_STB1(6,B);
		Setnum_STB1(7,C);
		Setnum_STB1(8,D);
	}
	else
	{
		Setnum_STB1(7,C);
		Setnum_STB1(8,D);	
	}
	Buf1[14]|=0x40;
	if(u)
	{
		Buf1[15]|=0x01;
		Buf1[14]&=~0x80;
	}
	else
	{
		Buf1[15]&=~0x01;
		Buf1[14]|=0x80;
  }
}
void LED_Show_RH(u8 r)
{
	u8 A,B,i;
	
	A=r/10;
	B=r%10;
	for(i=1;i<15;i+=2)
	{
		Buf1[i]&=0xfc;
	}
	if(A)
	{
		Setnum_STB1(9,A);
		Setnum_STB1(10,B);	
	}
	Buf1[15]|=0x02;
}
void LED_Show_MAX(short max,short min)
{
	u8 A,B,C,D,E;
	u8 a,b,c,d,e,i;
	for(i=0;i<14;i+=2)
	{
		Buf5[i]&=0x60;
		Buf4[i]&=0x60;
	}
	A=max/10000;
	B=(max/1000)%10;
	C=(max/100)%10;
	D=(max/10)%10;
	E=max%10;
	
	a=min/10000;
	b=(min/1000)%10;
	c=(min/100)%10;
	d=(min/10)%10;
	e=min%10;	
	
	if(A)
	{
		Setnum_STB5(1,A);
		Setnum_STB5(2,B);
		Setnum_STB5(3,C);
		Setnum_STB5(4,D);
		Setnum_STB5(5,E);
	}
	else if(B)
	{
		Setnum_STB5(2,B);
		Setnum_STB5(3,C);
		Setnum_STB5(4,D);
		Setnum_STB5(5,E);
	}
	else if(C)
	{
		Setnum_STB5(3,C);
		Setnum_STB5(4,D);
		Setnum_STB5(5,E);
	}
	else if(D)
	{
		Setnum_STB5(4,D);
		Setnum_STB5(5,E);
	}
	else
	{
		Setnum_STB5(5,E);
	}
	
	if(a)
	{
		Setnum_STB4(1,a);
		Setnum_STB4(2,b);
		Setnum_STB4(3,c);
		Setnum_STB4(4,d);
		Setnum_STB4(5,e);
	}
	else if(b)
	{
		Setnum_STB4(2,b);
		Setnum_STB4(3,c);
		Setnum_STB4(4,d);
		Setnum_STB4(5,e);
	}
	else if(c)
	{
		Setnum_STB4(3,c);
		Setnum_STB4(4,d);
		Setnum_STB4(5,e);
	}
	else if(d)
	{
		Setnum_STB4(4,d);
		Setnum_STB4(5,e);
	}
	else
	{
		Setnum_STB4(5,e);
	}
	Buf4[14]|=0x01;
	Buf5[14]|=0x01;
}
void LED_Show_Zone(short max,u8 b)
{
	u8 i,A,B,C,D,E;
	for(i=0;i<14;i+=2)
	{
		Buf5[i]&=0x20;
	}
	A=max/10000;
	B=(max/1000)%10;
	C=(max/100)%10;
	D=(max/10)%10;
	E=max%10;
	if(b)
	{
	if(A)
	{
		Setnum_STB5(1,A);
		Setnum_STB5(2,B);
		Setnum_STB5(3,C);
		Setnum_STB5(4,D);
		Setnum_STB5(5,E);
	}
	else if(B)
	{
		Setnum_STB5(2,B);
		Setnum_STB5(3,C);
		Setnum_STB5(4,D);
		Setnum_STB5(5,E);
	}
	else if(C)
	{
		Setnum_STB5(3,C);
		Setnum_STB5(4,D);
		Setnum_STB5(5,E);
	}
	else if(D)
	{
		Setnum_STB5(4,D);
		Setnum_STB5(5,E);
	}
	else
	{
		Setnum_STB5(5,E);
	}
  }
	TM1629_WriteBlock(5,0,16,Buf5);	
}
void Clear_Zone(void)
{
	u8 i;
	for(i=0;i<13;i+=2)
	{
		Buf5[i]=0;
	}	
	Buf5[14]&=0x3e;	
	TM1629_WriteBlock(5,0,16,Buf5);	
}
void LED_Show_Bar_Human(signed char Bar[16],int record[16],u8 t)
{
	signed char i,j;
	for(i=0;i<16;i++)
	{
		Buf2[i]=0;
		Buf3[i]=0;
	}
	
	for(i=15;i>=t;i--)
	{
		if(i<8)
		{
			for(j=0;j<Bar[i];j++)
			{
				Buf2[14-2*j]|=D7segment1[i+1];
			}
		}
		else
		{
		  for(j=0;j<Bar[i];j++)
			{
				Buf2[14-2*j+1]|=D7segment1[i-7];
			}
		}
	}
	Buf2[14]=Buf2[15]=0xff;
	for(i=0;i<16;i+=2)
	{
		Buf2[i]|=0x01;
	}
	for(i=1;i<Targetdown;i++)
	{
		Buf2[16-2*i]&=0xfe;
	}
	
	for(i=0;i<8-Targetup;i++)
	{
		Buf2[2*i]&=0xfe;
	}
	STB3transfer();
	for(i=0;i<16;i++)
	{
		if(i%2)
			Buf2[i]=0;
		else
			Buf2[i]&=0x01;
	}
	//if(badline==goodline)

	/*if(goodline)
	{
		if(goodline<9)
			for(i=0;i<goodline;i++)
			{
				Buf2[14-2*i]=0;
				Buf2[15-2*i]=0;
			}
		else
			for(i=0;i<16;i++)
			{
				Buf2[i]=0;
				Buf2[i]=0;
			}
	}
	if(badline<9)
	{
		if(badline>0)
			for(i=0;i<18-2*badline;i++)
			{
				Buf3[i]=0;
			}
		else
			for(i=0;i<16;i++)
			{
				Buf3[i]=0;
				Buf3[i]=0;
			}
	}*/
	/*for(i=15;i>=t;i--)
	{
		if(record[i]<cent_co2)
		{
			if(i<8)
			{
				Buf2[16-2*Bar[i]]&=~D7segment1[i+1];
				Buf3[16-2*Bar[i]+1]|=D7segment1[8-i];
			}
			else
			{
				Buf2[16-2*Bar[i]+1]&=~D7segment1[i-7];
				Buf3[16-2*Bar[i]]|=D7segment1[16-i];
			}
		}
		else if(record[i]<cent_co2+zone_co2)
		{
			if(i<8)
			{
				Buf2[16-2*Bar[i]]|=D7segment1[i+1];
				Buf3[16-2*Bar[i]+1]|=D7segment1[8-i];
			}
			else
			{
				Buf2[16-2*Bar[i]+1]|=D7segment1[i-7];
				Buf3[16-2*Bar[i]]|=D7segment1[16-i];
			}
		}
		else
		{
			if(i<8)
			{
				Buf2[16-2*Bar[i]]|=D7segment1[i+1];
				Buf3[16-2*Bar[i]+1]&=~D7segment1[8-i];
			}
			else
			{
				Buf2[16-2*Bar[i]+1]|=D7segment1[i-7];
				Buf3[16-2*Bar[i]]&=~D7segment1[16-i];
			}
		}
	}*/
}
void LED_Show_Bar_Plant(signed char Bar[16],u8 t)
{
	signed char i;
	//u8 j;
	for(i=0;i<16;i++)
	{
		Buf2[i]=0;
		Buf3[i]=0;
	}
	/*for(i=15;i>=t;i--)
	{
		
		if(i<8)
		{
			Buf2[16-2*centline]|=D7segment1[i+1];
			if(Bar[i]>0)
			{
				for(j=0;j<Bar[i];j++)
				{
					Buf2[16-2*(centline+j+1)]|=D7segment1[i+1];
				}
			}
			else
			{
				for(j=0;j<Bar[i]*(-1)+1;j++)
				{
					Buf2[16-2*(centline-j-1)]|=D7segment1[i+1];
				}
			}
		}
		else
		{
			Buf2[16-2*centline+1]|=D7segment1[i-7];
			if(Bar[i]>0)
			{
				for(j=0;j<Bar[i];j++)
				{
					Buf2[16-2*(centline+j+1)+1]|=D7segment1[i-7];
				}
			}
			else
			{
				for(j=0;j<Bar[i]*(-1)+1;j++)
				{
					Buf2[16-2*(centline-j-1)+1]|=D7segment1[i-7];
				}
			}
		}
	}
	STB3transfer();*/
	
	for(i=15;i>=t;i--)
	{
		if(i<8)
		{
			Buf2[8]|=D7segment1[i+1];
			switch(Bar[i])
			{
				case 0:
					break;
				case 1:
					Buf2[6]|=D7segment1[i+1];
					break;
				case 2:
					Buf2[6]|=D7segment1[i+1];
					Buf2[4]|=D7segment1[i+1];
					break;
				case 3:
					Buf2[6]|=D7segment1[i+1];
					Buf2[4]|=D7segment1[i+1];
				  Buf2[2]|=D7segment1[i+1];
					break;
				case -1:
					Buf2[10]|=D7segment1[i+1];
					break;
				case -2:
					Buf2[10]|=D7segment1[i+1];
					Buf2[12]|=D7segment1[i+1];
					break;
				case -3:
					Buf2[10]|=D7segment1[i+1];
					Buf2[12]|=D7segment1[i+1];
				  Buf2[14]|=D7segment1[i+1];
					break;
			}
		}
		else
		{
			Buf2[9]|=D7segment1[i-7];
			switch(Bar[i])
			{
				case 0:
					break;
				case 1:
					Buf2[7]|=D7segment1[i-7];
					break;
				case 2:
					Buf2[7]|=D7segment1[i-7];
					Buf2[5]|=D7segment1[i-7];
					break;
				case 3:
					Buf2[7]|=D7segment1[i-7];
					Buf2[5]|=D7segment1[i-7];
				  Buf2[3]|=D7segment1[i-7];
					break;
				case -1:
					Buf2[11]|=D7segment1[i-7];
					break;
				case -2:
					Buf2[11]|=D7segment1[i-7];
					Buf2[13]|=D7segment1[i-7];
					break;
				case -3:
					Buf2[11]|=D7segment1[i-7];
					Buf2[13]|=D7segment1[i-7];
				  Buf2[15]|=D7segment1[i-7];
					break;
			}
		}
	}
	STB3transfer();
	for(i=6;i<12;i++)
	{
		Buf2[i]=0;
	}
	Buf3[2]=Buf3[3]=Buf3[14]=Buf3[15]=0;
}
void LED_Show_Bar_All(void)
{
	u8 i;
	for(i=0;i<16;i++)
	{
		Buf2[i]=Buf3[i]=0xff;
	}
	TM1629_WriteBlock(2,0,16,Buf2);
	TM1629_WriteBlock(3,0,16,Buf3);
}
void Update_Bar_R2L(u8 a)
{
	u8 i;
	for(i=0;i<16;i++)
	{
		if(i%2==0)
		{
			Buf2[i]=Buf2[i]>>1;
			Buf2[i]|=((Buf2[i+1])<<7);
		}
		else
		{
			Buf2[i]=Buf2[i]>>1;
		}
	}
	for(i=0;i<4;i++)
	{
		if(a&0x1)
		{
			Buf2[9+2*i]|=D7segment1[8];
		}
		else
		{
			Buf2[9+2*i]&=~D7segment1[8];
		}
		a=a>>1;
  }
	for(i=0;i<4;i++)
	{
		if(a&0x1)
		{
			Buf2[1+2*i]|=D7segment1[8];
		}
		else
		{
			Buf2[1+2*i]&=~D7segment1[8];
		}
		a=a>>1;
  }
	for(i=0;i<16;i+=2)
	{
		Buf2[i]&=0xfe;
	}
	//wordptr++;
	//TM1629_WriteBlock(2,0,16,Buf2);
}
void LED_Show_Page(u8 p)
{
	u8 i;
	for(i=0;i<16;i++)
	{
		Buf4[i]=0;
	}
	Buf4[0]|=D7segment1[3];
	Buf4[2]|=D7segment1[3];
	Buf4[8]|=D7segment1[3];
	Buf4[10]|=D7segment1[3];
	Buf4[12]|=D7segment1[3];
	
	Setnum_STB4(4,p/10);
	if(p%10)
	{
		Setnum_STB4(5,p%10);
		Buf4[14]|=0x10;
	}
}
void LED_Show_Bknum(u8 l)
{
	switch(l)
	{
		case 1:
			Update_Bar_R2L(back1[wordptr]);
			wordptr++;
			break;
		case 2:
			Update_Bar_R2L(back2[wordptr]);
			wordptr++;
			break;
		case 3:
			Update_Bar_R2L(back3[wordptr]);
			wordptr++;
			break;
		case 4:
			Update_Bar_R2L(back4[wordptr]);
			wordptr++;
			break;
		case 5:
			Update_Bar_R2L(back5[wordptr]);
			wordptr++;
			break;
		case 6:
			Update_Bar_R2L(back6[wordptr]);
			wordptr++;
			break;
		case 7:
			Update_Bar_R2L(back7[wordptr]);
			wordptr++;
			break;
		case 8:
			Update_Bar_R2L(back8[wordptr]);
			wordptr++;
			break;
	}
}
void LED_Show_CF(u8 unit)
{
	u8 i;
	for(i=0;i<16;i++)
	{
		Buf2[i]=0;
	}
	if(unit==0)
	{
		Buf2[0]=0x80;
		Buf2[1]=0x38;
		Buf2[2]=0x40;
		Buf2[3]=0x45;
		Buf2[4]=0x80;
		Buf2[5]=0x04;	
		Buf2[7]=0x04;
		Buf2[9]=0x04;
		Buf2[11]=0x44;
		Buf2[13]=0x38;
	}
	else
	{
		Buf2[0]=0x80;
		Buf2[1]=0x7c;
		Buf2[2]=0x40;
		Buf2[3]=0x05;
		Buf2[4]=0x80;
		Buf2[5]=0x04;	
		Buf2[7]=0x3c;
		Buf2[9]=0x04;
		Buf2[11]=0x04;
		Buf2[13]=0x04;
	}
}
void LED_Show_Timespc(u8 t)
{
	switch(t)
	{
		case 0:
			Buf4[14]|=0x20;
			Buf4[12]|=0x20;
		
	  	Buf4[0]&=~0x20;
			Buf4[2]&=~0x20;
			break;
		case 1:
			Buf4[8]|=0x20;
			Buf4[10]|=0x20;
		
	  	Buf4[14]&=~0x20;
			Buf4[12]&=~0x20;
			break;
		case 2:
			Buf4[4]|=0x20;
			Buf4[6]|=0x20;
		
	  	Buf4[8]&=~0x20;
			Buf4[10]&=~0x20;
			break;
		case 3:
			Buf4[0]|=0x20;
			Buf4[2]|=0x20;
		
	  	Buf4[4]&=~0x20;
			Buf4[6]&=~0x20;
			break;
	}
	TM1629_WriteBlock(4,0,16,Buf4);
}
void LED_Show_Display(u8 dis)//0:Center  1:MAX
{
	if(dis)
	{
		Buf5[4]|=0x20;
		Buf5[6]|=0x20;
		
		Buf4[4]|=0x40;
		Buf4[8]|=0x40;
		
		Buf5[0]&=~0x20;
		Buf5[2]&=~0x20;
		
		Buf4[0]&=~0x40;
		Buf4[10]&=~0x40;
		
		switch(timespc)
		{
			case 0:
				if(m<16)
						LED_Show_MAX(maxco2_m,minco2_m);
				else
						LED_Show_Dash();
				break;
			case 1:
				if(h<16)
						LED_Show_MAX(maxco2_h,minco2_h);
				else
						LED_Show_Dash();
				break;
			case 2:
				if(d<16)
						LED_Show_MAX(maxco2_d,minco2_d);
				else
						LED_Show_Dash();
				break;
			case 3:
				if(w<16)
						LED_Show_MAX(maxco2_w,minco2_w);
				else
						LED_Show_Dash();
				break;
		}
	}
	else
	{
		Buf5[4]&=~0x20;
		Buf5[6]&=~0x20;
		
		Buf4[4]&=~0x40;
		Buf4[8]&=~0x40;
		
		Buf5[0]|=0x20;
		Buf5[2]|=0x20;
		
		Buf4[0]|=0x40;
		Buf4[10]|=0x40;
		
		LED_Show_MAX(cent_co2,zone_co2);
	}
	TM1629_WriteBlock(5,0,16,Buf5);
	TM1629_WriteBlock(4,0,16,Buf4);
}
void LED_Show_Control(u8 c,u8 t,u8 r)
{
	if(c)
		Buf1[5]|=0x80;
	else
		Buf1[5]&=~0x80;
	if(t)
		Buf1[9]|=0x80;
	else
		Buf1[9]&=~0x80;
	if(r)
		Buf1[13]|=0x80;
	else
		Buf1[13]&=~0x80;
	TM1629_WriteBlock(1,0,16,Buf1);
}
void LED_Show_Time(void)
{
	u8 i;
	u8 A,B,C,D;
	for(i=0;i<15;i+=2)
	{
		Buf4[i]&=0x60;
	}
	A=calendar.hour/10;
	B=calendar.hour%10;
	
	C=calendar.min/10;
	D=calendar.min%10;
	
	Setnum_STB4(1,A);
	Setnum_STB4(2,B);

	Setnum_STB4(4,C);
	Setnum_STB4(5,D);
	
}
void LED_Show_Dash(void)
{
	u8 i;
	for(i=0;i<14;i+=2)
	{
		Buf4[i]&=0x60;
		Buf5[i]&=0x20;
	}
	Buf4[12]|=0x1f;
	Buf5[12]|=0x1f;
}
void LED_Show_Half(u8 b)
{
	u8 i;
	for(i=0;i<16;i++)
	{
		Buf3[i]=0xff;
	}
	if(b)
	{
		for(i=0;i<16;i++)
		{
			Buf3[i]&=0x0f;
		}
	}
}
void LED_Show_Mode(u8 m,u8 b)
{
	if(b)
	{
		if(m)
		{
			Buf5[14]|=0x02;
			Buf5[14]&=~0x10;
		}
		else
		{
			Buf5[14]&=~0x02;
			Buf5[14]|=0x10;
		}
	}
	else
	{
		Buf5[14]&=~0x12;
	}
}
void LED_Show_Buz(u8 bu,u8 b)
{
	if(b)
	{
		if(bu)
		{
			Buf1[1]|=0x04;
			Buf1[3]|=0x04;
			
			Buf1[5]&=0x03;
			Buf1[7]&=0x03;
		}
		else
		{
			Buf1[5]|=0x04;
			Buf1[7]|=0x04;
			
			Buf1[1]&=0x03;
			Buf1[3]&=0x03;
		}
	}
	else
	{
		Buf1[5]&=0x03;
		Buf1[7]&=0x03;
			
		Buf1[1]&=0x03;
		Buf1[3]&=0x03;
	}
}
void LED_Show_Relay(u8 r)
{
	if(r)
		Buf5[14]|=0x04;
	else
		Buf5[14]&=~0x04;
}
void LED_Show_Light(u8 l)
{
	if(l)
		Buf5[14]|=0x08;
	else
		Buf5[14]&=~0x08;
}
void Run_Bar(char b)
{
	switch(b)
	{
		case 'a':
			Update_Bar_R2L(Word_a[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_a))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'b':
			Update_Bar_R2L(Word_b[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_b))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'c':
			Update_Bar_R2L(Word_c[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_c))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'd':
			Update_Bar_R2L(Word_d[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_d))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'e':
			Update_Bar_R2L(Word_e[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_e))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'f':
			Update_Bar_R2L(Word_f[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_f))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'g':
			Update_Bar_R2L(Word_g[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_g))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'h':
			Update_Bar_R2L(Word_h[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_h))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'i':
			Update_Bar_R2L(Word_i[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_i))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'k':
			Update_Bar_R2L(Word_k[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_k))
			{
				wordptr=0;
				C++;
			}
			break;	
		case 'l':
			Update_Bar_R2L(Word_l[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_l))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'm':
			Update_Bar_R2L(Word_m[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_m))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'n':
			Update_Bar_R2L(Word_n[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_n))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'o':
			Update_Bar_R2L(Word_o[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_o))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'p':
			Update_Bar_R2L(Word_p[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_p))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'q':
			Update_Bar_R2L(Word_q[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_q))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'r':
			Update_Bar_R2L(Word_r[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_r))
			{
				wordptr=0;
				C++;
			}
			break;
		case 's':
			Update_Bar_R2L(Word_s[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_s))
			{
				wordptr=0;
				C++;
			}
			break;
		case 't':
			Update_Bar_R2L(Word_t[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_t))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'u':
			Update_Bar_R2L(Word_u[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_u))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'v':
			Update_Bar_R2L(Word_v[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_v))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'y':
			Update_Bar_R2L(Word_y[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_y))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'z':
			Update_Bar_R2L(Word_z[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_z))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'A':
			Update_Bar_R2L(Word_A[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_A))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'B':
			Update_Bar_R2L(Word_B[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_B))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'C':
			Update_Bar_R2L(Word_C[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_C))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'D':
			Update_Bar_R2L(Word_D[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_D))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'E':
			Update_Bar_R2L(Word_E[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_E))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'F':
			Update_Bar_R2L(Word_F[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_F))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'H':
			Update_Bar_R2L(Word_H[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_H))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'M':
			Update_Bar_R2L(Word_M[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_M))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'N':
			Update_Bar_R2L(Word_N[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_N))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'O':
			Update_Bar_R2L(Word_O[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_O))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'P':
			Update_Bar_R2L(Word_P[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_P))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'Q':
			Update_Bar_R2L(Word_Q[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_Q))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'R':
			Update_Bar_R2L(Word_R[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_R))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'S':
			Update_Bar_R2L(Word_S[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_S))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'T':
			Update_Bar_R2L(Word_T[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_T))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'U':
			Update_Bar_R2L(Word_U[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_U))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'V':
			Update_Bar_R2L(Word_V[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_V))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'W':
			Update_Bar_R2L(Word_W[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_W))
			{
				wordptr=0;
				C++;
			}
			break;
		case 'Z':
			Update_Bar_R2L(Word_Z[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_Z))
			{
				wordptr=0;
				C++;
			}
			break;
		case ',':
			Update_Bar_R2L(Word_Com[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_Com))
			{
				wordptr=0;
				C++;
			}
			break;
		case '.':
			Update_Bar_R2L(Word_Dot[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_Dot))
			{
				wordptr=0;
				C++;
			}
			break;
		case ':':
			Update_Bar_R2L(Word_Col[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_Col))
			{
				wordptr=0;
				C++;
			}
			break;
		case ' ':
			Update_Bar_R2L(Word_Spa[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_Spa))
			{
				wordptr=0;
				C++;
			}
			break;
		case '0':
			Update_Bar_R2L(Word_0[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_0))
			{
				wordptr=0;
				C++;
			}
			break;
		case '1':
			Update_Bar_R2L(Word_1[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_1))
			{
				wordptr=0;
				C++;
			}
			break;
		case '2':
			Update_Bar_R2L(Word_2[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_2))
			{
				wordptr=0;
				C++;
			}
			break;
		case '3':
			Update_Bar_R2L(Word_3[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_3))
			{
				wordptr=0;
				C++;
			}
			break;
		case '5':
			Update_Bar_R2L(Word_5[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_5))
			{
				wordptr=0;
				C++;
			}
			break;
		case '7':
			Update_Bar_R2L(Word_7[wordptr]);
			wordptr++;
			if(wordptr>=sizeof(Word_7))
			{
				wordptr=0;
				C++;
			}
			break;			
		case '\0':
			C=0;
			if(finish_sentence<2)
				finish_sentence++;
			break;
	}
}