*   Is From :

https://gist.github.com/benpeoples/3aa57bffc0f26ede6623ca520f26628c

Per this forum post: https://esp32.com/viewtopic.php?t=6701
The way to calculate max PWM frequency is  integer (log 2 (LEDC_APB_CLK / frequency))

With a LEDC_APB_CLK == 80MHz, these are the following maximum values, in Hz:

LEDC_TIMER_1_BIT, 40000000
LEDC_TIMER_2_BIT, 20000000      
LEDC_TIMER_3_BIT, 10000000     
LEDC_TIMER_4_BIT,  5000000    
LEDC_TIMER_5_BIT,  2500000   
LEDC_TIMER_6_BIT,  1250000    
LEDC_TIMER_7_BIT,   625000   
LEDC_TIMER_8_BIT,   312500  
LEDC_TIMER_9_BIT,   156250   
LEDC_TIMER_10_BIT,   78125  
LEDC_TIMER_11_BIT,   39062  
LEDC_TIMER_12_BIT,   19531    
LEDC_TIMER_13_BIT,    9765  
LEDC_TIMER_14_BIT,    4882 
LEDC_TIMER_15_BIT,    2441  
LEDC_TIMER_16_BIT,    1220  
LEDC_TIMER_17_BIT,     610  
LEDC_TIMER_18_BIT,     305
LEDC_TIMER_19_BIT,     152 
LEDC_TIMER_20_BIT,      76 
