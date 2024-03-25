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


#include "LED.h"
#include "bsp_usart.h"
#include "Key.h"


/**************************** 任务句柄 ********************************/
/*
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* 创建任务句柄 */
static TaskHandle_t LED_Task_Handle = NULL;/* LED任务句柄 */
static TaskHandle_t KEY_Task_Handle = NULL;/* KEY任务句柄 */

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


/******************************* 全局变量声明 ************************************/



/*************************************************************************
*                             函数声明
*************************************************************************/
static void AppTaskCreate(void);/* 用于创建任务 */

static void LED_Task(void *pvParameters);/* LED_Task任务实现 */
static void KEY_Task(void *pvParameters);/* KEY_Task任务实现 */


static void BSP_Init(void);


int main(void)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

    /* 开发板硬件初始化 */
    BSP_Init();

    printf("这是一个-STM32开发板-FreeRTOS任务管理实验！\n\n");
    printf("按下KEY1挂起任务，按下KEY2恢复任务\n");

    /* 创建AppTaskCreate任务 */
    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,   /* 任务入口函数 */
                          (const char *)"AppTaskCreate",   /* 任务名字 */
                          (uint16_t)512,         /* 任务栈大小 */
                          (void *)NULL,         /* 任务入口函数参数 */
                          (UBaseType_t)1,     /* 任务的优先级 */
                          (TaskHandle_t *)&AppTaskCreate_Handle); /* 任务控制块指针 */
    /* 启动任务调度 */
    if (pdPASS == xReturn)
        vTaskStartScheduler();   /* 启动任务，开启调度 */
    else
        return -1;
    while (1);

}


static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

    taskENTER_CRITICAL();           //进入临界区

    /* 创建LED_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t)LED_Task,  /* 任务入口函数 */
                          (const char *)"LED_Task",   /* 任务名字 */
                          (uint16_t)512,          /* 任务栈大小 */
                          (void *)NULL,           /* 任务入口函数参数 */
                          (UBaseType_t)2,         /* 任务的优先级 */
                          (TaskHandle_t *)&LED_Task_Handle); /* 任务控制块指针 */
    if (pdPASS == xReturn)
        printf("创建LED_Task任务成功!\r\n");
    /* 创建KEY_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t)KEY_Task,   /* 任务入口函数 */
                          (const char *)"KEY_Task",   /* 任务名字 */
                          (uint16_t)512,         /* 任务栈大小 */
                          (void *)NULL,         /* 任务入口函数参数 */
                          (UBaseType_t)3,     /* 任务的优先级 */
                          (TaskHandle_t *)&KEY_Task_Handle); /* 任务控制块指针 */
    if (pdPASS == xReturn)
        printf("创建KEY_Task任务成功!\r\n");

    vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务

    taskEXIT_CRITICAL();            //退出临界区
}


static void LED_Task(void *parameter)
{
    while (1)
    {
        LED1_ON;
        printf("LED_Task Running,LED1_ON\r\n");
        vTaskDelay(500);   /* 延时500个tick */

        LED1_OFF;
        printf("LED_Task Running,LED1_OFF\r\n");
        vTaskDelay(500);   /* 延时500个tick */
    }
}

/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：
  * @ 返回值  ： 无
  ********************************************************************/
static void KEY_Task(void *parameter)
{
    while (1)
    {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
        {
            /* K1 被按下 */
            printf("	挂起LED任务！\n");
            vTaskSuspend(LED_Task_Handle);/* 挂起LED任务 */
            printf("	挂起LED任务成功！\n");
        }
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
        {
            /* K2 被按下 */
            printf("	恢复LED任务！\n");
            vTaskResume(LED_Task_Handle);/* 恢复LED任务！ */
            printf("	恢复LED任务成功！\n");
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

