/****************************************************
* �� �� ����
* ��    �����������ź�������������ļ�
* ��    �ߣ�youdream
* ��дʱ�䣺2017.3.3
* �޸�����:	
  |��Ŀ  |�޸���     |����       |�޸�����
  1��    youdream    2017.08.25  a)�������������ź�ӵ�ж������˲�ʱ��
  2��    youdream    2018.05.16  a)���Ӵ�ӡ����������״̬
  3��    youdream    2018.08.17  a)���֧��16�������������ź�
  4��    youdream    2018.09.03  a)�ļ�����
  5��    youdream    2018.12.28  a)ϵͳʱ����Ϊ���Ӻ�������ʽ
* ��    ����
  1�������������ʼ��Ϊ����״̬��
*****************************************************/

#include "OnOffSignal.h"

/**********************��������**********************/
// ---------------------------------------------------
/* ����������״̬ */
uint16_t OnOffInputData = 0;

/* ������������Կ��� */
uint8_t yOnOffInputTestSw = 0;

/* ���������뿹����ʱ�� */
static uint32_t OnOffInputAntiJamTimeTab[ON_OFF_INPUT_SIGNAL_MAX] = \
{
	ON_OFF_INPUT1_ANTI_JAM_TIME,	\
	ON_OFF_INPUT2_ANTI_JAM_TIME,	\
	ON_OFF_INPUT3_ANTI_JAM_TIME,	\
	ON_OFF_INPUT4_ANTI_JAM_TIME,	\
	ON_OFF_INPUT5_ANTI_JAM_TIME,	\
	ON_OFF_INPUT6_ANTI_JAM_TIME,	\
	ON_OFF_INPUT7_ANTI_JAM_TIME,	\
	ON_OFF_INPUT8_ANTI_JAM_TIME,	\
	ON_OFF_INPUT9_ANTI_JAM_TIME,	\
	ON_OFF_INPUT10_ANTI_JAM_TIME,	\
	ON_OFF_INPUT11_ANTI_JAM_TIME,	\
	ON_OFF_INPUT12_ANTI_JAM_TIME,	\
	ON_OFF_INPUT13_ANTI_JAM_TIME,	\
	ON_OFF_INPUT14_ANTI_JAM_TIME,	\
	ON_OFF_INPUT15_ANTI_JAM_TIME,	\
	ON_OFF_INPUT16_ANTI_JAM_TIME
};

/**********************��������**********************/
// ---------------------------------------------------
void OnOffSignalProc(void);
void OnOffSignalInit(void);
void OnOffInputTestSwHook(uint8_t cmd);
// ---------------------------------------------------
uint16_t OnOffIntputAllScan(void);
// ---------------------------------------------------
void OnOffSignalGpioConfig(void);

/**********************��������**********************/
// ---------------------------------------------------
// -----------------------------------------------------------------------------
/****************************************************
* �������� ��
* ��    �� ���������źŴ������
* ��ڲ��� ����
* ���ز��� ����
* ע������ ��
*****************************************************/
void OnOffSignalProc(void)
{
   #if ON_OFF_INPUT_TEST_SW
	uint8_t i = 0;
	uint8_t sta = 0;
	static uint32_t TimeCountOrigin = 0;

	if (yOnOffInputTestSw)
	{
		if ((yd_GetSysTimeHook() - TimeCountOrigin) > ON_OFF_INTPUT_TEST_OUTPUT_TIME)
		{
			/* ���ü�ʱԭ�� */
			TimeCountOrigin = yd_GetSysTimeHook();

			DPrintf("�������������źŵ�ƽ״̬---------------------------------\r\n");
			DPrintf("SignalTotal = 0x%04x\r\n", OnOffInputData);
			DPrintf("Channel 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16\r\n");
			DPrintf("Signal:");

			for (i = 0;i < ON_OFF_INPUT_SIGNAL_NUM;i++)
			{
				if (OnOffInputData&(0x0001<<i))
				{
					sta = 1;
				}
				else
				{
					sta = 0;
				}
				DPrintf(" %d", sta);
				DPrintf("  ");
			}
			DPrintf("\r\n");
			DPrintf("\r\n");
		}
	}
   #endif
}

