#include "../Device/LED/led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"
#include "../Device/OLED-8P/oled.h"
/************************************************
 ALIENTEK 精英版STM32开发板UCOS实验
 例4-1 UCOSIII UCOSIII移植
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/

//UCOSIII中以下优先级用户程序不能使用，ALIENTEK
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司
//作者：正点原子 @ALIENTEK

int main(void)
{
	//Delay_init();
	LED_Init();
	OLED_Init();
	while(1)
	{
		LED0;
		display_GB2312_string(0, 1, "1111");
		delay_ms(1000);
		display_GB2312_string(1, 1, "2222");
		LED1;
		delay_ms(2000);
	}

}



