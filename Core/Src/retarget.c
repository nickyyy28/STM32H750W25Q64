#if 1  
#include <stdio.h>
#pragma import(__use_no_semihosting)               
//��׼����Ҫ��֧�ֺ���                   
struct __FILE   
{   
    int handle;   
    /* Whatever you require here. If the only file you are using is */   
    /* standard output using printf() for debugging, no file handling */   
    /* is required. */   
};   
/* FILE is typedef�� d in stdio.h. */   
FILE __stdout;         
//����_sys_exit()�Ա���ʹ�ð�����ģʽ      
int _sys_exit(int x)   
{   
    x = x;   
}   
//�ض���fputc����  
//printf�������ָ��fputc����fputc���������  
//����ʹ�ô���1(USART1)���printf��Ϣ  
int fputc(int ch, FILE *f)  
{        
    //while((USART1->SR&0X40)==0);//�ȴ���һ�δ������ݷ������    
    //USART1->DR = (u8) ch;        //дDR,����1����������  
    return ch;  
}  
#endif