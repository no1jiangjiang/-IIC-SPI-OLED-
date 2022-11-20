/*******************************************/
/*@??????§µ?????????????????          */
/*@????????????????????                */
/*@?·Ú??V1.2                              */
/*******************************************/


#include "AHT20-21_DEMO_V1_3.h" 



void Delay_N10us(uint32_t t)//???????
{
  uint32_t k;

   while(t--)
  {
    for (k = 0; k < 2; k++);//110
  }
}

void SensorDelay_us(uint32_t t)//???????
{
		
	for(t = t-2; t>0; t--)
	{
		Delay_N10us(1);
	}
}

void Delay_4us(void)		//???????
{	
	Delay_N10us(1);
	Delay_N10us(1);
	Delay_N10us(1);
	Delay_N10us(1);
}
void Delay_5us(void)		//???????
{	
	Delay_N10us(1);
	Delay_N10us(1);
	Delay_N10us(1);
	Delay_N10us(1);
	Delay_N10us(1);

}

void Delay_1ms(uint32_t t)		//???????
{
   while(t--)
  {
    SensorDelay_us(1000);//////???1ms
  }
}


//void AHT20_Clock_Init(void)		//???????
//{
//	RCC_APB2PeriphClockCmd(CC_APB2Periph_GPIOB,ENABLE);
//}

void SDA_Pin_Output_High(void)   //??PB7???????? ?? ????????????? PB7???I2C??SDA
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//???????
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,& GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
}

void SDA_Pin_Output_Low(void)  //??P7????????  ???????????
{

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//???????
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,& GPIO_InitStruct);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
}

void SDA_Pin_IN_FLOATING(void)  //SDA?????????????
{

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB,&GPIO_InitStruct);
}

void SCL_Pin_Output_High(void) //SCL?????????P6???I2C??SCL
{
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
}

void SCL_Pin_Output_Low(void) //SCL???????
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
}

void Init_I2C_Sensor_Port(void) //?????I2C???,????????
{	
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//???????
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,& GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);//???????
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//???????
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,& GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);//???????
	
}
void I2C_Start(void)		 //I2C????????START???
{
	SDA_Pin_Output_High();
	SensorDelay_us(8);
	SCL_Pin_Output_High();
	SensorDelay_us(8);
	SDA_Pin_Output_Low();
	SensorDelay_us(8);
	SCL_Pin_Output_Low();
	SensorDelay_us(8);   
}


void AHT20_WR_Byte(uint8_t Byte) //??AHT20§Õ??????
{
	uint8_t Data,N,i;	
	Data=Byte;
	i = 0x80;
	for(N=0;N<8;N++)
	{
		SCL_Pin_Output_Low(); 
		Delay_4us();	
		if(i&Data)
		{
			SDA_Pin_Output_High();
		}
		else
		{
			SDA_Pin_Output_Low();
		}	
			
    SCL_Pin_Output_High();
		Delay_4us();
		Data <<= 1;
		 
	}
	SCL_Pin_Output_Low();
	SensorDelay_us(8);   
	SDA_Pin_IN_FLOATING();
	SensorDelay_us(8);	
}	


uint8_t AHT20_RD_Byte(void)//??AHT20?????????
{
	uint8_t Byte,i,a;
	Byte = 0;
	SCL_Pin_Output_Low();
	SDA_Pin_IN_FLOATING();
	SensorDelay_us(8);	
	for(i=0;i<8;i++)
	{
    SCL_Pin_Output_High();		
		Delay_5us();
		a=0;
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)) a=1;
		Byte = (Byte<<1)|a;
		SCL_Pin_Output_Low();
		Delay_5us();
	}
  SDA_Pin_IN_FLOATING();
	SensorDelay_us(8);	
	return Byte;
}


uint8_t Receive_ACK(void)   //??AHT20????§Ý??ACK
{
	uint16_t CNT;
	CNT = 0;
	SCL_Pin_Output_Low();	
	SDA_Pin_IN_FLOATING();
	SensorDelay_us(8);	
	SCL_Pin_Output_High();	
	SensorDelay_us(8);	
	while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))  && CNT < 100) 
	CNT++;
	if(CNT == 100)
	{
		return 0;
	}
 	SCL_Pin_Output_Low();	
	SensorDelay_us(8);	
	return 1;
}

void Send_ACK(void)		  //???????ACK???
{
	SCL_Pin_Output_Low();	
	SensorDelay_us(8);	
	SDA_Pin_Output_Low();
	SensorDelay_us(8);	
	SCL_Pin_Output_High();	
	SensorDelay_us(8);
	SCL_Pin_Output_Low();	
	SensorDelay_us(8);
	SDA_Pin_IN_FLOATING();
	SensorDelay_us(8);
}

