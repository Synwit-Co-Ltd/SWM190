/* Memory Size Config
    	Cache    Code Size	  Data Size
    0	On         120k          20k
    2	Off         32k           4k		*/
    .equ MEM_CONFIG, 2


    .syntax unified
    .arch armv6-m

/* Memory Model
   The HEAP starts at the end of the DATA section and grows upward.
   
   The STACK starts at the end of the RAM and grows downward     */
    .section .stack
    .align 3
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    0x400
__StackTop:


    .section .heap
    .align 3
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .space    0x000
__HeapLimit:


    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            
    .long    Reset_Handler         
    .long    NMI_Handler          
    .long    HardFault_Handler     
    .long    0     
    .long    0      
    .long    0   
    .long    0                    
    .long    0                    
    .long    0                    
    .long    MEM_CONFIG
    .long    SVC_Handler          
    .long    0     
    .long    0                     
    .long    PendSV_Handler           
    .long    SysTick_Handler         

    /* External interrupts */
	.long 	  GPIOA_Handler          
	.long 	  GPIOA0_Handler         
	.long 	  UART0_Handler          
	.long 	  UART1_Handler          
	.long 	  SPI0_Handler           
	.long 	  TIMR0_Handler          
	.long 	  DMA_Handler            
	.long 	  PWM0_Handler           
	.long 	  BTIMR0_Handler          
	.long 	  BTIMR3_Handler          
	.long 	  TIMR1_Handler          
	.long 	  WDT_Handler            
	.long 	  GPIOA4_Handler         
	.long 	  I2C0_Handler           
	.long 	  GPIOA5_Handler         
	.long 	  BTIMR2_Handler          
	.long 	  GPIOC_CORDIC_Handler   
	.long 	  SPI1_BTIMR1_Handler     
	.long 	  GPIOB_BOD_Handler      
	.long 	  GPIOC4_ADC0_Handler    
	.long 	  GPIOD_RTC_Handler      
	.long 	  XTAL_ACMP_Handler       
	.long 	  UART2_GPIOB4_Handler   
	.long 	  TIMR2_GPIOA15_Handler  
	.long 	  TIMR3_HALL_Handler     
	.long 	  GPIOC5_I2C1_Handler    
	.long 	  PWM1_GPIOB6_Handler    
	.long 	  UART3_GPIOB5_Handler   
	.long 	  PWM2_SFC_Handler     
	.long 	  PWM3_CACHE_Handler     
	.long 	  ADC1_GPIOA14_Handler   
	.long 	  GPIOE_PWMHALT_Handler

	/* Switch QUAL */
	.long 	  0x681823f8
	.long 	  0x07c96881
	.long 	  0x2101d1fc
	.long 	  0x31260309
	.long 	  0x21016001
	.long 	  0x024a6241
	.long 	  0x614161c2
	.long 	  0x07c96941
	.long 	  0x2141d1fc
	.long 	  0x60010149
	.long 	  0x6202685A
	.long 	  0x02c92101
	.long 	  0x47706001
	.long 	  0x00000000
	.long 	  0x4004a000
	.long 	  0xeb050206

	.section .text.Reset_Handler
    .align 2
    .thumb
    .globl    Reset_Handler
    .type     Reset_Handler, %function
Reset_Handler:
	MOVS    R0, #0x40		// 0x4000008C
	LSLS    R0, R0, #24
	ADDS    R0, R0, #0x8C
	LDR     R1, [R0, #0]
	MOVS    R0, #0x85		// 0x854012
	LSLS    R0, R0, #8
	ADDS    R0, R0, #0x40
	LSLS    R0, R0, #8
	ADDS    R0, R0, #0x12
	CMP     R0, R1
	BNE     qspi_done
	MOVS    R0, #0x40		// 0x4004A000
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
	MOVS	R0, #0xC1
	BLX	 R0
qspi_done:

/* Loop to copy data from read only memory to RAM. The ranges
 * of copy from/to are specified by symbols evaluated in linker script.  */
    ldr    r1, =__data_load__
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs   r3, r2
    ble .Lflash_to_ram_done

.Lflash_to_ram_loop:
    subs r3, #4
    ldr r0, [r1, r3]
    str r0, [r2, r3]
    bgt .Lflash_to_ram_loop
.Lflash_to_ram_done:


    ldr    r2, =__bss_start__
    ldr    r3, =__bss_end__

    subs   r3, r2
    ble .Lbss_to_ram_done

    movs    r0, 0
.Lbss_to_ram_loop:
    subs r3, #4
    str r0, [r2, r3]
    bgt .Lbss_to_ram_loop
.Lbss_to_ram_done:

    ldr    r0, =main
    bx     r0
    .pool    


    .text
/* Macro to define default handlers. 
   Default handler will be weak symbol and just dead loops. */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .endm

    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler

	def_default_handler    GPIOA_Handler          
	def_default_handler    GPIOA0_Handler         
	def_default_handler    UART0_Handler          
	def_default_handler    UART1_Handler          
	def_default_handler    SPI0_Handler           
	def_default_handler    TIMR0_Handler          
	def_default_handler    DMA_Handler            
	def_default_handler    PWM0_Handler           
	def_default_handler    BTIMR0_Handler          
	def_default_handler    BTIMR3_Handler          
	def_default_handler    TIMR1_Handler          
	def_default_handler    WDT_Handler            
	def_default_handler    GPIOA4_Handler         
	def_default_handler    I2C0_Handler           
	def_default_handler    GPIOA5_Handler         
	def_default_handler    BTIMR2_Handler          
	def_default_handler    GPIOC_CORDIC_Handler   
	def_default_handler    SPI1_BTIMR1_Handler     
	def_default_handler    GPIOB_BOD_Handler      
	def_default_handler    GPIOC4_ADC0_Handler    
	def_default_handler    GPIOD_RTC_Handler      
	def_default_handler    XTAL_ACMP_Handler       
	def_default_handler    UART2_GPIOB4_Handler   
	def_default_handler    TIMR2_GPIOA15_Handler  
	def_default_handler    TIMR3_HALL_Handler     
	def_default_handler    GPIOC5_I2C1_Handler    
	def_default_handler    PWM1_GPIOB6_Handler    
	def_default_handler    UART3_GPIOB5_Handler   
	def_default_handler    PWM2_SFC_Handler     
	def_default_handler    PWM3_CACHE_Handler     
	def_default_handler    ADC1_GPIOA14_Handler   
	def_default_handler    GPIOE_PWMHALT_Handler  

    def_default_handler    Default_Handler

    .end
