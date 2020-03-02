/****************************************************
* �� �� ����
* ��    ����LEDӦ��
* ��    �ߣ�youdream
* ��дʱ�䣺2018.12.27
* �޸ļ�¼��
  |��Ŀ  |�޸���     |����       |�޸�����
  1��    youdream    2018.12.27  a)��һ��
* ��    ����
  1����
*****************************************************/

#include "LedApp.h"

/**********************��������**********************/
// ---------------------------------------------------

/**********************��������**********************/
// ---------------------------------------------------
void AppTaskLed(void *p_arg);

/**********************��������**********************/
// ---------------------------------------------------
// -----------------------------------------------------------------------------
/****************************************************
* �������� ��
* ��    �� ��LedӦ��
* ��ڲ��� ��p_arg
* ���ز��� ����
* ע������ ��
*****************************************************/
void AppTaskLed(void *p_arg)
{
    OS_ERR	err;
	
	(void)p_arg;
	
	while(1)
	{
		WORK_LED = ~WORK_LED;
		OSTimeDly(CPU_RUN_LED_BLINK_TIME, OS_OPT_TIME_DLY, &err);
	}
}

// -----------------------------------------------------------------------------
/****************************************************
* �������� ��
* ��    �� ��CPU����ָʾ
* ��ڲ��� ����
* ���ز��� ����
* ע������ ����ѭ�����ж��е���
*****************************************************/
void LedCpuRunIndicate(void)
{
	static uint16_t TimeCnt = 0;
	
	TimeCnt++;	
	if (TimeCnt > CPU_RUN_LED_BLINK_TIME)
	{
		TimeCnt = 0;
		
		/* ָʾ�Ʒ�ת */
		WORK_LED = ~WORK_LED;
	}
}

/*********************End of File********************/ 