/****************************************************
* �������� ��
* ��    �� ���������źų�ʼ��
* ��ڲ��� ����
* ���ز��� ����
* ע������ ��    
*****************************************************/
void OnOffSignalInit(void)
{
	OnOffSignalGpioConfig();
}

/****************************************************
* �������� ��
* ��    �� �����Կ��ؿ���
* ��ڲ��� ��
	cmd��0-�رգ���0-��
* ���ز��� ����
* ע������ ��
*****************************************************/
void OnOffInputTestSwHook(uint8_t cmd)
{
	if (cmd == 0)
	{		
		yOnOffInputTestSw = 0;		
	}
	else
	{
		yOnOffInputTestSw = 1;
	}
}

// -----------------------------------------------------------------------------
#if 0
/****************************************************
* �������� ��
* ��    �� ��ɨ�迪���������źű仯 
* ��ڲ��� ����
* ���ز��� ���������ź�״̬��uint16_t���ͣ�
			 ÿ1λ����һ�������������źţ��ӵ�λ��ʼ���ǿ���������1��2����16�� 
* ע������ ��    
	1���ж��е��ã� 
*****************************************************/
uint16_t OnOffIntputAllScan(void)
{		    
	uint8_t  i = 0;
	uint16_t IntputNew = 0;
	static uint16_t IntputOld = 0;					   
	static uint32_t TimeCnt[ON_OFF_INPUT_SIGNAL_NUM] = {0};
							   
	IntputNew = ((ON_OFF_INTPUT_PIN1 << 0) |	\
 				 (ON_OFF_INTPUT_PIN2 << 1) |	\
	             (ON_OFF_INTPUT_PIN3 << 2));
						   
	for (i = 0;i < ON_OFF_INPUT_SIGNAL_NUM;i++)						   						   	
	{
		/* ��Ӧλ״̬�����仯���ұ仯���״̬����һ��ʱ�� */
		if (((IntputNew>>i)&0x0001) != ((IntputOld>>i)&0x0001))
		{
			TimeCnt[i]++;
			if (TimeCnt[i] >= OnOffInputAntiJamTimeTab[i]/*ON_OFF_INPUT_ANTI_JAM_TIME*/)
			{
				TimeCnt[i] = 0;
				
				/* ֻ��Զ�Ӧλ��״̬���� */
				IntputOld = (IntputOld&(~(0x0001<<i))) | (IntputNew&(0x0001<<i));
			}
		}
		else
		{
			TimeCnt[i] = 0;
		}		 
	}		
	
	return IntputOld;
}
#endif

