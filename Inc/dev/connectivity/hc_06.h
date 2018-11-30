// hc_06.h
// kdj6724@naver.com
// 2018-10-26
#include "stm32f1xx_hal.h"
#include "dev/data/queue.h"

#ifndef DEV_SENSOR_HC_06_H_
#define DEV_SENSOR_HC_06_H_

#ifdef __cplusplus
extern "C" {
#endif

Queue hc06MessageQueue_;
#define HC06_UART_ENDBYTE '\n'
#define HC06_ATCMD_PREFIX "AT+"

enum hc06_baudrate {
  HC06_BAUD_1200 = 0,
  HC06_BAUD_2400,
  HC06_BAUD_4800,
  HC06_BAUD_9600,
  HC06_BAUD_19200,
  HC06_BAUD_38400,
  HC06_BAUD_57600,
  HC06_BAUD_115200,
  HC06_BAUD_230400,
  HC06_BAUD_912600,
  HC06_BAUD_1382400,
};

enum hc06_cmd {
  HC06_CMD0 = 0,
  HC06_CMD1,
  HC06_CMD2,
  HC06_CMD3,
};

void hc06_usart_rx_callback(USART_TypeDef* uart);
int hc06_init(USART_TypeDef* uart);
int hc06_set_baudrate(int val);
int hc06_set_bluetoothname(char* name, int len);
int hc06_set_pin(char* pin, int len);

#ifdef __cplusplus
}
#endif

#endif  // DEV_SENSOR_HC_06_H_

