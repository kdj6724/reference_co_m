// hc_06.c
// kdj6724@naver.com
// 2018-10-26
#include <string.h>
#include "dev/connectivity/hc_06.h"

#define HC06_UART_RXLEN  128
USART_TypeDef* hc06UART_ = NULL;
uint8_t rxbuf_[HC06_UART_RXLEN];

void hc06_usart_rx_callback(USART_TypeDef* uart) {
  static uint16_t cnt = 0;
  char t;

  if(LL_USART_IsActiveFlag_RXNE(uart)) {
    t = LL_USART_ReceiveData8(uart);
#if 0
    if((t!='\n') && (cnt < (HC06_UART_RXLEN-1))) {
      rxbuf_[cnt] = t;
      cnt++;
    }
    else {
     // printf("[esp8266-dev] received : %s\r\n", rxbuf_);
      enqueue(&hc06MessageQueue_, (char*)rxbuf_, cnt);
      memset(rxbuf_, 0, sizeof(rxbuf_));
      cnt = 0;
    }
#else

  enqueue(&hc06MessageQueue_, (char*)&t, 1);
  memset(rxbuf_, 0, sizeof(rxbuf_));
  cnt = 0;
#endif
  }
}

int hc06_send_atcmd(USART_TypeDef* uart, uint8_t *Buffer, int reqLen) {
	int len = 0;

	  if (uart == NULL) {
	    printf("[esp8266-dev] uart is NULL(%d)\r\n", __LINE__);
	    return -1;
	  }

	  if (reqLen == 0)
	    return 0;

	  while(*Buffer){
	    while(!LL_USART_IsActiveFlag_TXE(uart)) {
	      HAL_Delay(10);
	    };
	    LL_USART_TransmitData8(uart,*Buffer++);
	    len++;
	    reqLen--;
	    if (reqLen == 0)
	      break;
	  }
	  return len;
}

int hc06_init(USART_TypeDef* uart) {
  init_queue(&hc06MessageQueue_);
  hc06UART_ = uart;
  hc06_set_bluetoothname("kdj6724", 7);
  HAL_Delay(500);
  hc06_set_pin("1111", 4);
  HAL_Delay(500);
  LL_USART_EnableIT_RXNE(uart);
  return 0;
}

int hc06_set_baudrate(int val) {
  uint8_t cmd[32];
  memset(cmd, 0, sizeof(cmd));
  sprintf((char*)cmd, "%sBAUD%d",HC06_ATCMD_PREFIX, val);
  return hc06_send_atcmd(hc06UART_, cmd, strlen((char*)cmd));
}

int hc06_set_bluetoothname(char* name, int len) {
  uint8_t cmd[32];
  memset(cmd, 0, sizeof(cmd));
  sprintf((char*)cmd, "%sNAME%s",HC06_ATCMD_PREFIX, name);
  return hc06_send_atcmd(hc06UART_, cmd, strlen((char*)cmd));
}

int hc06_set_pin(char* pin, int len) {
  uint8_t cmd[32];
  memset(cmd, 0, sizeof(cmd));
  sprintf((char*)cmd, "%sPIN%s",HC06_ATCMD_PREFIX, pin);
  return hc06_send_atcmd(hc06UART_, cmd, strlen((char*)cmd));
}

