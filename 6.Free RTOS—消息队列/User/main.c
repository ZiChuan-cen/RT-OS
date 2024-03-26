/**
  ******************************************************************************
  * @file    main.c
  * @author  zichuan
  * @version V1.0
  * @date    2023-06-13
  * @brief   主函数
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


/**************************** 任务句柄 ********************************/
/*
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */

static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t Receive_Task_Handle = NULL;
static TaskHandle_t Send_Task_Handle = NULL;

/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 *
 */

QueueHandle_t Test_Queue = NULL;


/******************************* 全局变量声明 ************************************/

#define QUEUE_LEN       4
#define QUEUE_SIZE      4


/*************************************************************************
*                             函数声明
*************************************************************************/

static void AppTaskCreate(void);
static void Receive_Task(void *parameter);
static void Send_Task(void *parameter);

static void BSP_Init(void);


int main(void)
{

    BaseType_t xReturn = pdPASS;    //定义一个创建信息返回值，默认为PASS

    BSP_Init();

    printf("消息队列\r\n");
    printf("按下KEY1或KEY2发送队列消息\r\n");
    printf("Receive任务接收到消息后回显\r\n");

    //创建AppTaskCreat函数
    xReturn = xTaskCreate((TaskFunction_t) AppTaskCreate,    //任务函数入口
                          (const char *) "Task_Create",   //任务名称
                          (uint16_t) 512, //任务栈大小
                          (void *) NULL,  //任务入口函数参数
                          (UBaseType_t) 1,    //优先级
                          (TaskHandle_t *) &AppTaskCreate_Handle);    //任务控制块

    //启用任务调度
    if (pdPASS == xReturn)
        vTaskStartScheduler();
    else
        return -1;


    while (1);

}


static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();       //进入临界区


    /* 创建Test_Queue */
    Test_Queue = xQueueCreate((UBaseType_t) QUEUE_LEN, /* 消息队列的长度 */
                              (UBaseType_t) QUEUE_SIZE); /* 消息的大小 */
    if (NULL != Test_Queue)
        printf("创建Test_Queue消息队列成功!\r\n");

    /* 创建Receive_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t)Receive_Task,  /* 任务入口函数 */
                          (const char *)"Receive_Task",   /* 任务名字 */
                          (uint16_t)512,          /* 任务栈大小 */
                          (void *)NULL,           /* 任务入口函数参数 */
                          (UBaseType_t)2,         /* 任务的优先级 */
                          (TaskHandle_t *)&Receive_Task_Handle); /* 任务控制块指针 */
    if (pdPASS == xReturn)
        printf("创建Receive_Task任务成功!\r\n");

    /* 创建Send_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t)Send_Task,   /* 任务入口函数 */
                          (const char *)"Send_Task",   /* 任务名字 */
                          (uint16_t)512,         /* 任务栈大小 */
                          (void *)NULL,         /* 任务入口函数参数 */
                          (UBaseType_t)3,     /* 任务的优先级 */
                          (TaskHandle_t *)&Send_Task_Handle); /* 任务控制块指针 */
    if (pdPASS == xReturn)
        printf("创建Send_Task任务成功!\n\n");

    vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务

    taskEXIT_CRITICAL();            //退出临界区


}


static void Receive_Task(void *parameter)
{
    BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdTRUE */
    uint32_t r_queue; /* 定义一个接收消息的变量 */
    while (1)
    {
        xReturn = xQueueReceive(Test_Queue,     /* 消息队列的句柄 */
                                &r_queue,      /* 发送的消息内容 */
                                portMAX_DELAY); /* 等待时间 一直等 */
        if (pdTRUE == xReturn)
        {
            printf("本次接收到的数据是%d\n\n", r_queue);

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
            printf("数据接收出错,错误代码0x%lx\n", xReturn);
    }
}


static void Send_Task(void *parameter)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    uint32_t send_data1 = 1;
    uint32_t send_data2 = 2;
    while (1)
    {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
        {
            /* K1 被按下 */
            printf("发送消息send_data1！\n");
            xReturn = xQueueSend(Test_Queue,  /* 消息队列的句柄 */
                                 &send_data1,/* 发送的消息内容 */
                                 0);         /* 等待时间 0 */
            if (pdPASS == xReturn)
                printf("消息send_data1发送成功!\n\n");
        }
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
        {
            /* K2 被按下 */
            printf("发送消息send_data2！\n");
            xReturn = xQueueSend(Test_Queue,  /* 消息队列的句柄 */
                                 &send_data2,/* 发送的消息内容 */
                                 0);         /* 等待时间 0 */
            if (pdPASS == xReturn)
                printf("消息send_data2发送成功!\n\n");
        }
        vTaskDelay(20);/* 延时20个tick */
    }
}




void BSP_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    LED_GPIO_Config();

    Usart1_Config();

    Key_GPIO_Config();
}

