/**
  ******************************************************************************
  * @file    main.c
  * @author  zichuan
  * @version V1.0
  * @date    2023-06-13
  * @brief   ������
  ******************************************************************************
  */

#include "stm32f10x.h"
//#include "delay.h"

#include "FreeRTOS.h"
#include "task.h"

#include "LED.h"
#include "bsp_usart.h"



/**************************** ������ ********************************/
/*
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
/* ���������� */
static TaskHandle_t AppTaskCreate_Handle;
/* LED������ */
static TaskHandle_t LED_Task_Handle;

/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */
/* AppTaskCreate���������ջ */
static StackType_t AppTaskCreate_Stack[128];
/* LED�����ջ */
static StackType_t LED_Task_Stack[128];

/* AppTaskCreate ������ƿ� */
static StaticTask_t AppTaskCreate_TCB;
/* LED ������ƿ� */
static StaticTask_t LED_Task_TCB;

/* �������������ջ */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* ��ʱ�������ջ */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/* ����������ƿ� */
static StaticTask_t Idle_Task_TCB;
/* ��ʱ��������ƿ� */
static StaticTask_t Timer_Task_TCB;

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void LED_Task(void *pvParameters);/* LED_Task����ʵ�� */



static void BSP_Init(void);

/**
    * ʹ���˾�̬�����ڴ棬�������������������û�ʵ�֣�������task.c�ļ���������
    *   ���ҽ��� configSUPPORT_STATIC_ALLOCATION ����궨��Ϊ 1 ��ʱ�����Ч
    */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);


int main(void)
{
    /* ������Ӳ����ʼ�� */
    BSP_Init();
    printf("����һ��-FreeRTOS-��̬����������!\r\n");
    /* ���� AppTaskCreate ���� */
    AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t)AppTaskCreate,         //������
                           (const char *)"AppTaskCreate",          //��������
                           (uint32_t)128,          //�����ջ��С
                           (void *)NULL,                       //���ݸ��������Ĳ���
                           (UBaseType_t)3,         //�������ȼ�
                           (StackType_t *)AppTaskCreate_Stack,     //�����ջ
                           (StaticTask_t *)&AppTaskCreate_TCB);    //������ƿ�

    if (NULL != AppTaskCreate_Handle) /* �����ɹ� */
		vTaskStartScheduler();   /* �������񣬿������� */

    while (1);  /* ��������ִ�е����� */
}



/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
    taskENTER_CRITICAL();           //�����ٽ���

    /* ����LED_Task���� */
    LED_Task_Handle = xTaskCreateStatic((TaskFunction_t)LED_Task,       //������
                                        (const char *)"LED_Task",           //��������
                                        (uint32_t)128,                          //�����ջ��С
                                        (void *)NULL,                       //���ݸ��������Ĳ���
                                        (UBaseType_t)4,                     //�������ȼ�
                                        (StackType_t *)LED_Task_Stack,      //�����ջ
                                        (StaticTask_t *)&LED_Task_TCB);     //������ƿ�

    if (NULL != LED_Task_Handle) /* �����ɹ� */
        printf("LED_Task���񴴽��ɹ�!\n");
    else
        printf("LED_Task���񴴽�ʧ��!\n");

    vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����

    taskEXIT_CRITICAL();            //�˳��ٽ���
}


/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��
  * @ ����ֵ  �� ��
  ********************************************************************/
static void LED_Task(void *parameter)
{
	printf("!1\r\n");
    while (1)
    {
        LED1_ON;
        printf("LED_Task Running,LED1_ON\r\n");
		vTaskDelay(500);   /* ��ʱ500��tick */

        LED1_OFF;
        printf("LED_Task Running,LED1_OFF\r\n");
		vTaskDelay(500);   /* ��ʱ500��tick */
    }
}







//Ӳ�������ʼ������
static void BSP_Init(void)
{
    /*
     * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
     * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
     * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
     */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* LED ��ʼ�� */
    LED_GPIO_Config();

    /* ���ڳ�ʼ��   */
    Usart1_Config();
}




/**
  **********************************************************************
  * @brief  ��ȡ��������������ջ��������ƿ��ڴ�
    *                   ppxTimerTaskTCBBuffer   :       ������ƿ��ڴ�
    *                   ppxTimerTaskStackBuffer :   �����ջ�ڴ�
    *                   pulTimerTaskStackSize   :       �����ջ��С
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  **********************************************************************
  */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &Idle_Task_TCB; /* ������ƿ��ڴ� */
    *ppxIdleTaskStackBuffer = Idle_Task_Stack; /* �����ջ�ڴ� */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE; /* �����ջ��С */
}

/**
  *********************************************************************
  * @brief  ��ȡ��ʱ������������ջ��������ƿ��ڴ�
    *                   ppxTimerTaskTCBBuffer   :       ������ƿ��ڴ�
    *                   ppxTimerTaskStackBuffer :   �����ջ�ڴ�
    *                   pulTimerTaskStackSize   :       �����ջ��С
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  **********************************************************************
  */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &Timer_Task_TCB; /* ������ƿ��ڴ� */
    *ppxTimerTaskStackBuffer = Timer_Task_Stack; /* �����ջ�ڴ� */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH; /* �����ջ��С */
}

/********************************END OF FILE****************************/

