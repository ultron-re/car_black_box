#include <xc.h>
#include "main.h"

extern int sec;
int return_time,tick;
void __interrupt() isr(void)
{
    static unsigned int count = 0,count1=0;
    
    if (TMR2IF == 1)
    {
        
        if (++count == 1250)
        {
            count = 0;
            if(sec>0)
            {
                sec--;
            }
            else if( sec == 0 && return_time >0 )
            {
                return_time--;
            }
          
        }
        if (++count1 == 750)
        {
            count1=0;
            if(tick++ == 60)
            {
                tick=0;
            }
            
        }
        
        TMR2IF = 0;
    }
}