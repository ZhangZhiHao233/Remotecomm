#include <reg52.h>  

#define	yes 1
#define open 1
#define start 1
  
void    initSer();          //���ڳ�ʼ��  
unsigned char   sendFlag = 0;     //δ��������ʱ  
unsigned char   receFlag =0;     //δ���ܵ�����ʱ  
  
code  unsigned char  TEXT0[] =  "****************************\r\n";  //\r\n�ǻس�����  
code  unsigned char   TEXT1[] = "Զ�̵��Թ��ߴ��ڲ���\r\n";  
code  unsigned char   TEXT2[] = "ZhangZhiHao 14552613\r\n";  
code  unsigned char   TEXT3[] = "****************************\r\n\r\n";//�س����в���1��  
  
void sendChar(unsigned char  sendValue);  //����һ�ֽ�����  
void sendAll(unsigned char  *pValue);       //����һ������  
  
void main(void)  
{  
    initSer();  
    while(1)  
    {  
      while(receFlag)               //��Ƭ�������յ��������ݺ�,��ʼ��PC��������  
      {  
           sendAll(TEXT0);         //��������  
           sendAll(TEXT1);  
           sendAll(TEXT2);  
           sendAll(TEXT3);  
            receFlag=0;             //���������־      
      }  
    }  
}  
  
void serInt() interrupt 4   //�жϺ���  
{  
    if(RI)                  //����յ���������  
    {  
        RI = 0;  
        P1=SBUF;        //���ݵı仯��led��ֱ�۷�Ӧ����  
        receFlag=1;   //�޸Ľ��ܱ�־,��������������while�з�����  
    }  
      
    if(TI)  
    {  
        TI = 0;                //������һ������  
        sendFlag = 0;        //���־λ  
    }  
}  
  
  
void sendChar(unsigned char  Value)  //����һ���ֽ�����  
{  
     SBUF = Value;       
     sendFlag = 1;       //���÷��ͱ�־λ,��һ�ֽھ���λ  
     while(sendFlag);    //ֱ����������,��sendFlag�����,���˳�sendChar����  
}  
  
void sendAll(unsigned char  *pValue) //����һ������  
{  
    while((*pValue) != '\0')   //���û�з�����Ͼͼ�����  
    {  
        sendChar(*pValue);      //����1�ֽ�����  
        pValue++;                 //ָ����1���ֽ�  
    }  
}  
  
void initSer()      //��ʼ��  
{  
    //ע��:���ϵ�ʱ,SCON��Ϊ0��  
    TMOD=TMOD|0x20; //T1��ʱ��ģʽ,������ʽ2  
    TH1=0Xfd;        // 256-(11059200/(32*12*9600))  
    TL1=0xfd;  
    SM0=0;           //����SCON�Ĵ���  
    SM1=1;           //���ڹ�����ʽ1,10λ�첽,�����ʿɸ�  
    REN=yes;             //�����ڽ���  
    ES=open;             //�������ж�  
    EA=open;             //�����ж�  
    TR1=start;       //������ʱ��  
} 