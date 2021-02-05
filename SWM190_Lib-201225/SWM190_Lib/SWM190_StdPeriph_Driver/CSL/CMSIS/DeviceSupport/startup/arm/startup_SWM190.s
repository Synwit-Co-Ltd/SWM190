;******************************************************************************************************************************************
; 文件名称:	startup_SWM182.s
; 功能说明:	SWM182单片机的启动文件
; 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
; 注意事项:
; 版本日期: V1.0.0		2016年1月30日
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
; 	   Cache	Code Memory     Data Memory
; 0		On		   120k				20k
; 2     Off			32k				 4k
MEM_CONFIG	  EQU    2


; Amount of memory (in bytes) allocated for Stack
Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; Amount of memory (in bytes) allocated for Heap
Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp               ; Top of Stack
                DCD    Reset_Handler              ; Reset Handler
                DCD    NMI_Handler                ; NMI Handler
                DCD    HardFault_Handler          ; Hard Fault Handler
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    MEM_CONFIG
                DCD    SVC_Handler                ; SVCall Handler
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    PendSV_Handler             ; PendSV Handler
                DCD    SysTick_Handler            ; SysTick Handler

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

__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors



                AREA    |.text|, CODE, READONLY

Reset_Handler    PROC
                 EXPORT  Reset_Handler          [WEAK]
				 IMPORT  __main
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
				 LDR     R0, =__main
                 BX      R0
				 
                 ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler             [WEAK]
                B       .
                ENDP

HardFault_Handler PROC
                EXPORT  HardFault_Handler       [WEAK]
                B       .
                ENDP
				
SVC_Handler     PROC
                EXPORT  SVC_Handler             [WEAK]
                B       .
                ENDP

PendSV_Handler  PROC
                EXPORT  PendSV_Handler          [WEAK]
                B       .
                ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler         [WEAK]
                B       .
                ENDP

GPIOA_Handler           PROC
                EXPORT  GPIOA_Handler           [WEAK]
                B       .
                ENDP

GPIOA0_Handler          PROC
                EXPORT  GPIOA0_Handler          [WEAK]
                B       .
                ENDP

UART0_Handler           PROC
                EXPORT  UART0_Handler           [WEAK]
                B       .
                ENDP

UART1_Handler           PROC
                EXPORT  UART1_Handler           [WEAK]
                B       .
                ENDP

SPI0_Handler            PROC
                EXPORT  SPI0_Handler            [WEAK]
                B       .
                ENDP

TIMR0_Handler           PROC
                EXPORT  TIMR0_Handler           [WEAK]
                B       .
                ENDP

DMA_Handler             PROC
                EXPORT  DMA_Handler             [WEAK]
                B       .
                ENDP

PWM0_Handler            PROC
                EXPORT  PWM0_Handler            [WEAK]
                B       .
                ENDP

BTIMR0_Handler          PROC
                EXPORT  BTIMR0_Handler          [WEAK]
                B       .
                ENDP

BTIMR3_Handler          PROC
                EXPORT  BTIMR3_Handler          [WEAK]
                B       .
                ENDP

TIMR1_Handler           PROC
                EXPORT  TIMR1_Handler           [WEAK]
                B       .
                ENDP

WDT_Handler             PROC
                EXPORT  WDT_Handler             [WEAK]
                B       .
                ENDP

GPIOA4_Handler          PROC
                EXPORT  GPIOA4_Handler          [WEAK]
                B       .
                ENDP

I2C0_Handler            PROC
                EXPORT  I2C0_Handler            [WEAK]
                B       .
                ENDP

GPIOA5_Handler          PROC
                EXPORT  GPIOA5_Handler          [WEAK]
                B       .
                ENDP

BTIMR2_Handler          PROC
                EXPORT  BTIMR2_Handler          [WEAK]
                B       .
                ENDP

GPIOC_CORDIC_Handler    PROC
                EXPORT  GPIOC_CORDIC_Handler    [WEAK]
                B       .
                ENDP

SPI1_BTIMR1_Handler     PROC
                EXPORT  SPI1_BTIMR1_Handler     [WEAK]
                B       .
                ENDP

GPIOB_BOD_Handler       PROC
                EXPORT  GPIOB_BOD_Handler       [WEAK]
                B       .
                ENDP

GPIOC4_ADC0_Handler     PROC
                EXPORT  GPIOC4_ADC0_Handler     [WEAK]
                B       .
                ENDP

GPIOD_RTC_Handler       PROC
                EXPORT  GPIOD_RTC_Handler       [WEAK]
                B       .
                ENDP

XTAL_ACMP_Handler       PROC
                EXPORT  XTAL_ACMP_Handler        [WEAK]
                B       .
                ENDP

UART2_GPIOB4_Handler    PROC
                EXPORT  UART2_GPIOB4_Handler    [WEAK]
                B       .
                ENDP

TIMR2_GPIOA15_Handler   PROC
                EXPORT  TIMR2_GPIOA15_Handler   [WEAK]
                B       .
                ENDP

TIMR3_HALL_Handler      PROC
                EXPORT  TIMR3_HALL_Handler      [WEAK]
                B       .
                ENDP

GPIOC5_I2C1_Handler     PROC
                EXPORT  GPIOC5_I2C1_Handler     [WEAK]
                B       .
                ENDP

PWM1_GPIOB6_Handler     PROC
                EXPORT  PWM1_GPIOB6_Handler     [WEAK]
                B       .
                ENDP

UART3_GPIOB5_Handler    PROC
                EXPORT  UART3_GPIOB5_Handler    [WEAK]
                B       .
                ENDP

PWM2_SFC_Handler        PROC
                EXPORT  PWM2_SFC_Handler      [WEAK]
                B       .
                ENDP

PWM3_CACHE_Handler      PROC
                EXPORT  PWM3_CACHE_Handler      [WEAK]
                B       .
                ENDP

ADC1_GPIOA14_Handler    PROC
                EXPORT  ADC1_GPIOA14_Handler    [WEAK]
                B       .
                ENDP

GPIOE_PWMHALT_Handler   PROC
                EXPORT  GPIOE_PWMHALT_Handler   [WEAK]
                B       .
                ENDP

                ALIGN


;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

                END
