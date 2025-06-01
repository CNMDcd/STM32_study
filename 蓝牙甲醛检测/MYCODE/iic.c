#include "iic.h"

/****************************************
引脚说明
SCL -- PB8
SDA -- PB9

*****************************************/

void Iic_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//打开GPIOB组时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	

	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9|GPIO_Pin_8;		//引脚8
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;				//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_OD;				//开漏输出
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;					//上拉
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;				//速度
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	//空闲状态
	SCL = 1;
	SDA_OUT = 1;
}

//数据引脚模式
void Iic_Sda_Mode(GPIOMode_TypeDef mode)
{

	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;		//引脚8
	GPIO_InitStruct.GPIO_Mode	= mode;							//引脚模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_OD;				//开漏输出
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;					//上拉
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;				//速度
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
}

//启动信号--主机发出
void Iic_Start(void)
{
	//数据线输出模式
	Iic_Sda_Mode(GPIO_Mode_OUT);

	SCL = 1;
	SDA_OUT = 1;

	delay_us(5);
	
	SDA_OUT = 0;
	
	delay_us(5);
	
	SCL = 0;
}

//停止信号--主机发出
void Iic_Stop(void)
{
	//数据线输出模式
	Iic_Sda_Mode(GPIO_Mode_OUT);

	SCL = 0;
	SDA_OUT = 0;

	delay_us(5);
	
	SCL = 1;
	
	delay_us(5);
	
	SDA_OUT = 1;

}






//发送一位数据
void Iic_Send_Ack(u8 ack)
{
	//数据线输出模式
	Iic_Sda_Mode(GPIO_Mode_OUT);

	//一个脉冲发一位数据
	SCL = 0;
	//引脚的变化代码
	if(ack == 1)
	{
		SDA_OUT = 1; // 引脚电平
	}
	if(ack == 0)
	{
		SDA_OUT = 0;// 引脚电平
	}	
	
	delay_us(5);
	
	SCL = 1;
	
	delay_us(5);

	SCL = 0;
	
}

//发送一个字节数据
//先发高位数据
void Iic_Send_Byte(u8 txdata) 
{
	u8 i;
	
	//数据线输出模式
	Iic_Sda_Mode(GPIO_Mode_OUT);	

	//8个脉冲
	SCL = 0;
	
	for(i=0; i<8; i++)
	{
		//引脚变更在SCL为低电平区间
		//逐步从高位判断数据位是1还是0
		if(txdata & (1<<(7-i)))  
		{
			SDA_OUT = 1;
		}
		else
		{
			SDA_OUT = 0;
		}

		delay_us(5);
		
		SCL = 1;
			
		delay_us(5);
			
		SCL = 0;	
	}

}

//主机收数据,收一位数据
u8 Iic_Recv_Ack(void)
{
	u8 ack = 0;
	
	//数据线输入模式
	Iic_Sda_Mode(GPIO_Mode_IN);		

	SCL = 0;
	
	delay_us(5);
	
	SCL = 1;
	
	delay_us(5);
	//SCL为高电平区间判断引脚电平
	if(SDA_IN == 1) //引脚为高电平
	{
		ack = 1; //数据位为1
	}
	else
	{
		ack = 0; //数据位为0
	}
	
	SCL = 0;
	
	return ack;
}

//主机收数据,收一个字节数据
u8 Iic_Recv_Byte(void)
{
	u8 i, rxdata = 0x00;
	
	//数据线输入模式
	Iic_Sda_Mode(GPIO_Mode_IN);			
	
	//8个脉冲
	SCL = 0;
	
	for(i=0; i<8; i++)
	{

		delay_us(5);
		
		SCL = 1;
			
		delay_us(5);
		
		//SCL为高电平区间，8位数据合成一个字节
		//SCL为高电平区间判断引脚电平
		if(SDA_IN == 1) //引脚为高电平
		{
			rxdata |= (1<<(7-i));
		}
	
			
		SCL = 0;	
	}	
	
	return rxdata;
}







//u8 addr:写数据起始地址
void At24c02_Write_Page(u8 addr, u8 *write_buf, u8 len)
{
	u8 ack;

	//启动信号--主机发出
	Iic_Start();
	
	//主机发送设备地址，并对设备执行写操作
	Iic_Send_Byte(AT24C02_WR_ADD); 
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\n");
		return ;
	}
	//STM32做为输出，发送写数据的起始地址
	Iic_Send_Byte(addr); 
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack2 failure\n");
		return ;
	}
	
	//写入数据
	while(len--)
	{
		Iic_Send_Byte(*write_buf); 
		ack = Iic_Recv_Ack();
		if(ack == 1)
		{
			printf("ack3 failure\n");
			return ;
		}		
		//地址指向下一个数据
		write_buf++;
	}
	
	Iic_Stop();
	
//	printf("write finish\r\n");
}

//读数据函数  u8 addr：读数据的起始地址
void At24c02_Read_Data(u8 addr, u8 *read_buff, u8 len)
{
	u8 ack;

	//启动信号--主机发出
	Iic_Start();
	
	//主机发送设备地址，并对设备执行写操作（为了写入读数据的起始地址）
	Iic_Send_Byte(AT24C02_WR_ADD); 
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\n");
		return ;
	}

	//主机发送读数据的起始地址
	Iic_Send_Byte(addr); 
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\n");
		return ;
	}
	
	//启动信号--主机发出
	Iic_Start();
	//主机发送设备地址，并对设备执行读操作
	Iic_Send_Byte(AT24C02_RD_ADD); 
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\n");
		return ;
	}
	
	while(len--)
	{
		*read_buff = Iic_Recv_Byte();
		
		if(len > 0)
			Iic_Send_Ack(0); //有效应答为低电平
		
		read_buff++;
	
	}
	
	Iic_Send_Ack(1); 

	Iic_Stop();
}



void At24c02_Write_Addr(u8 addr, u8 *write_buff, u8 len)
{
	u8 page, len_byte,which_page,bit;
    //判断地址在哪一页
	which_page = addr/8;//第几页
    
    //前面页的空地址
	bit = addr%8;//前面有多少位
	
    //计算有多少页
	page = (len-(8-bit))/8;
    //未满一页字节
	len_byte = (len-(8-bit))%8;
	
	At24c02_Write_Page(addr, write_buff, 8-bit); 
	addr = addr+(8-bit);  //页起始地址
	write_buff = write_buff+(8-bit);
	delay_ms(5);
	while(page--)
	{
		At24c02_Write_Page(addr, write_buff, 8); //写一页
		addr = addr+0x8;  //移动到下一页
		write_buff = write_buff+8;
		delay_ms(5);
	//	printf("1\n");
	}
	//加延时，跨页写数据无延时会失败
	delay_ms(5);

	//写入不满一页的数据
	if(len_byte > 0)
		At24c02_Write_Page(addr, write_buff, len_byte); 

}

