#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"
#include "oled.h"
/************************************************/

//定义任务栈
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

//定义任务控制块
static  OS_TCB   AppTaskStartTCB;

//任务函数声明
static void LED_Task(void *p_arg);



/************************************************/



int main(void)
{
	OS_ERR	err_creat;
	LED_Init();
	
	OSInit(&err_creat);
	
	OSTaskCreate (	&AppTaskStartTCB, 
					"LED1_TASK",
					LED_Task,
                    0,
                    3,
                    &AppTaskStartStk[0],
                    APP_TASK_START_STK_SIZE/10,
                    APP_TASK_START_STK_SIZE,
                    5,
                    0,
                    0,
                    (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                    &err_creat);
	
	OSStart(&err_creat);
	

}



static void LED_Task(void *p_arg)
{
	CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)p_arg;
    /* 开发板硬件初始化 */
    BSP_Init();          
  
    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();                           
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        
    OS_CPU_SysTickInit(cnts);                                   
     
    /* 内存初始化*/
    Mem_Init();                                                
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               
#endif

    CPU_IntDisMeasMaxCurReset();

	
	while(1)
	{
		macLED1_OFF();
		OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
		macLED1_ON();
	}
	
	
	
}


