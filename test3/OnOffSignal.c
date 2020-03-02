/****************************************************
* 文 件 名：
* 描    述：开关量信号输入输出操作文件
* 作    者：youdream
* 编写时间：2017.3.3
* 修改日期:	
  |条目  |修改人     |日期       |修改内容
  1、    youdream    2017.08.25  a)各开关量输入信号拥有独立的滤波时间
  2、    youdream    2018.05.16  a)增加打印开关量输入状态
  3、    youdream    2018.08.17  a)最大支持16个开关量输入信号
  4、    youdream    2018.09.03  a)文件整理
  5、    youdream    2018.12.28  a)系统时基改为钩子函数的形式
* 备    忘：
  1、开关量输出初始化为合适状态；
*****************************************************/

#include "OnOffSignal.h"

/**********************变量定义**********************/
// ---------------------------------------------------
/* 开关量输入状态 */
uint16_t OnOffInputData = 0;

/* 开关量输入测试开关 */
uint8_t yOnOffInputTestSw = 0;

/* 开关量输入抗抖动时间 */
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

/**********************函数声明**********************/
// ---------------------------------------------------
void OnOffSignalProc(void);
void OnOffSignalInit(void);
void OnOffInputTestSwHook(uint8_t cmd);
// ---------------------------------------------------
uint16_t OnOffIntputAllScan(void);
// ---------------------------------------------------
void OnOffSignalGpioConfig(void);

/**********************函数定义**********************/
// ---------------------------------------------------
// -----------------------------------------------------------------------------
/****************************************************
* 函数名称 ：
* 功    能 ：开关量信号处理进程
* 入口参数 ：无
* 返回参数 ：无
* 注意事项 ：
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
			/* 重置计时原点 */
			TimeCountOrigin = yd_GetSysTimeHook();

			DPrintf("各开关量输入信号电平状态---------------------------------\r\n");
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
* 函数名称 ：
* 功    能 ：开关量信号初始化
* 入口参数 ：无
* 返回参数 ：无
* 注意事项 ：    
*****************************************************/
void OnOffSignalInit(void)
{
	OnOffSignalGpioConfig();
}

