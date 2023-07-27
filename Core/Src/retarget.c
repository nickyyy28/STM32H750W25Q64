#if 1  
#include <stdio.h>
#pragma import(__use_no_semihosting)               
//标准库需要的支持函数                   
struct __FILE   
{   
    int handle;   
    /* Whatever you require here. If the only file you are using is */   
    /* standard output using printf() for debugging, no file handling */   
    /* is required. */   
};   
/* FILE is typedef’ d in stdio.h. */   
FILE __stdout;         
//定义_sys_exit()以避免使用半主机模式      
int _sys_exit(int x)   
{   
    x = x;   
}   
//重定向fputc函数  
//printf的输出，指向fputc，由fputc输出到串口  
//这里使用串口1(USART1)输出printf信息  
int fputc(int ch, FILE *f)  
{        
    //while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成    
    //USART1->DR = (u8) ch;        //写DR,串口1将发送数据  
    return ch;  
}  
#endif