void Send_NOT_ACK(void)	//?????????ACK
{
	SCL_Pin_Output_Low();	
	SensorDelay_us(8);
	SDA_Pin_Output_High();
	SensorDelay_us(8);
	SCL_Pin_Output_High();	
	SensorDelay_us(8);		
	SCL_Pin_Output_Low();	
	SensorDelay_us(8);
    SDA_Pin_Output_Low();
	SensorDelay_us(8);
}

void Stop_I2C(void)	  //???§¿?????
{
	SDA_Pin_Output_Low();
	SensorDelay_us(8);
	SCL_Pin_Output_High();	
	SensorDelay_us(8);
	SDA_Pin_Output_High();
	SensorDelay_us(8);
}

uint8_t AHT20_Read_Status(void)//???AHT20?????????
{

	uint8_t Byte_first;	
	I2C_Start();
	AHT20_WR_Byte(0x71);
	Receive_ACK();
	Byte_first = AHT20_RD_Byte();
	Send_NOT_ACK();
	Stop_I2C();
	return Byte_first;
}

uint8_t AHT20_Read_Cal_Enable(void)  //???cal enable¦Ë????????
{
	uint8_t val = 0;//ret = 0,
  val = AHT20_Read_Status();
	 if((val & 0x68)==0x08)
		 return 1;
   else  return 0;
 }

void AHT20_SendAC(void) //??AHT20????AC????
{

	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xac);//0xAC???????
	Receive_ACK();
	AHT20_WR_Byte(0x33);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();

}

//CRC§µ???????CRC8/MAXIM
//???????X8+X5+X4+1
//Poly??0011 0001  0x31
//??¦Ë?????????? 1000 1100 0x8c
//C???????
uint8_t Calc_CRC8(uint8_t *message,uint8_t Num)
{
        uint8_t i;
        uint8_t byte;
        uint8_t crc=0xFF;
  for(byte=0; byte<Num; byte++)
  {
    crc^=(message[byte]);
    for(i=8;i>0;--i)
    {
      if(crc&0x80) crc=(crc<<1)^0x31;
      else crc=(crc<<1);
    }
  }
        return crc;
}

void AHT20_Read_CTdata(uint32_t *ct) //???CRC§µ?ï…?????AHT20?????????????
{
	volatile uint8_t  Byte_1th=0;
	volatile uint8_t  Byte_2th=0;
	volatile uint8_t  Byte_3th=0;
	volatile uint8_t  Byte_4th=0;
	volatile uint8_t  Byte_5th=0;
	volatile uint8_t  Byte_6th=0;
	 uint32_t RetuData = 0;
	uint16_t cnt = 0;
	AHT20_SendAC();//??AHT10????AC????
	Delay_1ms(80);//???80ms????	
    cnt = 0;
	while(((AHT20_Read_Status()&0x80)==0x80))//?????bit[7]?0?????????????????1????????
	{
		SensorDelay_us(1508);
		if(cnt++>=100)
		{
		 break;
		 }
	}
	I2C_Start();
	AHT20_WR_Byte(0x71);
	Receive_ACK();
	Byte_1th = AHT20_RD_Byte();//?????????????0x98,?????????bit[7]?1?????0x1C??????0x0C??????0x08????????????bit[7]?0
	Send_ACK();
	Byte_2th = AHT20_RD_Byte();//???
	Send_ACK();
	Byte_3th = AHT20_RD_Byte();//???
	Send_ACK();
	Byte_4th = AHT20_RD_Byte();//???/???
	Send_ACK();
	Byte_5th = AHT20_RD_Byte();//???
	Send_ACK();
	Byte_6th = AHT20_RD_Byte();//???
	Send_NOT_ACK();
	Stop_I2C();

	RetuData = (RetuData|Byte_2th)<<8;
	RetuData = (RetuData|Byte_3th)<<8;
	RetuData = (RetuData|Byte_4th);
	RetuData =RetuData >>4;
	ct[0] = RetuData;//???
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;
	RetuData = (RetuData|Byte_5th)<<8;
	RetuData = (RetuData|Byte_6th);
	RetuData = RetuData&0xfffff;
	ct[1] =RetuData; //???

}