/****************************************************
* �������� ��
* ��    �� ��ɨ�迪���������źű仯 
* ��ڲ��� ����
* ���ز��� ���������ź�״̬��uint16_t���ͣ�
			 ÿ1λ����һ�������������źţ��ӵ�λ��ʼ���ǿ���������1��2����16�� 
* ע������ ��    
	1���жϻ���ѭ���е��ã������ж��е��û�����ʵ㣻
*****************************************************/
uint16_t OnOffIntputAllScan(void)
{		    
	uint8_t  i = 0;
	uint16_t IntputNew = 0;
	static uint16_t IntputOld = 0;
	static uint32_t TimeCnt[ON_OFF_INPUT_SIGNAL_NUM] = {0};
	static uint8_t  flag[ON_OFF_INPUT_SIGNAL_NUM] = {0};

   #if 0	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 1
	IntputNew = ((ON_OFF_INTPUT_PIN1  << 0));
   #elif ON_OFF_INPUT_SIGNAL_NUM == 4
	IntputNew = ((ON_OFF_INTPUT_PIN1  << 0) |	\
 				 (ON_OFF_INTPUT_PIN2  << 1) |	\
	 		     (ON_OFF_INTPUT_PIN3  << 2) |	\
	             (ON_OFF_INTPUT_PIN4  << 3));
   #elif ON_OFF_INPUT_SIGNAL_NUM == 7
	IntputNew = ((ON_OFF_INTPUT_PIN1  << 0) |	\
 				 (ON_OFF_INTPUT_PIN2  << 1) |	\
	 		     (ON_OFF_INTPUT_PIN3  << 2) |	\
 				 (ON_OFF_INTPUT_PIN4  << 3) |	\
	 		     (ON_OFF_INTPUT_PIN5  << 4) |	\
 				 (ON_OFF_INTPUT_PIN6  << 5) |	\
	             (ON_OFF_INTPUT_PIN7  << 6));
   #elif ON_OFF_INPUT_SIGNAL_NUM == 9
	IntputNew = ((ON_OFF_INTPUT_PIN1  << 0) |	\
 				 (ON_OFF_INTPUT_PIN2  << 1) |	\
	 		     (ON_OFF_INTPUT_PIN3  << 2) |	\
 				 (ON_OFF_INTPUT_PIN4  << 3) |	\
	 		     (ON_OFF_INTPUT_PIN5  << 4) |	\
 				 (ON_OFF_INTPUT_PIN6  << 5) |	\
	 		     (ON_OFF_INTPUT_PIN7  << 6) |	\
 				 (ON_OFF_INTPUT_PIN8  << 7) |	\
	             (ON_OFF_INTPUT_PIN9  << 8));	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 10
	IntputNew = ((ON_OFF_INTPUT_PIN1  << 0) |	\
 				 (ON_OFF_INTPUT_PIN2  << 1) |	\
	 		     (ON_OFF_INTPUT_PIN3  << 2) |	\
 				 (ON_OFF_INTPUT_PIN4  << 3) |	\
	 		     (ON_OFF_INTPUT_PIN5  << 4) |	\
 				 (ON_OFF_INTPUT_PIN6  << 5) |	\
	 		     (ON_OFF_INTPUT_PIN7  << 6) |	\
 				 (ON_OFF_INTPUT_PIN8  << 7) |	\
	 		     (ON_OFF_INTPUT_PIN9  << 8) |	\
	             (ON_OFF_INTPUT_PIN10 << 9));
   #else
	#error "Error:���ڴ������س���"				 
   #endif

	for (i = 0;i < ON_OFF_INPUT_SIGNAL_NUM;i++)
	{	
		/* ��Ӧλ״̬�����仯���ұ仯���״̬����һ��ʱ�� */
		if (((IntputNew>>i)&0x0001) != ((IntputOld>>i)&0x0001))
		{
			if (!flag[i])
			{
				flag[i] = 1;
				TimeCnt[i] = yd_GetSysTimeHook();
			}
			else
			{
				if (yd_GetSysTimeHook() - TimeCnt[i] >= OnOffInputAntiJamTimeTab[i]/*ON_OFF_INPUT_ANTI_JAM_TIME*/)
				{
					flag[i] = 0;
					
					/* ֻ��Զ�Ӧλ��״̬���� */
					IntputOld = (IntputOld&(~(0x0001<<i))) | (IntputNew&(0x0001<< i));
				}
				else
				{}
			}
		}
		else
		{
			flag[i] = 0;
		}
	}

	OnOffInputData = IntputOld;

	return IntputOld;
}	

/****************************************************
* �������� ��
* ��    �� ��xxx
* ��ڲ��� ��xxx
* ���ز��� ��xxx
* ע������ ��    
*****************************************************/
uint8_t OnOffSignalIntputPin1Scan(void)
{
	return 0;
}

/****************************************************
* �������� ��
* ��    �� ��xxx
* ��ڲ��� ��xxx
* ���ز��� ��xxx
* ע������ ��    
*****************************************************/
uint8_t OnOffSignalOutputCtrl(void)
{
	return 0;
}

