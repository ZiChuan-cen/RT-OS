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
#include "queue.h"


#include "LED.h"
#include "bsp_usart.h"
#include "Key.h"


/**************************** ������ ********************************/
/*
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */

static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t Receive_Task_Handle = NULL;
static TaskHandle_t Send_Task_Handle = NULL;

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

QueueHandle_t Test_Queue = NULL;


/******************************* ȫ�ֱ������� ************************************/

#define QUEUE_LEN       4
#define QUEUE_SIZE      4


/*************************************************************************
*                             ��������
*************************************************************************/

static void AppTaskCreate(void);
static void Receive_Task(void *parameter);
static void Send_Task(void *parameter);

static void BSP_Init(void);


int main(void)
{

    BaseType_t xReturn = pdPASS;    //����һ��������Ϣ����ֵ��Ĭ��ΪPASS

    BSP_Init();

    printf("��Ϣ����\r\n");
    printf("����KEY1��KEY2���Ͷ�����Ϣ\r\n");
    printf("Receive������յ���Ϣ�����\r\n");

    //����AppTaskCreat����
    xReturn = xTaskCreate((TaskFunction_t) AppTaskCreate,    //���������
                          (const char *) "Task_Create",   //��������
                          (uint16_t) 512, //����ջ��С
                          (void *) NULL,  //������ں�������
                          (UBaseType_t) 1,    //���ȼ�
                          (TaskHandle_t *) &AppTaskCreate_Handle);    //������ƿ�

    //�����������
    if (pdPASS == xReturn)
        vTaskStartScheduler();
    else
        return -1;


    while (1);

}


static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();       //�����ٽ���


    /* ����Test_Queue */
    Test_Queue = xQueueCreate((UBaseType_t) QUEUE_LEN, /* ��Ϣ���еĳ��� */
                              (UBaseType_t) QUEUE_SIZE); /* ��Ϣ�Ĵ�С */
    if (NULL != Test_Queue)
        printf("����Test_Queue��Ϣ���гɹ�!\r\n");

    /* ����Receive_Task���� */
    xReturn = xTaskCreate((TaskFunction_t)Receive_Task,  /* ������ں��� */
                          (const char *)"Receive_Task",   /* �������� */
                          (uint16_t)512,          /* ����ջ��С */
                          (void *)NULL,           /* ������ں������� */
                          (UBaseType_t)2,         /* ��������ȼ� */
                          (TaskHandle_t *)&Receive_Task_Handle); /* ������ƿ�ָ�� */
    if (pdPASS == xReturn)
        printf("����Receive_Task����ɹ�!\r\n");

    /* ����Send_Task���� */
    xReturn = xTaskCreate((TaskFunction_t)Send_Task,   /* ������ں��� */
                          (const char *)"Send_Task",   /* �������� */
                          (uint16_t)512,         /* ����ջ��С */
                          (void *)NULL,         /* ������ں������� */
                          (UBaseType_t)3,     /* ��������ȼ� */
                          (TaskHandle_t *)&Send_Task_Handle); /* ������ƿ�ָ�� */
    if (pdPASS == xReturn)
        printf("����Send_Task����ɹ�!\n\n");

    vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����

    taskEXIT_CRITICAL();            //�˳��ٽ���


}


static void Receive_Task(void *parameter)
{
    BaseType_t xReturn = pdTRUE;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdTRUE */
    uint32_t r_queue; /* ����һ��������Ϣ�ı��� */
    while (1)
    {
        xReturn = xQueueReceive(Test_Queue,     /* ��Ϣ���еľ�� */
                                &r_queue,      /* ���͵���Ϣ���� */
                                portMAX_DELAY); /* �ȴ�ʱ�� һֱ�� */
        if (pdTRUE == xReturn)
        {
            printf("���ν��յ���������%d\n\n", r_queue);

//              if(r_queue == 1)
//              {
//                  LED_RGBOFF;
//                  LED_RED;
//              }
//              else
//                  LED_RGBOFF;
//                  LED_YELLOW;
        }
        else
            printf("���ݽ��ճ���,�������0x%lx\n", xReturn);
    }
}


static void Send_Task(void *parameter)
{
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    uint32_t send_data1 = 1;
    uint32_t send_data2 = 2;
    while (1)
    {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
        {
            /* K1 ������ */
            printf("������Ϣsend_data1��\n");
            xReturn = xQueueSend(Test_Queue,  /* ��Ϣ���еľ�� */
                                 &send_data1,/* ���͵���Ϣ���� */
                                 0);         /* �ȴ�ʱ�� 0 */
            if (pdPASS == xReturn)
                printf("��Ϣsend_data1���ͳɹ�!\n\n");
        }
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
        {
            /* K2 ������ */
            printf("������Ϣsend_data2��\n");
            xReturn = xQueueSend(Test_Queue,  /* ��Ϣ���еľ�� */
                                 &send_data2,/* ���͵���Ϣ���� */
                                 0);         /* �ȴ�ʱ�� 0 */
            if (pdPASS == xReturn)
                printf("��Ϣsend_data2���ͳɹ�!\n\n");
        }
        vTaskDelay(20);/* ��ʱ20��tick */
    }
}




void BSP_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    LED_GPIO_Config();

    Usart1_Config();

    Key_GPIO_Config();
}