/****************************************************
* 函数名称 ：
* 功    能 ：测试开关开闭
* 入口参数 ：
	cmd：0-关闭，非0-打开
* 返回参数 ：无
* 注意事项 ：
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
* 函数名称 ：
* 功    能 ：扫描开关量输入信号变化 
* 入口参数 ：无
* 返回参数 ：开关量信号状态，uint16_t类型，
			 每1位代表一个开关量输入信号，从低位开始数是开关量输入1、2……16。 
* 注意事项 ：    
	1、中断中调用； 
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
		/* 对应位状态发生变化并且变化后的状态保持一段时间 */
		if (((IntputNew>>i)&0x0001) != ((IntputOld>>i)&0x0001))
		{
			TimeCnt[i]++;
			if (TimeCnt[i] >= OnOffInputAntiJamTimeTab[i]/*ON_OFF_INPUT_ANTI_JAM_TIME*/)
			{
				TimeCnt[i] = 0;
				
				/* 只针对对应位的状态操作 */
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
* 函数名称 ：
* 功    能 ：扫描开关量输入信号变化 
* 入口参数 ：无
* 返回参数 ：开关量信号状态，uint16_t类型，
			 每1位代表一个开关量输入信号，从低位开始数是开关量输入1、2……16。 
* 注意事项 ：    
	1、中断或主循环中调用，放在中断中调用会更合适点；
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
	#error "Error:请在此添加相关程序！"				 
   #endif

	for (i = 0;i < ON_OFF_INPUT_SIGNAL_NUM;i++)
	{	
		/* 对应位状态发生变化并且变化后的状态保持一段时间 */
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
					
					/* 只针对对应位的状态操作 */
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
* 函数名称 ：
* 功    能 ：xxx
* 入口参数 ：xxx
* 返回参数 ：xxx
* 注意事项 ：    
*****************************************************/
uint8_t OnOffSignalIntputPin1Scan(void)
{
	return 0;
}

/****************************************************
* 函数名称 ：
* 功    能 ：xxx
* 入口参数 ：xxx
* 返回参数 ：xxx
* 注意事项 ：    
*****************************************************/
uint8_t OnOffSignalOutputCtrl(void)
{
	return 0;
}

// -----------------------------------------------------------------------------
/****************************************************
* 函数名称 ：
* 功    能 ：开关量控制端口配置
* 入口参数 ：无
* 返回参数 ：无
* 注意事项 ：
*****************************************************/
void OnOffSignalGpioConfig(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;							// 定义一个GPIO_InitTypeDef类型的结构体	 	 	
		
	/* 开关量输出控制引脚 ------------------------- */
   #if 0	
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 1
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN1_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 5
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN1_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN2_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN3_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN4_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN5_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 6	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN1_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN2_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN3_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN4_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN5_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
	RCC_APB2PeriphClockCmd(ON_OFF_OUTPUT_PIN6_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
   #else
	#error "Error:请在此添加相关程序！"	
	#endif	
		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				// 设置引脚的输出速率，50MHz	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				// 设置引脚的操作模式，推挽输出		
   #if 0	
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 1
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN1_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO 
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 5
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN1_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN2_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN3_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN4_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN5_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN5_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
	ON_OFF_OUTPUT_PIN1 = 0;
	ON_OFF_OUTPUT_PIN2 = 0;
	ON_OFF_OUTPUT_PIN3 = 1;
	ON_OFF_OUTPUT_PIN4 = 0;
	ON_OFF_OUTPUT_PIN5 = 0;
   #elif ON_OFF_OUTPUT_SIGNAL_NUM == 6
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN1_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN2_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN3_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN4_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN5_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN5_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_OUTPUT_PIN6_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_OUTPUT_PIN6_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
	ON_OFF_OUTPUT_PIN1 = 0;
	ON_OFF_OUTPUT_PIN2 = 0;
	ON_OFF_OUTPUT_PIN3 = 0;
	ON_OFF_OUTPUT_PIN4 = 0;
	ON_OFF_OUTPUT_PIN5 = 0;
	ON_OFF_OUTPUT_PIN6 = 0;
   #else
	#error "Error:请在此添加相关程序！"	
	#endif	
	
	/* 开关量输入控制引脚 ------------------------- */
   #if 0	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 1
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN1_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
   #elif ON_OFF_INPUT_SIGNAL_NUM == 4	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN1_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN2_GPIO_CLK, ENABLE);	// 开启对应外设的时钟  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN3_GPIO_CLK, ENABLE);	// 开启对应外设的时钟 	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN4_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
   #elif ON_OFF_INPUT_SIGNAL_NUM == 7
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN1_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN2_GPIO_CLK, ENABLE);	// 开启对应外设的时钟  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN3_GPIO_CLK, ENABLE);	// 开启对应外设的时钟 	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN4_GPIO_CLK, ENABLE);	// 开启对应外设的时钟   
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN5_GPIO_CLK, ENABLE);	// 开启对应外设的时钟  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN6_GPIO_CLK, ENABLE);	// 开启对应外设的时钟 
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN7_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 9
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN1_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN2_GPIO_CLK, ENABLE);	// 开启对应外设的时钟  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN3_GPIO_CLK, ENABLE);	// 开启对应外设的时钟 	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN4_GPIO_CLK, ENABLE);	// 开启对应外设的时钟   
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN5_GPIO_CLK, ENABLE);	// 开启对应外设的时钟  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN6_GPIO_CLK, ENABLE);	// 开启对应外设的时钟 
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN7_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN8_GPIO_CLK, ENABLE);	// 开启对应外设的时钟  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN9_GPIO_CLK, ENABLE);	// 开启对应外设的时钟	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 10
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN1_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN2_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN3_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN4_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN5_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN6_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN7_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN8_GPIO_CLK, ENABLE);	// 开启对应外设的时钟  
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN9_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
	RCC_APB2PeriphClockCmd(ON_OFF_INTPUT_PIN10_GPIO_CLK, ENABLE);	// 开启对应外设的时钟
   #else
	#error "Error:请在此添加相关程序！"	
	#endif
			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				// 设置引脚的输出速率，50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					// 设置引脚的操作模式，上拉输入	//GPIO_Mode_IPU;
   #if 0	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 1
	GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN1_PIN;			// 选择要控制的GPIO引脚
   #elif ON_OFF_INPUT_SIGNAL_NUM == 4	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN1_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN2_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO	
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN3_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN4_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
   #elif ON_OFF_INPUT_SIGNAL_NUM == 7
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN1_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN2_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO	
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN3_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN4_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN5_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN5_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN6_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN6_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN7_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN7_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
   #elif ON_OFF_INPUT_SIGNAL_NUM == 9
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN1_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN2_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO	
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN3_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN4_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN5_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN5_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN6_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN6_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN7_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN7_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN8_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN8_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN9_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN9_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
   #elif ON_OFF_INPUT_SIGNAL_NUM == 10
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN1_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN1_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN2_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN2_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO	
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN3_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN3_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN4_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN4_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN5_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN5_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN6_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN6_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN7_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN7_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN8_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN8_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
	
    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN9_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN9_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO

    GPIO_InitStructure.GPIO_Pin = ON_OFF_INTPUT_PIN10_PIN;			// 选择要控制的GPIO引脚
	GPIO_Init(ON_OFF_INTPUT_PIN10_GPIO_PORT, &GPIO_InitStructure);	// 初始化对应GPIO
   #else
	#error "Error:请在此添加相关程序！"	
   #endif
}

/*********************End of File********************/ 
#if 0
开关量输入测试：同时给所有开关量输入信号加高/低电平信号，则IO检测到的输入应为高/低电平信号；
开关量输出测试：同时控制所有开关量输出高/低电平信号，则外面所接指示灯应该亮；
#endif
void OnOffSignalTest(void)
{
	ON_OFF_OUTPUT_PIN1 = 1;
	ON_OFF_OUTPUT_PIN2 = 1;
	// 此时灯应亮
	
	ON_OFF_OUTPUT_PIN1 = 0;
	ON_OFF_OUTPUT_PIN2 = 0;
	// 此时灯应灭
	
	// 实现全自动测试，最可靠的方法是做一个与之匹配的自动化测试板
	// ……
}
