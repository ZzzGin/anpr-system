#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "sram.h"   
#include "malloc.h" 
#include "usmart.h"  
#include "sdio_sdcard.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "piclib.h"	
#include "string.h"	
#include "math.h"	
#include "dcmi.h"	
#include "ov5640.h"	
#include "beep.h"	
#include "timer.h" 
//ALIENTEK ̽����STM32F407������
//OV5640�����ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

u8 bmp_request=0;						//bmp��������:0,��bmp��������;1,��bmp��������,��Ҫ��֡�ж�����,�ر�DCMI�ӿ�.
u8 ovx_mode=0;							//bit0:0,RGB565ģʽ;1,JPEGģʽ 
										
#define jpeg_dma_bufsize	5*1024		//����JPEG DMA����ʱ���ݻ���jpeg_buf0/1�Ĵ�С(*4�ֽ�)
volatile u32 jpeg_data_len=0; 			//buf�е�JPEG��Ч���ݳ���(*4�ֽ�)
volatile u8 jpeg_data_ok=0;				//JPEG���ݲɼ���ɱ�־ 
										//0,����û�вɼ���;
										//1,���ݲɼ�����,���ǻ�û����;
										//2,�����Ѿ����������,���Կ�ʼ��һ֡����
										
u32 *jpeg_buf0;							//JPEG���ݻ���buf,ͨ��malloc�����ڴ�
u32 *jpeg_buf1;							//JPEG���ݻ���buf,ͨ��malloc�����ڴ�
u32 *jpeg_data_buf;						//JPEG���ݻ���buf,ͨ��malloc�����ڴ�


