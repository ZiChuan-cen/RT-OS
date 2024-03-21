/**
  ******************************************************************************
  * @file    main.c
  * @author  zichuan
  * @version V1.0
  * @date    2023-06-13
  * @brief   ������
  ******************************************************************************
  */

//#include "stm32f10x.h"
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
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED������ */
static TaskHandle_t LED_Task_Handle = NULL;

/******************************* ȫ�ֱ������� ************************************/


/**************************************************************************
*                             ��������
**************************************************************************/
static void AppTaskCreate(void);/* ���ڴ������� */

static void LED_Task(void *pvParameters);/* LED_Task����ʵ�� */

static void BSP_Init(void);


int main(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	
    /* ������Ӳ����ʼ�� */
    BSP_Init();
    
	printf("����һ��-FreeRTOS-��̬��������!\r\n");
    /* ����AppTaskCreate���� */
    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,   /* ������ں��� */
                          (const char *)"AppTaskCreate",   /* �������� */
                          (uint16_t)512,         /* ����ջ��С */
                          (void *)NULL,         /* ������ں������� */
                          (UBaseType_t)1,     /* ��������ȼ� */
                          (TaskHandle_t *)&AppTaskCreate_Handle); /* ������ƿ�ָ�� */
    /* ����������� */
    if (pdPASS == xReturn)
        vTaskStartScheduler();   /* �������񣬿������� */
    else
        return -1;

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
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

    taskENTER_CRITICAL();           //�����ٽ���

    /* ����LED_Task���� */
    xReturn = xTaskCreate((TaskFunction_t)LED_Task,  /* ������ں��� */
                          (const char *)"LED_Task",   /* �������� */
                          (uint16_t)512,          /* ����ջ��С */
                          (void *)NULL,           /* ������ں������� */
                          (UBaseType_t)2,         /* ��������ȼ� */
                          (TaskHandle_t *)&LED_Task_Handle); /* ������ƿ�ָ�� */
    if (pdPASS == xReturn)
        printf("����LED_Task����ɹ�!\r\n");

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
    while (1)
    {
        LED1_ON;
        vTaskDelay(500);   /* ��ʱ500��tick */
        printf("LED_Task Running,LED1_ON\r\n");

        LED1_OFF;
        vTaskDelay(500);   /* ��ʱ500��tick */
        printf("LED_Task Running,LED1_OFF\r\n");
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


