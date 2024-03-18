#include "../Device/LED/led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"
#include "../Device/OLED-8P/oled.h"
/************************************************
 ALIENTEK ��Ӣ��STM32������UCOSʵ��
 ��4-1 UCOSIII UCOSIII��ֲ
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//UCOSIII���������ȼ��û�������ʹ�ã�ALIENTEK
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾
//���ߣ�����ԭ�� @ALIENTEK

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