//����JPEG����
//���ɼ���һ֡JPEG���ݺ�,���ô˺���,�л�JPEG BUF.��ʼ��һ֡�ɼ�.
void jpeg_data_process(void)
{
	u16 i;
	u16 rlen;//ʣ�����ݳ���
	u32 *pbuf;
	if(ovx_mode&0X01)	//ֻ����JPEG��ʽ��,����Ҫ������.
	{
		if(jpeg_data_ok==0)	//jpeg���ݻ�δ�ɼ���?
		{	
			DMA_Cmd(DMA2_Stream1, DISABLE);//ֹͣ��ǰ���� 
			while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//�ȴ�DMA2_Stream1������  
			rlen=jpeg_dma_bufsize-DMA_GetCurrDataCounter(DMA2_Stream1);//�õ��˴����ݴ���ĳ���
			pbuf=jpeg_data_buf+jpeg_data_len;//ƫ�Ƶ���Ч����ĩβ,�������
			if(DMA2_Stream1->CR&(1<<19))for(i=0;i<rlen;i++)pbuf[i]=jpeg_buf1[i];//��ȡbuf1�����ʣ������
			else for(i=0;i<rlen;i++)pbuf[i]=jpeg_buf0[i];//��ȡbuf0�����ʣ������ 
			jpeg_data_len+=rlen;			//����ʣ�೤��
			jpeg_data_ok=1; 				//���JPEG���ݲɼ��갴��,�ȴ�������������
		}
		if(jpeg_data_ok==2)	//��һ�ε�jpeg�����Ѿ���������
		{
			DMA_SetCurrDataCounter(DMA2_Stream1,jpeg_dma_bufsize);//���䳤��Ϊjpeg_buf_size*4�ֽ�
			DMA_Cmd(DMA2_Stream1,ENABLE); //���´���
			jpeg_data_ok=0;					//�������δ�ɼ�
			jpeg_data_len=0;				//�������¿�ʼ
		}
	}else
	{
		if(bmp_request==1)//��RGB��,��bmp��������,�ر�DCMI
		{
			DCMI_Stop();	//ֹͣDCMI
			bmp_request=0;	//������������.
		}
		LCD_SetCursor(0,0);  
		LCD_WriteRAM_Prepare();		//��ʼд��GRAM
	}	
} 
//jpeg���ݽ��ջص�����
void jpeg_dcmi_rx_callback(void)
{ 
	u16 i;
	u32 *pbuf;
	pbuf=jpeg_data_buf+jpeg_data_len;//ƫ�Ƶ���Ч����ĩβ
	if(DMA2_Stream1->CR&(1<<19))//buf0����,��������buf1
	{ 
		for(i=0;i<jpeg_dma_bufsize;i++)pbuf[i]=jpeg_buf0[i];//��ȡbuf0���������
		jpeg_data_len+=jpeg_dma_bufsize;//ƫ��
	}else //buf1����,��������buf0
	{
		for(i=0;i<jpeg_dma_bufsize;i++)pbuf[i]=jpeg_buf1[i];//��ȡbuf1���������
		jpeg_data_len+=jpeg_dma_bufsize;//ƫ�� 
	} 	
}
//�л�ΪOV2640ģʽ��GPIOC8/9/11�л�Ϊ DCMI�ӿڣ�
void sw_ov5640_mode(void)
{
	OV5640_WR_Reg(0X3017,0XFF);	//����OV5650���(����������ʾ)
	OV5640_WR_Reg(0X3018,0XFF); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_DCMI);  //PC8,AF13  DCMI_D2
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_DCMI);  //PC9,AF13  DCMI_D3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_DCMI); //PC11,AF13 DCMI_D4  
 
} 
//�л�ΪSD��ģʽ��GPIOC8/9/11�л�Ϊ SDIO�ӿڣ�
void sw_sdcard_mode(void)
{
	OV5640_WR_Reg(0X3017,0X00);	//�ر�OV5640ȫ�����(��Ӱ��SD��ͨ��)
	OV5640_WR_Reg(0X3018,0X00); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_SDIO);  //PC8,AF12
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_SDIO);//PC9,AF12 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SDIO); 
}
//�ļ������������⸲�ǣ�
//mode:0,����.bmp�ļ�;1,����.jpg�ļ�.
//bmp��ϳ�:����"0:PHOTO/PIC13141.bmp"���ļ���
//jpg��ϳ�:����"0:PHOTO/PIC13141.jpg"���ļ���
void camera_new_pathname(u8 *pname,u8 mode)
{	 
	u8 res;					 
	u16 index=0;
	while(index<0XFFFF)
	{
		if(mode==0)sprintf((char*)pname,"0:PHOTO/PIC%05d.bmp",index);
		else sprintf((char*)pname,"0:PHOTO/PIC%05d.jpg",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//���Դ�����ļ�
		if(res==FR_NO_FILE)break;		//���ļ���������=����������Ҫ��.
		index++;
	}
}
//OV5640����jpgͼƬ
//����ֵ:0,�ɹ�
//    ����,�������
u8 ov5640_jpg_photo(u8  *pname)
{
	FIL* f_jpg; 
	u8 res=0;
	u32 bwr;
	u32 i;
	u8* pbuf;
	int size;
	f_jpg=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
	if(f_jpg==NULL)return 0XFF;				//�ڴ�����ʧ��.
	ovx_mode=1;
	jpeg_data_ok=0;
	sw_ov5640_mode();						//�л�ΪOV5640ģʽ 
	OV5640_JPEG_Mode();						//JPEGģʽ  
	OV5640_OutSize_Set(16,4,400,300);		//��������ߴ�(500W) ======================================================================================= (600;300)
	dcmi_rx_callback=jpeg_dcmi_rx_callback;	//JPEG�������ݻص�����
	DCMI_DMA_Init((u32)jpeg_buf0,(u32)jpeg_buf1,jpeg_dma_bufsize,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//DCMI DMA����(˫����ģʽ)
//	DCMI_DMA_Init((u32)dcmi_line_buf[0],(u32)dcmi_line_buf[1],jpeg_line_size,2,1);//DCMI DMA����    
	DCMI_Start(); 			//�������� 
	while(jpeg_data_ok!=1);	//�ȴ���һ֡ͼƬ�ɼ���
	jpeg_data_ok=2;			//���Ա�֡ͼƬ,������һ֡�ɼ� 
	while(jpeg_data_ok!=1);	//�ȴ��ڶ�֡ͼƬ�ɼ���,�ڶ�֡,�ű��浽SD��ȥ. 
	DCMI_Stop(); 			//ֹͣDMA����
	ovx_mode=0; 
	sw_sdcard_mode();		//�л�ΪSD��ģʽ
	res=f_open(f_jpg,(const TCHAR*)pname,FA_WRITE|FA_CREATE_NEW);//ģʽ0,���߳��Դ�ʧ��,�򴴽����ļ�	 
	if(res==0)
	{
		printf("jpeg data size:%d\r\n",jpeg_data_len*4);//���ڴ�ӡJPEG�ļ���С+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++HEREHEREHERE
		pbuf=(u8*)jpeg_data_buf;
		for(i=0;i<jpeg_data_len*4;i++)//����0XFF,0XD8��0XFF,0XD9,��ȡjpg�ļ���С
		{
			if((pbuf[i]==0XFF)&&(pbuf[i+1]==0XD8))break;//�ҵ�FF D8
		}
		if(i==jpeg_data_len*4)res=0XFD;//û�ҵ�0XFF,0XD8
		else//�ҵ���
		{
			pbuf+=i;//ƫ�Ƶ�0XFF,0XD8��
			printf("IDS\r\n");
			delay_ms(200);
			for(size = 0;size<jpeg_data_len*4-1;size++){
				printf("%x\n",pbuf[size]);
			}
			delay_ms(200);
			printf("IDE\r\n");
			delay_ms(200);
			res=f_write(f_jpg,pbuf,jpeg_data_len*4-i,&bwr);
			if(bwr!=(jpeg_data_len*4-i))res=0XFE; 
		}
	}
	jpeg_data_len=0;
	f_close(f_jpg); 
	sw_ov5640_mode();		//�л�ΪOV5640ģʽ
	OV5640_RGB565_Mode();	//RGB565ģʽ  
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Disable);//DCMI DMA����  
	myfree(SRAMIN,f_jpg); 
	return res;
}
int main(void)
{ 
	u8 res,fac;							 
	u8 *pname;				//��·�����ļ��� 
	u8 key;					//��ֵ		   
	u8 i;						 
	u8 sd_ok=1;				//0,sd��������;1,SD������. 
 	u8 scale=1;				//Ĭ����ȫ�ߴ�����
	u8 msgbuf[15];			//��Ϣ������
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
	usmart_dev.init(84);		//��ʼ��USMART
	TIM3_Int_Init(10000-1,8400-1);//10Khz����,1�����ж�һ��
 	LCD_Init();					//LCD��ʼ�� 
	FSMC_SRAM_Init();			//��ʼ���ⲿSRAM.
	BEEP_Init();				//��������ʼ��
 	KEY_Init();					//������ʼ�� 
	OV5640_Init();				//��ʼ��OV5640
	sw_sdcard_mode();				//�л�ΪSDIOģʽ	
	W25QXX_Init();				//��ʼ��W25Q128 
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMEX);		//��ʼ���ڲ��ڴ��  
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ��
	SD_Init();							//SD����ʼ��
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
  f_mount(fs[0],"0:",1); 		//����SD��  
	POINT_COLOR=RED;
	while(font_init()) 		//����ֿ�
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}
	while(OV5640_Init())
	{
		LCD_ShowString(30,50,200,16,16,"OV5640 Init error");
		delay_ms(200);
		LCD_Fill(30,50,240,66,WHITE);//�����ʾ	     
		delay_ms(200);
	}
	sw_sdcard_mode(); 		//�л�ΪSD��ģʽ	
	if(SD_Init())
	{
		LCD_ShowString(30,50,240,16,16,"SD Init error��");
		delay_ms(200);
		LCD_Fill(30,50,240,66,WHITE);//�����ʾ	     
		delay_ms(200);
	}
	f_mount(fs[0],"0:",1); 		//����SD��
	Show_Str(30,50,200,16,"Explorer STM32F4������",16,0);	 			    	 
	//Show_Str(30,70,200,16,"OV5640�����ʵ��",16,0);				    	 
	//Show_Str(30,90,200,16,"KEY0:����(bmp��ʽ)",16,0);			    	 
	Show_Str(30,110,200,16,"KEY1:����(jpg��ʽ)",16,0);			    	 
	//Show_Str(30,130,200,16,"KEY2:�Զ��Խ�",16,0);					    	 
	Show_Str(30,150,200,16,"WK_UP:FullSize/Scale",16,0);				    	 
	//Show_Str(30,170,200,16,"2016��6��1��",16,0);
	res=f_mkdir("0:/PHOTO");		//����PHOTO�ļ���
	if(res!=FR_EXIST&&res!=FR_OK) 	//�����˴���
	{		    
		Show_Str(30,190,240,16,"SD������!!!!!",16,0);
		delay_ms(200);				  
		Show_Str(30,210,240,16,"���չ��ܽ�������!",16,0);
		sd_ok=0;  	
	} 	
	jpeg_buf0=mymalloc(SRAMIN,jpeg_dma_bufsize*4);	//Ϊjpeg dma���������ڴ�	
	jpeg_buf1=mymalloc(SRAMIN,jpeg_dma_bufsize*4);	//Ϊjpeg dma���������ڴ�	
	jpeg_data_buf=mymalloc(SRAMEX,300*1024);		//Ϊjpeg�ļ������ڴ�(���300KB)
 	pname=mymalloc(SRAMIN,30);//Ϊ��·�����ļ�������30���ֽڵ��ڴ�	 
 	while(pname==NULL||!jpeg_buf0||!jpeg_buf1||!jpeg_data_buf)	//�ڴ�������
 	{	    
		Show_Str(30,230,240,16,"�ڴ����ʧ��!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,230,240,146,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}	
	Show_Str(30,190,250,16,"OV5640 ����",16,0);
	sw_ov5640_mode();
	//�Զ��Խ���ʼ��
	OV5640_RGB565_Mode();	//RGB565ģʽ 
	OV5640_Focus_Init(); 
	OV5640_Light_Mode(0);	//�Զ�ģʽ
	OV5640_Color_Saturation(3);//ɫ�ʱ��Ͷ�0
	OV5640_Brightness(4);	//����0
	OV5640_Contrast(3);		//�Աȶ�0
	OV5640_Sharpness(33);	//�Զ����
	OV5640_Focus_Constant();//���������Խ�
	
	My_DCMI_Init();			//DCMI����
	MYDCMI_DMA_Init((u32)&LCD->LCD_RAM,1,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Disable);//DCMI DMA����
//	DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Disable);//DCMI DMA����
	OV5640_OutSize_Set(4,0,lcddev.width,lcddev.height);	//����������ʾ
	DCMI_Start(); 			//��������
	
	while(1)
	{	
		key=KEY_Scan(0);//��֧������
		if(key)
		{ 
			if(key!=KEY2_PRES)
			{
				//if(key==KEY0_PRES)//�����BMP����,��ȴ�1����,ȥ����,�Ի���ȶ���bmp��Ƭ	
				//{
					//delay_ms(300);
					//bmp_request=1;		//����ر�DCMI
					//while(bmp_request);	//�ȴ����������
 				//}else DCMI_Stop();
			}
			if(key==WKUP_PRES)		//���Ŵ���
			{
				scale=!scale;  
				if(scale==0)
				{
					fac=800/lcddev.height;//�õ���������
					OV5640_OutSize_Set((1280-fac*lcddev.width)/2,(800-fac*lcddev.height)/2,lcddev.width,lcddev.height); 	 
					sprintf((char*)msgbuf,"Full Size 1:1");
				}else 
				{
					OV5640_OutSize_Set(16,4,lcddev.width,lcddev.height);
					sprintf((char*)msgbuf,"Scale");
				}
				delay_ms(800); 	
			}else if(key==KEY2_PRES)OV5640_Focus_Single(); //�ֶ������Զ��Խ�
			else if(sd_ok)//SD�������ſ�������
			{    
				sw_sdcard_mode();	//�л�ΪSD��ģʽ
				if(key==KEY0_PRES)	//BMP����
				{
					camera_new_pathname(pname,0);	//�õ��ļ���	
					res=bmp_encode(pname,0,0,lcddev.width,lcddev.height,0);
					sw_ov5640_mode();				//�л�ΪOV5640ģʽ
				}else if(key==KEY1_PRES)//JPG����
				{
					camera_new_pathname(pname,1);//�õ��ļ���	
					res=ov5640_jpg_photo(pname);
					
					if(scale==0)
					{
						fac=800/lcddev.height;//�õ���������
						OV5640_OutSize_Set((1280-fac*lcddev.width)/2,(800-fac*lcddev.height)/2,lcddev.width,lcddev.height); 	 
 					}else 
					{
						OV5640_OutSize_Set(16,4,lcddev.width,lcddev.height);
 					}	
				}
				if(res)//��������
				{
					Show_Str(30,130,240,16,"д���ļ�����!",16,0);		 
				}else 
				{
					Show_Str(30,130,240,16,"���ճɹ�!",16,0);
					Show_Str(30,150,240,16,"����Ϊ:",16,0);
					Show_Str(30+42,150,240,16,pname,16,0);		    
					BEEP=1;	//�������̽У���ʾ�������
					delay_ms(100);
					BEEP=0;	//�رշ�����
				}  
				delay_ms(1000);		//�ȴ�1����	
				DCMI_Start();		//������ʹ��dcmi,Ȼ�������ر�DCMI,�����ٿ���DCMI,���Է�ֹRGB���Ĳ�������.
				DCMI_Stop();			
			}else //��ʾSD������
			{					    
				Show_Str(30,130,240,16,"SD������!",16,0);
				Show_Str(30,150,240,16,"���չ��ܲ�����!",16,0);			    
			}   		
			if(key!=KEY2_PRES)DCMI_Start();//��ʼ��ʾ  
		} 
		delay_ms(10);
		i++;
		if(i==20)//DS0��˸.
		{
			i=0;
			LED0=!LED0;
 		}	  
	} 
}