void AHT20_Read_CTdata_crc(uint32_t *ct) //CRC§µ??????AHT20?????????????
{
	volatile uint8_t  Byte_1th=0;
	volatile uint8_t  Byte_2th=0;
	volatile uint8_t  Byte_3th=0;
	volatile uint8_t  Byte_4th=0;
	volatile uint8_t  Byte_5th=0;
	volatile uint8_t  Byte_6th=0;
	volatile uint8_t  Byte_7th=0;
	 uint32_t RetuData = 0;
	 uint16_t cnt = 0;
	// uint8_t  CRCDATA=0;
	 uint8_t  CTDATA[6]={0};//????CRC????????
	
	AHT20_SendAC();//??AHT10????AC????
	Delay_1ms(80);//???80ms????	
    cnt = 0;
	while(((AHT20_Read_Status()&0x80)==0x80))//?????bit[7]?0?????????????????1????????
	{
		SensorDelay_us(1508);
		if(cnt++>=100)
		{
		 break;
		}
	}
	
	I2C_Start();

	AHT20_WR_Byte(0x71);
	Receive_ACK();
	CTDATA[0]=Byte_1th = AHT20_RD_Byte();//?????????????0x98,?????????bit[7]?1?????0x1C??????0x0C??????0x08????????????bit[7]?0
	Send_ACK();
	CTDATA[1]=Byte_2th = AHT20_RD_Byte();//???
	Send_ACK();
	CTDATA[2]=Byte_3th = AHT20_RD_Byte();//???
	Send_ACK();
	CTDATA[3]=Byte_4th = AHT20_RD_Byte();//???/???
	Send_ACK();
	CTDATA[4]=Byte_5th = AHT20_RD_Byte();//???
	Send_ACK();
	CTDATA[5]=Byte_6th = AHT20_RD_Byte();//???
	Send_ACK();
	Byte_7th = AHT20_RD_Byte();//CRC????
	Send_NOT_ACK();                           //???: ????????NAK
	Stop_I2C();
	
	if(Calc_CRC8(CTDATA,6)==Byte_7th)
	{
	RetuData = (RetuData|Byte_2th)<<8;
	RetuData = (RetuData|Byte_3th)<<8;
	RetuData = (RetuData|Byte_4th);
	RetuData =RetuData >>4;
	ct[0] = RetuData;//???
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;
	RetuData = (RetuData|Byte_5th)<<8;
	RetuData = (RetuData|Byte_6th);
	RetuData = RetuData&0xfffff;
	ct[1] =RetuData; //???
		
	}
	else
	{
		ct[0]=0x00;
		ct[1]=0x00;//§µ??????????????????????????????
	}//CRC????
}


void AHT20_Init(void)   //?????AHT20
{	
	Init_I2C_Sensor_Port();
	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xa8);//0xA8????NOR??????
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();

	Delay_1ms(10);//???10ms????

	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xbe);//0xBE?????????AHT20????????????0xBE,   AHT10????????????0xE1
	Receive_ACK();
	AHT20_WR_Byte(0x08);//???????bit[3]??1???§µ????
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();
	Delay_1ms(10);//???10ms????
}
void JH_Reset_REG(uint8_t addr)
{
	
	uint8_t Byte_first,Byte_second,Byte_third;
	I2C_Start();
	AHT20_WR_Byte(0x70);//?????0x70
	Receive_ACK();
	AHT20_WR_Byte(addr);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();

	Delay_1ms(5);//???5ms????
	I2C_Start();
	AHT20_WR_Byte(0x71);//
	Receive_ACK();
	Byte_first = AHT20_RD_Byte();
	Send_ACK();
	Byte_second = AHT20_RD_Byte();
	Send_ACK();
	Byte_third = AHT20_RD_Byte();
	Send_NOT_ACK();
	Stop_I2C();
	
    Delay_1ms(10);//???10ms????
	I2C_Start();
	AHT20_WR_Byte(0x70);///
	Receive_ACK();
	AHT20_WR_Byte(0xB0|addr);////?????????
	Receive_ACK();
	AHT20_WR_Byte(Byte_second);
	Receive_ACK();
	AHT20_WR_Byte(Byte_third);
	Receive_ACK();
	Stop_I2C();
	
	Byte_second=0x00;
	Byte_third =0x00;
}

void AHT20_Start_Init(void)
{
	JH_Reset_REG(0x1b);
	JH_Reset_REG(0x1c);
	JH_Reset_REG(0x1e);
}

//int32_t main(void)
//{
//    uint32_t CT_data[2];
//	volatile int  c1,t1;
//	/***********************************************************************************/
//	/**///?????íà???§à?????????????????100~500ms,????500ms
//	/***********************************************************************************/
//	Delay_1ms(500);
//	/***********************************************************************************/
//	/**///????????¦Ç?0x71?????????§Ø?????????0x18,???????0x18,???§Þ?????????
//	/***********************************************************************************/
//	if((AHT20_Read_Status()&0x18)!=0x18)
//	{
//	AHT20_Start_Init(); //???3?????????
//	Delay_1ms(10);
//	}
//	
//	/***********************************************************************************/
//	/**///?????????????????????????????????????while??1????????????????????????????????¦Ï?
//	/***********************************************************************************/
//	while(1)
//	{
//	 AHT20_Read_CTdata(CT_data);       //??????CRC§µ?ï…?????AHT20?????????????    ??????????1S?????
//    //AHT20_Read_CTdata_crc(CT_data);  //crc§µ??????AHT20????????????? 
//	

//	 c1 = CT_data[0]*100*10/1024/1024;  //??????????c1???????10????
//	 t1 = CT_data[1]*200*10/1024/1024-500;//??????????t1???????10????
//	////????????????????????
//	 }

// }	