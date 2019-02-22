/*****************************************************************************************
* @file               : startup.c
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Source files for startup.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/

/*macros --------------------------------------------------------------------------------*/
#define WEAK       __attribute__ ((weak))
#define SECTION(x) __attribute__ ((section(x)))

/*typedefs ------------------------------------------------------------------------------*/
typedef void (*vector_table_entry_t)(void);

/*variables -----------------------------------------------------------------------------*/
/* provided by the linker script */
extern unsigned long _etext;  /* start address of the static initialization data */
extern unsigned long _sidata; /* start address of the static initialization data */
extern unsigned long _sdata;  /* start address of the data section */
extern unsigned long _edata;  /* end address of the data section */
extern unsigned long _sbss;   /* start address of the bss section */
extern unsigned long _ebss;   /* end address of the bss section */
extern unsigned long _estack; /* end address of the stack section */

/*prototypes ----------------------------------------------------------------------------*/


/*private -------------------------------------------------------------------------------*/


/*public --------------------------------------------------------------------------------*/

int main(void);

WEAK void Reset_Handler(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void MemManage_Handler(void);
WEAK void BusFault_Handler(void);
WEAK void UsageFault_Handler(void);
WEAK void MemManage_Handler(void);
WEAK void SVC_Handler(void);
WEAK void DebugMon_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void WWDG_IRQHandler(void);
WEAK void PVD_IRQHandler(void);
WEAK void TAMPER_IRQHandler(void);
WEAK void RTC_IRQHandler(void);
WEAK void FLASH_IRQHandler(void);
WEAK void RCC_IRQHandler(void);
WEAK void EXTI0_IRQHandler(void);
WEAK void EXTI1_IRQHandler(void);
WEAK void EXTI2_IRQHandler(void);
WEAK void EXTI3_IRQHandler(void);
WEAK void EXTI4_IRQHandler(void);
WEAK void DMA1_Channel1_IRQHandler(void);
WEAK void DMA1_Channel2_IRQHandler(void);
WEAK void DMA1_Channel3_IRQHandler(void);
WEAK void DMA1_Channel4_IRQHandler(void);
WEAK void DMA1_Channel5_IRQHandler(void);
WEAK void DMA1_Channel6_IRQHandler(void);
WEAK void DMA1_Channel7_IRQHandler(void);
WEAK void ADC1_2_IRQHandler(void);
WEAK void USB_HP_CAN1_TX_IRQHandler(void);
WEAK void USB_LP_CAN1_RX0_IRQHandler(void);
WEAK void CAN1_RX1_IRQHandler(void);
WEAK void CAN1_SCE_IRQHandler(void);
WEAK void EXTI9_5_IRQHandler(void);
WEAK void TIM1_BRK_IRQHandler(void);
WEAK void TIM1_UP_IRQHandler(void);
WEAK void TIM1_TRG_COM_IRQHandler(void);
WEAK void TIM1_CC_IRQHandler(void);
WEAK void TIM2_IRQHandler(void);
WEAK void TIM3_IRQHandler(void);
WEAK void TIM4_IRQHandler(void);
WEAK void I2C1_EV_IRQHandler(void);
WEAK void I2C1_ER_IRQHandler(void);
WEAK void I2C2_EV_IRQHandler(void);
WEAK void I2C2_ER_IRQHandler(void);
WEAK void SPI1_IRQHandler(void);
WEAK void SPI2_IRQHandler(void);
WEAK void USART1_IRQHandler(void);
WEAK void USART2_IRQHandler(void);
WEAK void USART3_IRQHandler(void);
WEAK void EXTI15_10_IRQHandler(void);
WEAK void RTCAlarm_IRQHandler(void);
WEAK void USBWakeUp_IRQHandler(void);
WEAK void TIM8_BRK_IRQHandler(void);
WEAK void TIM8_UP_IRQHandler(void);
WEAK void TIM8_TRG_COM_IRQHandler(void);
WEAK void TIM8_CC_IRQHandler(void);
WEAK void ADC3_IRQHandler(void);
WEAK void FSMC_IRQHandler(void);
WEAK void SDIO_IRQHandler(void);
WEAK void TIM5_IRQHandler(void);
WEAK void SPI3_IRQHandler(void);
WEAK void UART4_IRQHandler(void);
WEAK void UART5_IRQHandler(void);
WEAK void TIM6_IRQHandler(void);
WEAK void TIM7_IRQHandler(void);
WEAK void DMA2_Channel1_IRQHandler(void);
WEAK void DMA2_Channel2_IRQHandler(void);
WEAK void DMA2_Channel3_IRQHandler(void);
WEAK void DMA2_Channel4_5_IRQHandler(void);

void pre_main(void)
{
	unsigned long *src = &_sidata;
	unsigned long *dst = &_sdata;
	/* copy the data segment into ram */
	if (src != dst)	{
		while(dst < &_edata){
			*dst++ = *src++;
		}
	}
	/* zero the bss segment */
	dst = &_sbss;
	while(dst < &_ebss) {
		*dst++ = 0;
	}
}

void Reset_Handler(void) {

	pre_main();                         /* Initialize memory, data and bss */
	
	(void)main();                       /* start execution of the program */
	
	while(1);
}

SECTION(".vectors")
vector_table_entry_t vector_table[] = {
 (vector_table_entry_t)&_estack,        /* The stack pointer after relocation */
 /********************** Cortex-M Exception Handlers *********************/
 Reset_Handler,                         /* Reset Handler */
 NMI_Handler,                           /* NMI Handler */
 HardFault_Handler,                     /* Hard Fault Handler */
 MemManage_Handler,                     /* MPU Fault Handler */
 BusFault_Handler,                      /* Bus Fault Handler */
 UsageFault_Handler,                    /* Usage Fault Handler */
 0, 0, 0, 0,                            /* Reserved */
 SVC_Handler,                           /* SVCall Handler */
 DebugMon_Handler,                      /* Debug Monitor Handler */
 0,                                     /* Reserved */
 PendSV_Handler,                        /* PendSV Handler */
 SysTick_Handler,                       /* SysTick Handler */
/************************** External Interrupts **************************/
 WWDG_IRQHandler,                       /* Window Watchdog */
 PVD_IRQHandler,                        /* PVD through EXTI Line detect */
 TAMPER_IRQHandler,                     /* Tamper */
 RTC_IRQHandler,                        /* RTC */
 FLASH_IRQHandler,                      /* Flash */
 RCC_IRQHandler,                        /* RCC */
 EXTI0_IRQHandler,                      /* EXTI Line 0 */
 EXTI1_IRQHandler,                      /* EXTI Line 1 */
 EXTI2_IRQHandler,                      /* EXTI Line 2 */
 EXTI3_IRQHandler,                      /* EXTI Line 3 */
 EXTI4_IRQHandler,                      /* EXTI Line 4 */
 DMA1_Channel1_IRQHandler,              /* DMA1 Channel 1 */
 DMA1_Channel2_IRQHandler,              /* DMA1 Channel 2 */
 DMA1_Channel3_IRQHandler,              /* DMA1 Channel 3 */
 DMA1_Channel4_IRQHandler,              /* DMA1 Channel 4 */
 DMA1_Channel5_IRQHandler,              /* DMA1 Channel 5 */
 DMA1_Channel6_IRQHandler,              /* DMA1 Channel 6 */
 DMA1_Channel7_IRQHandler,              /* DMA1 Channel 7 */
 ADC1_2_IRQHandler,                     /* ADC1 & ADC2 */
 USB_HP_CAN1_TX_IRQHandler,             /* USB High Priority or CAN1 TX */
 USB_LP_CAN1_RX0_IRQHandler,            /* USB Low Priority or CAN1 RX0 */
 CAN1_RX1_IRQHandler,                   /* CAN1 RX1 */
 CAN1_SCE_IRQHandler,                   /* CAN1 SCE */
 EXTI9_5_IRQHandler,                    /* EXTI Line 9..5 */
 TIM1_BRK_IRQHandler,                   /* TIM1 Break */
 TIM1_UP_IRQHandler,                    /* TIM1 Update */
 TIM1_TRG_COM_IRQHandler,               /* TIM1 Trigger and Commutation */
 TIM1_CC_IRQHandler,                    /* TIM1 Capture Compare */
 TIM2_IRQHandler,                       /* TIM2 */
 TIM3_IRQHandler,                       /* TIM3 */
 TIM4_IRQHandler,                       /* TIM4 */
 I2C1_EV_IRQHandler,                    /* I2C1 Event */
 I2C1_ER_IRQHandler,                    /* I2C1 Error */
 I2C2_EV_IRQHandler,                    /* I2C2 Event */
 I2C2_ER_IRQHandler,                    /* I2C2 Error */
 SPI1_IRQHandler,                       /* SPI1 */
 SPI2_IRQHandler,                       /* SPI2 */
 USART1_IRQHandler,                     /* USART1 */
 USART2_IRQHandler,                     /* USART2 */
 USART3_IRQHandler,                     /* USART3 */
 EXTI15_10_IRQHandler,                  /* EXTI Line 15..10 */
 RTCAlarm_IRQHandler,                   /* RTC Alarm through EXTI Line */
 USBWakeUp_IRQHandler,                  /* USB Wakeup from suspend */
 TIM8_BRK_IRQHandler,                   /* TIM8 Break */
 TIM8_UP_IRQHandler,                    /* TIM8 Update */
 TIM8_TRG_COM_IRQHandler,               /* TIM8 Trigger and Commutation */
 TIM8_CC_IRQHandler,                    /* TIM8 Capture Compare */
 ADC3_IRQHandler,                       /* ADC3 */
 FSMC_IRQHandler,                       /* FSMC */
 SDIO_IRQHandler,                       /* SDIO */
 TIM5_IRQHandler,                       /* TIM5 */
 SPI3_IRQHandler,                       /* SPI3 */
 UART4_IRQHandler,                      /* UART4 */
 UART5_IRQHandler,                      /* UART5 */
 TIM6_IRQHandler,                       /* TIM6 */
 TIM7_IRQHandler,                       /* TIM7 */
 DMA2_Channel1_IRQHandler,              /* DMA2 Channel 1 */
 DMA2_Channel2_IRQHandler,              /* DMA2 Channel 2 */
 DMA2_Channel3_IRQHandler,              /* DMA2 Channel 3 */
 DMA2_Channel4_5_IRQHandler,            /* DMA2 Channel 4..5 */
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0,
 /********************************* End *********************************/
 (vector_table_entry_t)0xF1E0F85F       /* @0x1E0. This is for boot in 
                                           RAM mode for STM32F10x High 
                                           Density devices. */
};


void Default_Handler(void)
{
	while (1);
}

#pragma weak NMI_Handler                = Default_Handler
#pragma weak MemManage_Handler          = Default_Handler
#pragma weak BusFault_Handler           = Default_Handler
#pragma weak UsageFault_Handler         = Default_Handler
#pragma weak SVC_Handler                = Default_Handler
#pragma weak DebugMon_Handler           = Default_Handler
#pragma weak PendSV_Handler             = Default_Handler
#pragma weak SysTick_Handler            = Default_Handler
#pragma weak WWDG_IRQHandler            = Default_Handler
#pragma weak PVD_IRQHandler             = Default_Handler
#pragma weak TAMPER_IRQHandler          = Default_Handler
#pragma weak RTC_IRQHandler             = Default_Handler
#pragma weak FLASH_IRQHandler           = Default_Handler
#pragma weak RCC_IRQHandler             = Default_Handler
#pragma weak EXTI0_IRQHandler           = Default_Handler
#pragma weak EXTI1_IRQHandler           = Default_Handler
#pragma weak EXTI2_IRQHandler           = Default_Handler
#pragma weak EXTI3_IRQHandler           = Default_Handler
#pragma weak EXTI4_IRQHandler           = Default_Handler
#pragma weak DMA1_Channel1_IRQHandler   = Default_Handler
#pragma weak DMA1_Channel2_IRQHandler   = Default_Handler
#pragma weak DMA1_Channel3_IRQHandler   = Default_Handler
#pragma weak DMA1_Channel4_IRQHandler   = Default_Handler
#pragma weak DMA1_Channel5_IRQHandler   = Default_Handler
#pragma weak DMA1_Channel6_IRQHandler   = Default_Handler
#pragma weak DMA1_Channel7_IRQHandler   = Default_Handler
#pragma weak ADC1_2_IRQHandler          = Default_Handler
#pragma weak USB_HP_CAN1_TX_IRQHandler  = Default_Handler
#pragma weak USB_LP_CAN1_RX0_IRQHandler = Default_Handler
#pragma weak CAN1_RX1_IRQHandler        = Default_Handler
#pragma weak CAN1_SCE_IRQHandler        = Default_Handler
#pragma weak EXTI9_5_IRQHandler         = Default_Handler
#pragma weak TIM1_BRK_IRQHandler        = Default_Handler
#pragma weak TIM1_UP_IRQHandler         = Default_Handler
#pragma weak TIM1_TRG_COM_IRQHandler    = Default_Handler
#pragma weak TIM1_CC_IRQHandler         = Default_Handler
#pragma weak TIM2_IRQHandler            = Default_Handler
#pragma weak TIM3_IRQHandler            = Default_Handler
#pragma weak TIM4_IRQHandler            = Default_Handler
#pragma weak I2C1_EV_IRQHandler         = Default_Handler
#pragma weak I2C1_ER_IRQHandler         = Default_Handler
#pragma weak I2C2_EV_IRQHandler         = Default_Handler
#pragma weak I2C2_ER_IRQHandler         = Default_Handler
#pragma weak SPI1_IRQHandler            = Default_Handler
#pragma weak SPI2_IRQHandler            = Default_Handler
#pragma weak USART1_IRQHandler          = Default_Handler
#pragma weak USART2_IRQHandler          = Default_Handler
#pragma weak USART3_IRQHandler          = Default_Handler
#pragma weak EXTI15_10_IRQHandler       = Default_Handler
#pragma weak RTCAlarm_IRQHandler        = Default_Handler
#pragma weak USBWakeUp_IRQHandler       = Default_Handler
#pragma weak TIM8_BRK_IRQHandler        = Default_Handler
#pragma weak TIM8_UP_IRQHandler         = Default_Handler
#pragma weak TIM8_TRG_COM_IRQHandler    = Default_Handler
#pragma weak TIM8_CC_IRQHandler         = Default_Handler
#pragma weak ADC3_IRQHandler            = Default_Handler
#pragma weak FSMC_IRQHandler            = Default_Handler
#pragma weak SDIO_IRQHandler            = Default_Handler
#pragma weak TIM5_IRQHandler            = Default_Handler
#pragma weak SPI3_IRQHandler            = Default_Handler
#pragma weak UART4_IRQHandler           = Default_Handler
#pragma weak UART5_IRQHandler           = Default_Handler
#pragma weak TIM6_IRQHandler            = Default_Handler
#pragma weak TIM7_IRQHandler            = Default_Handler
#pragma weak DMA2_Channel1_IRQHandler   = Default_Handler
#pragma weak DMA2_Channel2_IRQHandler   = Default_Handler
#pragma weak DMA2_Channel3_IRQHandler   = Default_Handler
#pragma weak DMA2_Channel4_5_IRQHandler = Default_Handler

