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
#include "delay.h"

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
/* LED1������ */
static TaskHandle_t LED1_Task_Handle = NULL;
/* LED2������ */
static TaskHandle_t LED2_Task_Handle = NULL;
/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 *
 */


/******************************* ȫ�ֱ������� ************************************/



/*************************************************************************
*                             ��������
*************************************************************************/
static void AppTaskCreate(void);    /* ���ڴ������� */

static void LED1_Task(void *pvParameters);  /* LED1_Task����ʵ�� */
static void LED2_Task(void *pvParameters);  /* LED2_Task����ʵ�� */
static void BSP_Init(void);

int main(void)
{

    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

    /* ������Ӳ����ʼ�� */
    BSP_Init();
    printf("FreeRTOS ��̬����������\r\n");
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

    while (1)
    {}
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

    /********************************* ����LED1_Task���� *********************************/
    xReturn = xTaskCreate((TaskFunction_t)LED1_Task,  /* ������ں��� */
                          (const char *)"LED1_Task",   /* �������� */
                          (uint16_t)512,          /* ����ջ��С */
                          (void *)NULL,           /* ������ں������� */
                          (UBaseType_t)2,         /* ��������ȼ� */
                          (TaskHandle_t *)&LED1_Task_Handle); /* ������ƿ�ָ�� */
    if (pdPASS == xReturn)
        printf("����LED1_Task����ɹ�!\r\n");

	
	
    /********************************* ����LED2_Task���� *********************************/
    xReturn = xTaskCreate((TaskFunction_t)LED2_Task,  /* ������ں��� */
                          (const char *)"LED2_Task",   /* �������� */
                          (uint16_t)512,          /* ����ջ��С */
                          (void *)NULL,           /* ������ں������� */
                          (UBaseType_t)5,         /* ��������ȼ� */
                          (TaskHandle_t *)&LED2_Task_Handle); /* ������ƿ�ָ�� */
    if (pdPASS == xReturn)
        printf("����LED2_Task����ɹ�!\r\n");
	
	
	

    vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����

    taskEXIT_CRITICAL();            //�˳��ٽ���
}




static void LED1_Task(void *parameter)
{
    while (1)
    {
        LED1_ON;
        printf("LED1_Task Running,LED1_ON\r\n");
        vTaskDelay(1000);   /* ��ʱ500��tick */

        LED1_OFF;
        printf("LED1_Task Over,LED1_OFF\r\n");
        vTaskDelay(1000);   /* ��ʱ500��tick */
    }
}


static void LED2_Task(void *parameter)
{
    while (1)
    {
        LED2_ON;
        printf("LED2_Task Running,LED2_ON\r\n");
        vTaskDelay(2000);   /* ��ʱ500��tick */

        LED2_OFF;
        printf("LED2_Task Over,LED2_OFF\r\n");
        vTaskDelay(2000);   /* ��ʱ500��tick */
    }
}

void BSP_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    LED_GPIO_Config();

    Usart1_Config();
}