// -----------------------------------------------------------------------------
/****************************************************
* �������� ��
* ��    �� �����������ƶ˿�����
* ��ڲ��� ����
* ���ز��� ����
* ע������ ��
*****************************************************/
void OnOffSignalGpioConfig(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;							// ����һ��GPIO_InitTypeDef���͵Ľṹ��	 	 	
		
	/* ����������������� ------------------------- */
   #if 0	
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 1
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN1_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 5
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN1_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN2_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN3_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN4_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN5_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 6	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN1_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN2_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN3_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN4_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN5_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN6_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
   #else
	#error "Error:���ڴ������س���"	
	#endif	
		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				// �������ŵ�������ʣ�50MHz	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				// �������ŵĲ���ģʽ���������		
   #if 0	
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 1
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN1_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO 
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 5
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN1_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN2_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN3_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN4_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN5_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN5_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
	ON_OFF_OUTPUT_PIN1 = 0;
	ON_OFF_OUTPUT_PIN2 = 0;
	ON_OFF_OUTPUT_PIN3 = 1;
	ON_OFF_OUTPUT_PIN4 = 0;
	ON_OFF_OUTPUT_PIN5 = 0;
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 6
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN1_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN2_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN3_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN4_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN5_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN5_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN6_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_OUTPUT_PIN6_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
	ON_OFF_OUTPUT_PIN1 = 0;
	ON_OFF_OUTPUT_PIN2 = 0;
	ON_OFF_OUTPUT_PIN3 = 0;
	ON_OFF_OUTPUT_PIN4 = 0;
	ON_OFF_OUTPUT_PIN5 = 0;
	ON_OFF_OUTPUT_PIN6 = 0;
   #else
	#error "Error:���ڴ������س���"	
	#endif	
	
	/* ����������������� ------------------------- */
   #if 0	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 1
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN1_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
   #elif ON_OFF_INPUT_SIGNAL_NUM == 4	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN1_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN2_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN3_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ�� 	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN4_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
   #elif ON_OFF_INPUT_SIGNAL_NUM == 7
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN1_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN2_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN3_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ�� 	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN4_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��   
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN5_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN6_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ�� 
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN7_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 9
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN1_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN2_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN3_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ�� 	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN4_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��   
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN5_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN6_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ�� 
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN7_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN8_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN9_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 10
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN1_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN2_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN3_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN4_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN5_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN6_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN7_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN8_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN9_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN10_GPIO_CLK, ENABLE);	// ������Ӧ�����ʱ��
   #else
	#error "Error:���ڴ������س���"	
	#endif
			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				// �������ŵ�������ʣ�50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					// �������ŵĲ���ģʽ����������	//GPIO_Mode_IPU;
   #if 0	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 1
	GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN1_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
   #elif ON_OFF_INPUT_SIGNAL_NUM == 4	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN1_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN2_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO	
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN3_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN4_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
   #elif ON_OFF_INPUT_SIGNAL_NUM == 7
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN1_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN2_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO	
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN3_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN4_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN5_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN5_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN6_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN6_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN7_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN7_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 9
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN1_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN2_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO	
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN3_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN4_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN5_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN5_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN6_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN6_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN7_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN7_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN8_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN8_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN9_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN9_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
   #elif ON_OFF_INPUT_SIGNAL_NUM == 10
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN1_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN2_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO	
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN3_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN4_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN5_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN5_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN6_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN6_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN7_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN7_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN8_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN8_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN9_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN9_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN10_PIN;			// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_Init(ON_OFF_INTPUT_PIN10_GPIO_PORT, &GPIO_InitStructure);	// ��ʼ����ӦGPIO
   #else
	#error "Error:���ڴ������س���"	
   #endif
}

/*********************End of File********************/ 
#if 0
������������ԣ�ͬʱ�����п����������źżӸ�/�͵�ƽ�źţ���IO��⵽������ӦΪ��/�͵�ƽ�źţ�
������������ԣ�ͬʱ�������п����������/�͵�ƽ�źţ�����������ָʾ��Ӧ������
#endif
void OnOffSignalTest(void)
{
	ON_OFF_OUTPUT_PIN1 = 1;
	ON_OFF_OUTPUT_PIN2 = 1;
	// ��ʱ��Ӧ��
	
	ON_OFF_OUTPUT_PIN1 = 0;
	ON_OFF_OUTPUT_PIN2 = 0;
	// ��ʱ��Ӧ��
	
	// ʵ��ȫ�Զ����ԣ���ɿ��ķ�������һ����֮ƥ����Զ������԰�
	// ����
}
