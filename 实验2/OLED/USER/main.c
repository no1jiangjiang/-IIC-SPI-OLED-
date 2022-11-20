//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103RCT6,����ԭ��MiniSTM32������,��Ƶ72MHZ������12MHZ
//QDtech-OLEDҺ������ for STM32
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/8/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
// OLEDģ��               STM32��Ƭ��
//   VCC         ��       DC 5V/3.3V      //OLED����Դ��
//   GND         ��          GND          //OLED����Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ4����SPI
// OLEDģ��               STM32��Ƭ��
//   D1          ��          PB15        //OLED��SPIд�ź�
//=======================================Һ���������߽���==========================================//
// OLEDģ��               STM32��Ƭ��
//   CS          ��          PB11        //OLED��Ƭѡ�����ź�
//   RES         ��          PB12        //OLED����λ�����ź�
//   DC          ��          PB10        //OLED������/����ѡ������ź�
//   D0          ��          PB13        //OLED��SPIʱ���ź�
//=========================================����������=========================================//
//��ģ�鲻���������ܣ����Բ���Ҫ����������
****************************************************************************************************/	
/***************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
****************************************************************************************************/	
/*#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "gui.h"
#include "test.h"
int main(void)
{	
	delay_init();	    	       //��ʱ������ʼ��	  
	NVIC_Configuration(); 	   //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	
	OLED_Init();			         //��ʼ��OLED  
	OLED_Clear(0);             //������ȫ�ڣ�
	while(1) 
	{	
		TEST_MainPage();
		
		
	}
}
*/


#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "gui.h"
#include "test.h"
#include "AHT20-21_DEMO_V1_3.h" 
void TEST_MainPage1(int c1,int t1)
{	
		
	GUI_ShowCHinese(30,0,16,"��н����",1);
    GUI_ShowCHinese(5,30,16,"ʪ��",1);
    GUI_ShowCHinese(5,45,16,"�¶�",1);
	GUI_ShowNum(35,30,c1/10,4,16,1);
	GUI_ShowCHinese(80,30,16,"��",1);
	GUI_ShowNum(35,45,t1/10,4,16,1);
	GUI_ShowCHinese(80,45,16,"��",1);
	Delay_1ms(1000);
}
volatile int  c1,t1;
uint32_t CT_data[2]={0,0};
u8 temp[10];  
u8 hum[10];
int main(void)
{	
	delay_init();	    	       //��ʱ������ʼ��	
	OLED_Init();			         //��ʼ��OLED  
	OLED_Clear(0);             //������ȫ�ڣ�
	
	/***********************************************************************************/
	/**///�ٸ��ϵ磬��ƷоƬ�ڲ�������Ҫʱ�䣬��ʱ100~500ms,����500ms
	/***********************************************************************************/
	AHT20_Init();
	Delay_1ms(500);
	/***********************************************************************************/
	/**///���ϵ��һ�η�0x71��ȡ״̬�֣��ж�״̬���Ƿ�Ϊ0x18,�������0x18,���мĴ�����ʼ��
	/***********************************************************************************/
  
	if((AHT20_Read_Status()&0x18)!=0x18)
	{
		AHT20_Start_Init(); //���³�ʼ���Ĵ���
		Delay_1ms(10);
	}
	
	  
	//NVIC_Configuration(); 	   //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	
   OLED_WR_Byte(0x2E,OLED_CMD); //�رչ���

	OLED_WR_Byte(0x27,OLED_CMD); //ˮƽ��������ҹ��� 26/27

	OLED_WR_Byte(0x00,OLED_CMD); //�����ֽ�

	OLED_WR_Byte(0x00,OLED_CMD); //��ʼҳ 0

	OLED_WR_Byte(0x07,OLED_CMD); //����ʱ����

	OLED_WR_Byte(0x09,OLED_CMD); //��ֹҳ 2

	OLED_WR_Byte(0x00,OLED_CMD); //�����ֽ�

	OLED_WR_Byte(0xFF,OLED_CMD); //�����ֽ�
	while(1) 
	{		
		//AHT20_Read_CTdata(CT_data);       //������CRCУ�飬ֱ�Ӷ�ȡAHT20���¶Ⱥ�ʪ������    �Ƽ�ÿ������1S��һ��
    AHT20_Read_CTdata_crc(CT_data);  //crcУ��󣬶�ȡAHT20���¶Ⱥ�ʪ������ 
	  c1 = CT_data[0]*100*10/1024/1024;  //����õ�ʪ��ֵc1���Ŵ���10����
	  t1 = CT_data[1]*200*10/1024/1024-500;//����õ��¶�ֵt1���Ŵ���10����	
		TEST_MainPage1(c1,t1);         //������ʾ
		OLED_WR_Byte(0x2F,OLED_CMD); //��������
	  Delay_1ms(2000);

	}
}


