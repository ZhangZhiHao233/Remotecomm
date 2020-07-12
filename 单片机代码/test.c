#include <reg52.h>  

#define	yes 1
#define open 1
#define start 1
  
void    initSer();          //串口初始化  
unsigned char   sendFlag = 0;     //未发送数据时  
unsigned char   receFlag =0;     //未接受到数据时  
  
code  unsigned char  TEXT0[] =  "****************************\r\n";  //\r\n是回车换行  
code  unsigned char   TEXT1[] = "远程调试工具串口测试\r\n";  
code  unsigned char   TEXT2[] = "ZhangZhiHao 14552613\r\n";  
code  unsigned char   TEXT3[] = "****************************\r\n\r\n";//回车换行并空1行  
  
void sendChar(unsigned char  sendValue);  //发送一字节数据  
void sendAll(unsigned char  *pValue);       //发送一组数据  
  
void main(void)  
{  
    initSer();  
    while(1)  
    {  
      while(receFlag)               //单片机串口收到任意数据后,开始向PC发送数据  
      {  
           sendAll(TEXT0);         //发送数据  
           sendAll(TEXT1);  
           sendAll(TEXT2);  
           sendAll(TEXT3);  
            receFlag=0;             //发完了清标志      
      }  
    }  
}  
  
void serInt() interrupt 4   //中断函数  
{  
    if(RI)                  //如果收到任意数据  
    {  
        RI = 0;  
        P1=SBUF;        //数据的变化让led灯直观反应出来  
        receFlag=1;   //修改接受标志,便于主函数进入while中发数据  
    }  
      
    if(TI)  
    {  
        TI = 0;                //发送完一个数据  
        sendFlag = 0;        //清标志位  
    }  
}  
  
  
void sendChar(unsigned char  Value)  //发送一个字节数据  
{  
     SBUF = Value;       
     sendFlag = 1;       //设置发送标志位,发一字节就置位  
     while(sendFlag);    //直到发完数据,将sendFlag清零后,才退出sendChar函数  
}  
  
void sendAll(unsigned char  *pValue) //发送一组数据  
{  
    while((*pValue) != '\0')   //如果没有发送完毕就继续发  
    {  
        sendChar(*pValue);      //发送1字节数据  
        pValue++;                 //指向下1个字节  
    }  
}  
  
void initSer()      //初始化  
{  
    //注意:刚上电时,SCON是为0的  
    TMOD=TMOD|0x20; //T1定时器模式,工作方式2  
    TH1=0Xfd;        // 256-(11059200/(32*12*9600))  
    TL1=0xfd;  
    SM0=0;           //属于SCON寄存器  
    SM1=1;           //串口工作方式1,10位异步,波特率可改  
    REN=yes;             //允许串口接收  
    ES=open;             //开串口中断  
    EA=open;             //开总中断  
    TR1=start;       //启动定时器  
} 