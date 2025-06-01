#include "iic.h"

/****************************************
����˵��
SCL -- PB8
SDA -- PB9

*****************************************/

void Iic_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//��GPIOB��ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	

	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9|GPIO_Pin_8;		//����8
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;				//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_OD;				//��©���
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;					//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;				//�ٶ�
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	//����״̬
	SCL = 1;
	SDA_OUT = 1;
}

//��������ģʽ
void Iic_Sda_Mode(GPIOMode_TypeDef mode)
{

	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;		//����8
	GPIO_InitStruct.GPIO_Mode	= mode;							//����ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_OD;				//��©���
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;					//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;				//�ٶ�
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
}

//�����ź�--��������
void Iic_Start(void)
{
	//���������ģʽ
	Iic_Sda_Mode(GPIO_Mode_OUT);

	SCL = 1;
	SDA_OUT = 1;

	delay_us(5);
	
	SDA_OUT = 0;
	
	delay_us(5);
	
	SCL = 0;
}

//ֹͣ�ź�--��������
void Iic_Stop(void)
{
	//���������ģʽ
	Iic_Sda_Mode(GPIO_Mode_OUT);

	SCL = 0;
	SDA_OUT = 0;

	delay_us(5);
	
	SCL = 1;
	
	delay_us(5);
	
	SDA_OUT = 1;

}






//����һλ����
void Iic_Send_Ack(u8 ack)
{
	//���������ģʽ
	Iic_Sda_Mode(GPIO_Mode_OUT);

	//һ�����巢һλ����
	SCL = 0;
	//���ŵı仯����
	if(ack == 1)
	{
		SDA_OUT = 1; // ���ŵ�ƽ
	}
	if(ack == 0)
	{
		SDA_OUT = 0;// ���ŵ�ƽ
	}	
	
	delay_us(5);
	
	SCL = 1;
	
	delay_us(5);

	SCL = 0;
	
}

//����һ���ֽ�����
//�ȷ���λ����
void Iic_Send_Byte(u8 txdata) 
{
	u8 i;
	
	//���������ģʽ
	Iic_Sda_Mode(GPIO_Mode_OUT);	

	//8������
	SCL = 0;
	
	for(i=0; i<8; i++)
	{
		//���ű����SCLΪ�͵�ƽ����
		//�𲽴Ӹ�λ�ж�����λ��1����0
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

//����������,��һλ����
u8 Iic_Recv_Ack(void)
{
	u8 ack = 0;
	
	//����������ģʽ
	Iic_Sda_Mode(GPIO_Mode_IN);		

	SCL = 0;
	
	delay_us(5);
	
	SCL = 1;
	
	delay_us(5);
	//SCLΪ�ߵ�ƽ�����ж����ŵ�ƽ
	if(SDA_IN == 1) //����Ϊ�ߵ�ƽ
	{
		ack = 1; //����λΪ1
	}
	else
	{
		ack = 0; //����λΪ0
	}
	
	SCL = 0;
	
	return ack;
}

//����������,��һ���ֽ�����
u8 Iic_Recv_Byte(void)
{
	u8 i, rxdata = 0x00;
	
	//����������ģʽ
	Iic_Sda_Mode(GPIO_Mode_IN);			
	
	//8������
	SCL = 0;
	
	for(i=0; i<8; i++)
	{

		delay_us(5);
		
		SCL = 1;
			
		delay_us(5);
		
		//SCLΪ�ߵ�ƽ���䣬8λ���ݺϳ�һ���ֽ�
		//SCLΪ�ߵ�ƽ�����ж����ŵ�ƽ
		if(SDA_IN == 1) //����Ϊ�ߵ�ƽ
		{
			rxdata |= (1<<(7-i));
		}
	
			
		SCL = 0;	
	}	
	
	return rxdata;
}



//u8 addr:д������ʼ��ַ
void At24c02_Write_Page(u8 addr, u8 *write_buf, u8 len)
{
	u8 ack;

	//�����ź�--��������
	Iic_Start();
	
	//���������豸��ַ�������豸ִ��д����
	Iic_Send_Byte(AT24C02_WR_ADD); 
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\n");
		return ;
	}
	//STM32��Ϊ���������д���ݵ���ʼ��ַ
	Iic_Send_Byte(addr); 
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack2 failure\n");
		return ;
	}
	
	//д������
	while(len--)
	{
		Iic_Send_Byte(*write_buf); 
		ack = Iic_Recv_Ack();
		if(ack == 1)
		{
			printf("ack3 failure\n");
			return ;
		}		
		//��ַָ����һ������
		write_buf++;
	}
	
	Iic_Stop();
	
	printf("write finish\r\n");
} 

//�����ݺ���  u8 addr�������ݵ���ʼ��ַ
void At24c02_Read_Data(u8 addr, u8 *read_buff, u8 len)
{
	u8 ack;

	//�����ź�--��������
	Iic_Start();
	
	//���������豸��ַ�������豸ִ��д������Ϊ��д������ݵ���ʼ��ַ��
	Iic_Send_Byte(AT24C02_WR_ADD); 
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\n");
		return ;
	}

	//�������Ͷ����ݵ���ʼ��ַ
	Iic_Send_Byte(addr); 
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\n");
		return ;
	}
	
	//�����ź�--��������
	Iic_Start();
	//���������豸��ַ�������豸ִ�ж�����
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
			Iic_Send_Ack(0); //��ЧӦ��Ϊ�͵�ƽ
		
		read_buff++;
	
	}
	
	Iic_Send_Ack(1); 

	Iic_Stop();
}




