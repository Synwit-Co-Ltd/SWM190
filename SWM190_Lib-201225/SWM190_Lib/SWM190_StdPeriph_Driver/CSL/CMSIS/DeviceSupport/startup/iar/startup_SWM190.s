;******************************************************************************************************************************************
; 文件名称:    startup_SWM190.s
; 功能说明:    SWM190单片机的启动文件
; 技术支持:    http://www.synwit.com.cn/e/tool/gbook/?bid=1
; 注意事项:
; 版本日期: V1.0.0        2016年1月30日
; 升级记录:
;
;
;******************************************************************************************************************************************
; @attention
;
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
; REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
; FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
; OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
; -ECTION WITH THEIR PRODUCTS.
;
; COPYRIGHT 2012 Synwit Technology
;******************************************************************************************************************************************

; Memory Size Config
;      Cache    Code Memory     Data Memory
; 0     On         120k             20k
; 2     Off         32k              4k
MEM_CONFIG    EQU    2



        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler              ; Reset Handler
        DCD     NMI_Handler                ; NMI Handler
        DCD     HardFault_Handler          ; Hard Fault Handler
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     MEM_CONFIG
        DCD     SVC_Handler                ; SVCall Handler
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     PendSV_Handler             ; PendSV Handler
        DCD     SysTick_Handler            ; SysTick Handler

        ; External Interrupts
        DCD    GPIOA_Handler          
        DCD    GPIOA0_Handler         
        DCD    UART0_Handler          
        DCD    UART1_Handler          
        DCD    SPI0_Handler           
        DCD    TIMR0_Handler          
        DCD    DMA_Handler            
        DCD    PWM0_Handler           
        DCD    BTIMR0_Handler          
        DCD    BTIMR3_Handler          
        DCD    TIMR1_Handler          
        DCD    WDT_Handler            
        DCD    GPIOA4_Handler         
        DCD    I2C0_Handler           
        DCD    GPIOA5_Handler         
        DCD    BTIMR2_Handler          
        DCD    GPIOC_CORDIC_Handler   
        DCD    SPI1_BTIMR1_Handler     
        DCD    GPIOB_BOD_Handler      
        DCD    GPIOC4_ADC0_Handler    
        DCD    GPIOD_RTC_Handler      
        DCD    XTAL_ACMP_Handler       
        DCD    UART2_GPIOB4_Handler   
        DCD    TIMR2_GPIOA15_Handler  
        DCD    TIMR3_HALL_Handler     
        DCD    GPIOC5_I2C1_Handler    
        DCD    PWM1_GPIOB6_Handler    
        DCD    UART3_GPIOB5_Handler   
        DCD    PWM2_SFC_Handler     
        DCD    PWM3_CACHE_Handler     
        DCD    ADC1_GPIOA14_Handler   
        DCD    GPIOE_PWMHALT_Handler 
        
        ; Switch QUAL
        DCD		0x681823f8
        DCD		0x07c96881
        DCD		0x2101d1fc
        DCD		0x31260309
        DCD		0x21016001
        DCD		0x024a6241
        DCD		0x614161c2
        DCD		0x07c96941
        DCD		0x2141d1fc
        DCD		0x60010149
        DCD		0x6202685A
        DCD		0x02c92101
        DCD		0x47706001
        DCD		0x00000000
        DCD		0x4004a000
        DCD		0xeb050206


        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        MOVS    R0, #0x40		; 0x4000008C
        LSLS    R0, R0, #24
        ADDS    R0, R0, #0x8C
        LDR     R1, [R0, #0]
        MOVS    R0, #0x85		; 0x854012
        LSLS    R0, R0, #8
        ADDS    R0, R0, #0x40
        LSLS    R0, R0, #8
        ADDS    R0, R0, #0x12
        CMP     R0, R1
        BNE     qspi_done
        MOVS    R0, #0x40		; 0x4004A000
        LSLS    R0, R0, #8
        ADDS    R0, R0, #0x04
        LSLS    R0, R0, #8
        ADDS    R0, R0, #0xA0
        LSLS    R0, R0, #8
        LDR     R1, [R0, #0]
        LSRS    R0, R1, #10
        MOVS    R1, #3
        ANDS    R0, R0, R1
        CMP     R0, #2
        BEQ     qspi_done
        MOVS	 R0, #0xC1
        BLX	 R0
qspi_done
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler


        PUBWEAK GPIOA_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA_Handler
        B GPIOA_Handler

        PUBWEAK GPIOA0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA0_Handler
        B GPIOA0_Handler

        PUBWEAK UART0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_Handler
        B UART0_Handler

        PUBWEAK UART1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_Handler
        B UART1_Handler

        PUBWEAK SPI0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI0_Handler
        B SPI0_Handler

        PUBWEAK TIMR0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR0_Handler
        B TIMR0_Handler

        PUBWEAK DMA_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA_Handler
        B DMA_Handler

        PUBWEAK PWM0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM0_Handler
        B PWM0_Handler

        PUBWEAK BTIMR0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BTIMR0_Handler
        B BTIMR0_Handler

        PUBWEAK BTIMR3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BTIMR3_Handler
        B BTIMR3_Handler

        PUBWEAK TIMR1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR1_Handler
        B TIMR1_Handler

        PUBWEAK WDT_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT_Handler
        B WDT_Handler

        PUBWEAK GPIOA4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA4_Handler
        B GPIOA4_Handler

        PUBWEAK I2C0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_Handler
        B I2C0_Handler

        PUBWEAK GPIOA5_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA5_Handler
        B GPIOA5_Handler

        PUBWEAK BTIMR2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BTIMR2_Handler
        B BTIMR2_Handler

        PUBWEAK GPIOC_CORDIC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOC_CORDIC_Handler
        B GPIOC_CORDIC_Handler

        PUBWEAK SPI1_BTIMR1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI1_BTIMR1_Handler
        B SPI1_BTIMR1_Handler

        PUBWEAK GPIOB_BOD_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOB_BOD_Handler
        B GPIOB_BOD_Handler

        PUBWEAK GPIOC4_ADC0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOC4_ADC0_Handler
        B GPIOC4_ADC0_Handler

        PUBWEAK GPIOD_RTC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOD_RTC_Handler
        B GPIOD_RTC_Handler

        PUBWEAK XTAL_ACMP_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
XTAL_ACMP_Handler
        B XTAL_ACMP_Handler

        PUBWEAK UART2_GPIOB4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART2_GPIOB4_Handler
        B UART2_GPIOB4_Handler

        PUBWEAK TIMR2_GPIOA15_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR2_GPIOA15_Handler
        B TIMR2_GPIOA15_Handler

        PUBWEAK TIMR3_HALL_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR3_HALL_Handler
        B TIMR3_HALL_Handler

        PUBWEAK GPIOC5_I2C1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOC5_I2C1_Handler
        B GPIOC5_I2C1_Handler

        PUBWEAK PWM1_GPIOB6_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM1_GPIOB6_Handler
        B PWM1_GPIOB6_Handler

        PUBWEAK UART3_GPIOB5_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART3_GPIOB5_Handler
        B UART3_GPIOB5_Handler

        PUBWEAK PWM2_SFC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM2_SFC_Handler
        B PWM2_SFC_Handler

        PUBWEAK PWM3_CACHE_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM3_CACHE_Handler
        B PWM3_CACHE_Handler

        PUBWEAK ADC1_GPIOA14_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC1_GPIOA14_Handler
        B ADC1_GPIOA14_Handler

        PUBWEAK GPIOE_PWMHALT_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOE_PWMHALT_Handler
        B GPIOE_PWMHALT_Handler

        END
