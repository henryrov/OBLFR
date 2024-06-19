#ifndef __OBLFR_MAILBOX_NUTTX_H__
#define __OBLFR_MAILBOX_NUTTX_H__

#include <stdint.h>
#include <stdbool.h>

#include "oblfr_common.h"
#include "sdkconfig.h"

/* Peripheral device ID */
enum m0_interrupt_source_e {
  BFLB_IPC_DEVICE_SDHCI,
  BFLB_IPC_DEVICE_UART2,
  BLFB_IPC_DEVICE_USB,
  BFLB_IPC_DEVICE_EMAC,
  BFLB_IPC_DEVICE_GPIO,
  BFLB_IPC_DEVICE_MBOX_TX,
  BFLB_IPC_DEVICE_MBOX_RX,
  BFLB_IPC_DEVICE_DMA0_ALL,
  BFLB_IPC_DEVICE_DMA1_ALL,
  BFLB_IPC_DEVICE_IRTX,
  BFLB_IPC_DEVICE_IRRX,
  BFLB_IPC_DEVICE_GPADC_DMA,
  BFLB_IPC_DEVICE_SPI0,
  BFLB_IPC_DEVICE_UART0,
  BFLB_IPC_DEVICE_UART1,
  BFLB_IPC_DEVICE_GPIO_DMA,
  BFLB_IPC_DEVICE_I2C0,
  BFLB_IPC_DEVICE_I2C1,
  BFLB_IPC_DEVICE_PWM,
  BFLB_IPC_DEVICE_TIMER0_CH0,
  BFLB_IPC_DEVICE_TIMER0_CH1,
  BFLB_IPC_DEVICE_TIMER0_WDT,
  BFLB_IPC_DEVICE_I2S,
  BFLB_IPC_DEVICE_PDS_WAKEUP,
  BFLB_IPC_DEVICE_HBN_OUT0,
  BFLB_IPC_DEVICE_HBN_OUT1
};

/* MailBox Service */
#define BFLB_IPC_MBOX_VIRTIO            1

/* Operations for MBOX_VIRTIO */
#define BFLB_IPC_MBOX_VIRTIO_OP_KICK    1

typedef void (*mbox_signal_handler_t)(uint16_t service, uint16_t op, uint32_t param, void *arg);

oblfr_err_t oblfr_mailbox_nuttx_init(void);

oblfr_err_t oblfr_mailbox_nuttx_add_signal_handler(uint16_t service, uint16_t op, mbox_signal_handler_t handler, void *arg);
oblfr_err_t oblfr_mailbox_nuttx_del_signal_handler(uint16_t service, uint16_t op );
oblfr_err_t oblfr_mailbox_nuttx_send_signal(uint16_t service, uint16_t op, uint32_t data);
oblfr_err_t oblfr_mailbox_nuttx_mask_signal(uint16_t service, uint16_t op);
oblfr_err_t oblfr_mailbox_nuttx_unmask_signal(uint16_t service, uint16_t op);
#endif
