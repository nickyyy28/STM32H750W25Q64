#include "exflash_task.h"
#include "cmsis_os.h"
#include "w25qxx.h"
#include "main.h"
#include "usbd_cdc_if.h"


void exflash_task(const void* param)
{
	
	while(1) {
		//CDC_Transmit_FS("hello h750\r\n", 12);
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		osDelay(500);
	}
}