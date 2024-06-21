#include <stdbool.h>
#include <string.h>
#include <sys/queue.h>
#include <board.h>
#include <bl808.h>
#include <bl808_common.h>
#include <bl808_glb.h>
#include <ipc_reg.h>
#include <bflb_clock.h>
#include <bflb_gpio.h>

#include "oblfr_courier.h"
#include "oblfr_usb_peripheral.h"
#define DBG_TAG "COURIER"
#include "log.h"

#ifndef CONFIG_CHIP_BL808
#error "This component is only for BL808"
#endif

typedef struct {
    char name[16];
    uint32_t irq;
    uint32_t count;
    void (*handler)(int irq, void *arg);
} courier_irq_t;

static void Send_IPC_IRQ(int device);

static void SDHCI_IRQHandler(int irq, void *arg)
{
    LOG_T("Got SDH IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_SDHCI);
}

static void UART2_IRQHandler(int irq, void *arg)
{
    LOG_T("Got UART2 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_UART2);
}

static void USB_IRQHandler(int irq, void *arg)
{
    LOG_T("Got USB IRQ\r\n");
    Send_IPC_IRQ(BLFB_IPC_DEVICE_USB);
}

static void EMAC_IRQHandler(int irq, void *arg)
{
    LOG_T("Got EMAC IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_EMAC);
}

static void GPIO_IRQHandler(int irq, void *arg)
{
    LOG_T("Got GPIO IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_GPIO);
}

static void DMA0_ALL_IRQHandler(int irq, void *arg)
{
    LOG_T("Got DMA0 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_DMA0_ALL);
}

static void DMA1_ALL_IRQHandler(int irq, void *arg)
{
    LOG_T("Got DMA1 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_DMA1_ALL);
}

static void IRTX_IRQHandler(int irq, void *arg)
{
    LOG_T("Got IRTX IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_IRTX);
}

static void IRRX_IRQHandler(int irq, void *arg)
{
    LOG_T("Got IRRX IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_IRRX);
}

static void GPADC_DMA_IRQHandler(int irq, void *arg)
{
    LOG_T("Got GPADC_DMA IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_GPADC_DMA);
}

static void SPI0_IRQHandler(int irq, void *arg)
{
    LOG_T("Got SPI0 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_SPI0);
}

static void UART0_IRQHandler(int irq, void *arg)
{
    LOG_T("Got UART0 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_UART0);
}

static void UART1_IRQHandler(int irq, void *arg)
{
    LOG_T("Got UART1 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_UART1);
}

static void GPIO_DMA_IRQHandler(int irq, void *arg)
{
    LOG_T("Got GPIO_DMA IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_GPIO_DMA);
}

static void I2C0_IRQHandler(int irq, void *arg)
{
    LOG_T("Got I2C0 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_I2C0);
}

static void I2C1_IRQHandler(int irq, void *arg)
{
    LOG_T("Got I2C1 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_I2C1);
}

static void PWM_IRQHandler(int irq, void *arg)
{
    LOG_T("Got PWM IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_PWM);
}

static void TIMER0_CH0_IRQHandler(int irq, void *arg)
{
    LOG_T("Got TIMER0_CH0 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_TIMER0_CH0);
}

static void TIMER0_CH1_IRQHandler(int irq, void *arg)
{
    LOG_T("Got TIMER0_CH1 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_TIMER0_CH1);
}

static void TIMER0_WDT_IRQHandler(int irq, void *arg)
{
    LOG_T("Got TIMER0_WDT IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_TIMER0_WDT);
}

static void I2S_IRQHandler(int irq, void *arg)
{
    LOG_T("Got I2S IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_I2S);
}

static void PDS_WAKEUP_IRQHandler(int irq, void *arg)
{
    LOG_T("Got PDS IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_PDS_WAKEUP);
}

static void HBN_OUT0_IRQHandler(int irq, void *arg)
{
    LOG_T("Got HBN_OUT0 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_HBN_OUT0);
}

static void HBN_OUT1_IRQHandler(int irq, void *arg)
{
    LOG_T("Got HBN_OUT1 IRQ\r\n");
    Send_IPC_IRQ(BFLB_IPC_DEVICE_HBN_OUT1);
}

courier_irq_t nuttx_ipc_irqs[32] = {
    [BFLB_IPC_DEVICE_SDHCI] = { "SDH", SDH_IRQn, 0, SDHCI_IRQHandler},
    [BFLB_IPC_DEVICE_UART2] = { "UART2", UART2_IRQn, 0, UART2_IRQHandler},
    [BLFB_IPC_DEVICE_USB] = { "USB", USB_IRQn, 0, USB_IRQHandler},
    [BFLB_IPC_DEVICE_EMAC] = { "EMAC", EMAC_IRQn, 0, EMAC_IRQHandler},
    [BFLB_IPC_DEVICE_GPIO] = { "GPIO", GPIO_INT0_IRQn, 0, GPIO_IRQHandler},
    [BFLB_IPC_DEVICE_DMA0_ALL] = { "DMA0_ALL", DMA0_ALL_IRQn, 0, DMA0_ALL_IRQHandler},
    [BFLB_IPC_DEVICE_DMA1_ALL] = { "DMA1_ALL", DMA1_ALL_IRQn, 0, DMA1_ALL_IRQHandler},
    [BFLB_IPC_DEVICE_IRTX] = { "IRTX", IRTX_IRQn, 0, IRTX_IRQHandler},
    [BFLB_IPC_DEVICE_IRRX] = { "IRRX", IRRX_IRQn, 0, IRRX_IRQHandler},
    [BFLB_IPC_DEVICE_GPADC_DMA] = { "GPADC_DMA", GPADC_DMA_IRQn, 0, GPADC_DMA_IRQHandler},
    [BFLB_IPC_DEVICE_SPI0] = { "SPI0", SPI0_IRQn, 0, SPI0_IRQHandler},
    [BFLB_IPC_DEVICE_UART0] = { "UART0", UART0_IRQn, 0, UART0_IRQHandler},
    [BFLB_IPC_DEVICE_UART1] = { "UART1", UART1_IRQn, 0, UART1_IRQHandler},
    [BFLB_IPC_DEVICE_GPIO_DMA] = { "GPIO_DMA", GPIO_DMA_IRQn, 0, GPIO_DMA_IRQHandler},
    [BFLB_IPC_DEVICE_I2C0] = { "I2C0", I2C0_IRQn, 0, I2C0_IRQHandler},
    [BFLB_IPC_DEVICE_I2C1] = { "I2C1", I2C1_IRQn, 0, I2C1_IRQHandler},
    [BFLB_IPC_DEVICE_PWM] = { "PWM", PWM_IRQn, 0, PWM_IRQHandler},
    [BFLB_IPC_DEVICE_TIMER0_CH0] = { "TIMER0_CH0", TIMER0_CH0_IRQn, 0, TIMER0_CH0_IRQHandler},
    [BFLB_IPC_DEVICE_TIMER0_CH1] = { "TIMER0_CH1", TIMER0_CH1_IRQn, 0, TIMER0_CH1_IRQHandler},
    [BFLB_IPC_DEVICE_TIMER0_WDT] = { "TIMER0_WDT", TIMER0_WDT_IRQn, 0, TIMER0_WDT_IRQHandler},
    [BFLB_IPC_DEVICE_I2S] = { "I2S", I2S_IRQn, 0, I2S_IRQHandler},
    [BFLB_IPC_DEVICE_PDS_WAKEUP] = { "PDS_WAKEUP", PDS_WAKEUP_IRQn, 0, PDS_WAKEUP_IRQHandler},
    [BFLB_IPC_DEVICE_HBN_OUT0] = { "HBN_OUT0", HBN_OUT0_IRQn, 0, HBN_OUT0_IRQHandler},
    [BFLB_IPC_DEVICE_HBN_OUT1] = { "HBN_OUT1", HBN_OUT1_IRQn, 0, HBN_OUT1_IRQHandler},
};

static void Send_IPC_IRQ(int device)
{
    if (nuttx_ipc_irqs[device].irq == 0) {
        LOG_E("Invalid IPC IRQ %d\r\n", device);
        return;
    }
    LOG_I("Forward");
    bflb_irq_disable(nuttx_ipc_irqs[device].irq); // D0 will need to reenable
    // Bits 0-7 will indicate IRQ number. Bit 8 will set off the interrupt.
    BL_WR_REG(IPC2_BASE, IPC_CPU1_IPC_ISWR, nuttx_ipc_irqs[device].irq | (1 << IPC_INT_SIG_SHIFT));
    nuttx_ipc_irqs[device].count++;
}

void IPC_M0_IRQHandler(int irq, void *arg)
{
  uint32_t irqStatus = BL_RD_REG(IPC0_BASE, IPC_CPU0_IPC_IRSRR);
  int irqn = irqStatus & IPC_IRQn_MASK;
  bool enable = irqStatus & (1 << IPC_INT_ENABLE_SIG_SHIFT);
  if (enable) {
    LOG_I("Got enable signal for IRQn %d", irqn);
    bflb_irq_enable(irqn);
  } else {
    LOG_I("Got disable signal for IRQn %d", irqn);
    bflb_irq_disable(irqn);
  }

  BL_WR_REG(IPC0_BASE, IPC_CPU0_IPC_ICR, irqStatus);
}
  

oblfr_err_t setup_sdh_peripheral() {
    LOG_D("setting up SDH peripheral\r\n");
    struct bflb_device_s *gpio;

    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_0, GPIO_FUNC_SDH | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    bflb_gpio_init(gpio, GPIO_PIN_1, GPIO_FUNC_SDH | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    bflb_gpio_init(gpio, GPIO_PIN_2, GPIO_FUNC_SDH | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    bflb_gpio_init(gpio, GPIO_PIN_3, GPIO_FUNC_SDH | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    bflb_gpio_init(gpio, GPIO_PIN_4, GPIO_FUNC_SDH | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    bflb_gpio_init(gpio, GPIO_PIN_5, GPIO_FUNC_SDH | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);

    PERIPHERAL_CLOCK_SDH_ENABLE();
    LOG_D("SDH peripheral clock: %d\r\n", bflb_clk_get_peripheral_clock(BFLB_DEVICE_TYPE_SDH, 0)/1000000);
    return OBLFR_OK;
}

static oblfr_err_t setup_emac_peripheral(void)
{
    GLB_GPIO_Cfg_Type gpio_cfg;
    int pin;

    PERIPHERAL_CLOCK_EMAC_ENABLE();

    struct bflb_device_s *gpio = bflb_device_get_by_name("gpio");
    for (pin = GLB_GPIO_PIN_24; pin <= GLB_GPIO_PIN_33; pin++) {
        bflb_gpio_init(gpio, pin, GPIO_FUNC_EMAC | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    }
    return OBLFR_OK;
}

oblfr_err_t oblfr_courier_init()
{
    int i;

    /* setup the IPC Interupt */
    bflb_irq_attach(IPC_M0_IRQn, IPC_M0_IRQHandler, NULL);
    BL_WR_REG(IPC0_BASE, IPC_CPU0_IPC_IUSR, (1 << IPC_INT_SIG_SHIFT));
    bflb_irq_enable(IPC_M0_IRQn);

    /* register our Interupt Handlers to forward over */

    for (i = 0; i < sizeof(nuttx_ipc_irqs) / sizeof(nuttx_ipc_irqs[0]); i++) {
        if (nuttx_ipc_irqs[i].irq != 0) {
            LOG_I("Forwarding Interupt %s (%d) to D0 (%p)\r\n", nuttx_ipc_irqs[i].name, nuttx_ipc_irqs[i].irq, nuttx_ipc_irqs[i].handler);
            bflb_irq_attach(nuttx_ipc_irqs[i].irq, nuttx_ipc_irqs[i].handler, NULL);
            // Not enabling irqs yet - bflb_irq_enable(nuttx_ipc_irqs[i].irq);
        }
    }

    if (setup_sdh_peripheral() != SUCCESS) {
        LOG_E("Failed to setup SDH peripheral\r\n");
        return OBLFR_ERR_ERROR;
    }

    if (setup_emac_peripheral() != SUCCESS) {
        LOG_E("Failed to setup EMAC peripheral\r\n");
        return OBLFR_ERR_ERROR;
    }

    if (setup_usb_peripheral() != SUCCESS) {
        LOG_E("Failed to setup USB peripheral\r\n");
        return OBLFR_ERR_ERROR;
    }
    
    return OBLFR_OK;
}
