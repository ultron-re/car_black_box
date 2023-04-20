#include "main.h"

unsigned char clock_reg[3];
char time[7];
char log[11];
char pos = -1;
int access=0;
int sec;
extern int return_time,tick;
char *menu[]={"View log","Clear log","Download log","Reset time","Change password"};
void get_time(void)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
    // HH -> bcd to ascii
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
   
    // MM 
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    
    // SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = '\0';
}
 
void display_time(reset_flag)
{
    if(reset_flag != RESET_TIME)
    {
        get_time();
    }
    //to display time
    clcd_putch(time[0],LINE2(2));
    clcd_putch(time[1],LINE2(3));
    clcd_putch(':',LINE2(4));
    clcd_putch(time[2],LINE2(5));
    clcd_putch(time[3],LINE2(6));
    clcd_putch(':',LINE2(7));
    clcd_putch(time[4],LINE2(8));
    clcd_putch(time[5],LINE2(9));
    
}

void display_dash_board(char event[],unsigned char speed,unsigned char reset_flag )
{
    clcd_print("  TIME     E  SP",LINE1(0));
    //to display time
    display_time(reset_flag);
    //to display event
    clcd_print(event,LINE2(11));
    //to display speed
    clcd_putch(speed/10+'0',LINE2(14));
    clcd_putch(speed%10+'0',LINE2(15));
    
}

void log_car_event(void)
{
    char addr;
    addr = 5;
    pos++;
    if(pos == 10)
    {
        pos=0;
    }
    addr = pos *10 + addr;
    
    ext_eeprom_24C02_str_write(addr,log);
    if(access<9)
        access++;
}


void log_event(char event[],unsigned char speed)
{
    strncpy(log,time,6);
    strncpy(&log[6],event,2);
    log[8]=speed/10;
    log[9]=speed%10;
    log[10]='\0';
    log_car_event();
    
}

unsigned char login(unsigned char reset_flag,unsigned char key)
{
    static char npasswd[4],i;
    static char spasswd[4];
    static unsigned char attempt_rem;
    if( reset_flag == RESET_PASSWORD)
    {
        i=0;
        attempt_rem=3;
        npasswd[0]='\0';
        npasswd[1]='\0';
        npasswd[2]='\0';
        npasswd[3]='\0'; 
        key = ALL_RELEASED;
        return_time=5;
        sec=0;
    }
    if(return_time == 0)
    {
        return RETURN_BACK ;
    }

    if(key == SW4 && i < 4)
    {
        npasswd[i]='1';
        i++;
        clcd_putch('*',LINE2(i+2));
        return_time=5;
    }
    else if(key == SW5 && i < 4)
    {
        npasswd[i]='0';
        i++;
        clcd_putch('*',LINE2(i+2));
        return_time=5;
    }
    if( i == 4 )
    {
        for(int k=0;k<4;k++)
        {
             spasswd[k]=ext_eeprom_24C02_read(k);
        }
        if(strncmp(npasswd,spasswd,4)==0)
        {
            return LOGIN_SUCCESS;
        }
        else
        {
            attempt_rem--;
            if(attempt_rem == 0)
            {
                clear_screen();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_ms(100);
                clcd_print("You are blocked",LINE1(0));
                clcd_print("Wait for 60 seconds",LINE2(0));
                
                sec=60;
                while(sec)
                {
                   clcd_putch(sec/10+'0',LINE2(9));
                   clcd_putch(sec%10+'0',LINE2(10));
                }
                attempt_rem=3;
            }
            else
            {
                clear_screen();
                clcd_print("wrong password",LINE1(0));
                clcd_putch(attempt_rem+'0',LINE2(0));
                clcd_print("Attempt rem",LINE2(2));
                __delay_ms(2000);
            }
            clear_screen();
            clcd_print("Enter Password",LINE1(1));
            clcd_putch(' ',LINE2(2));
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            i=0;
            return_time=5;
        }
    }
}

void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}

unsigned char login_menu(unsigned char reset_flag,unsigned char key)
{
    //char *menu[]={"View log","Clear log","Download log","Reset time","Change password"};
    static unsigned char menu_pos;   
    
    if(reset_flag == RESET_LOGIN_MENU)
    {
        menu_pos=0;
        clear_screen();
    }
    
    if(key == SW5 && menu_pos < 4 )
    {
        menu_pos++;
        clear_screen();
        
    }
    else if(key == SW4 && menu_pos > 0 )
    {
        menu_pos--;
        clear_screen();
        
    }
    if( menu_pos < 4 )
    {
        clcd_putch('*',LINE1(0));
        clcd_print(menu[menu_pos],LINE1(2));
        clcd_print(menu[menu_pos+1],LINE2(2));
    }
    else if( menu_pos == 4 )
    {
        clcd_putch('*',LINE2(0));
        clcd_print(menu[menu_pos-1],LINE1(2));
        clcd_print(menu[menu_pos],LINE2(2));
    }
    return menu_pos;
}

unsigned char view_log(unsigned char key,unsigned char reset_flag)
{
    char nlog[10];
    unsigned char add;
    static unsigned char npos;
    TMR2ON = 1;
         
    if( reset_flag == RESET_VIEW_LOG_POS)
    {
        key = ALL_RELEASED;
        return_time=10;
        sec=0;
    }
    if(access == -1)
    {
        clcd_print(" No logs",LINE2(0));
    }
     else
    {
        if (reset_flag == RESET_VIEW_LOG_POS)
        {
            npos=0;
        }
        if(key == SW4 && npos < (access-1))
        {
            npos++;
            return_time=10;
        }
        else if(key==SW5 && npos > 0)
        {
            npos--;
            return_time=10;
        }
        for(int i=0;i<10;i++)
        {
            add=npos*10+5;
            nlog[i]=ext_eeprom_24C02_read(add + i);
        }

        clcd_putch(npos%10 + '0',LINE2(0));

        clcd_putch(nlog[0],LINE2(2));
        clcd_putch(nlog[1],LINE2(3));
        clcd_putch(':',LINE2(4));

        clcd_putch(nlog[2],LINE2(5));
        clcd_putch(nlog[3],LINE2(6));
        clcd_putch(':',LINE2(7));

        clcd_putch(nlog[4],LINE2(8));
        clcd_putch(nlog[5],LINE2(9));
        
        clcd_putch(nlog[6],LINE2(11));
        clcd_putch(nlog[7],LINE2(12));

        clcd_putch(nlog[8],LINE2(14));
        clcd_putch(nlog[9],LINE2(15));   
    }
    if(key == SW6 || return_time == 0)
    {
        return RETURN_BACK;
    }
}

unsigned char clear_log(unsigned char reset_flag)
{
    if( reset_flag == RESET_MEMORY)
    {
        pos = -1;
        access = -1;
        clcd_print(" Logs Cleared ",LINE1(0));
        clcd_print(" Successfully ",LINE2(0));
        __delay_ms(3000);
         return TASK_SUCCESS;
    }
    return TASK_FAIL;
}

unsigned char set_time(unsigned char reset_flag,unsigned char key)
{
    static int fp = 3;
    TMR2ON = 1;
    int hr,min,sec;
    TMR2ON = 1;
         
    if( reset_flag == RESET_TIME)
    {
        key = ALL_RELEASED;
        
    }
    if(tick%2==0)
    {
        clcd_putch(time[0],LINE2(2));
        clcd_putch(time[1],LINE2(3));
        clcd_putch(':',LINE2(4));
        clcd_putch(time[2],LINE2(5));
        clcd_putch(time[3],LINE2(6));
        clcd_putch(':',LINE2(7));
        clcd_putch(time[4],LINE2(8));
        clcd_putch(time[5],LINE2(9));       
    }  
    else
    {
        clcd_putch(0xFF,LINE2(fp));
    }
    if(key == SW5 )
    {
        if(fp < 9)
        {
           fp = fp + 3 ;
        }
        else
            fp=3;
    }
    if(key == SW4 )
    {
        if(fp == 3)
        {
            if(time[1]++ == '9' && time[0] < '2')
            {
                time[0]++;
                time[1]='0';
            }
            else if(time[0] == '2' )
            {
                if(time[1] =='4')
                {
                    time[1]='0';
                    time[0]='0';
                }
            }
        }
        else if(fp == 6)
        {
            if(time[3]++ == '9' && time[2] < '6')
            {
                time[2]++;
                time[3]='0';
            }
            if(time[2] == '6' )
            {
                time[3]='0';
                time[2]='0';
            }
        }
        else if(fp == 9)
        {
            if(time[5]++ == '9' && time[4] < '6')
            {
                time[4]++;
                time[5]='0';
            }
            if(time[4] == '6' )
            {
                time[5]='0';
                time[4]='0';
            }
        }
    }
    if(key == SW6 )
    {
        return RETURN_BACK;
    }
}
unsigned char change_password(unsigned char key,unsigned char reset_flag)
{
    static char pwd[9];
    static int p,once;
    if(reset_flag==RESET_CHANGE_PASSWORD)
    {
        p=0;
        once=1;
    }
    if(p<4 && once)
    {
        once=0;
        clcd_print("Enter New Pwd",LINE1(0));
        clcd_write(LINE2(0), INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
    }
    else if(p>=4 && (once==0))
    {
        once=1;
        clear_screen();
        clcd_print("Re-enter New Pwd",LINE1(0));
        clcd_write(LINE2(0), INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
    }
    if(key==SW4)// '4'
    {
        pwd[p]='4';
        clcd_putch('*',LINE2(p%4));
        p++;
    }
    else if(key==SW5)// '2'
    {
        pwd[p]='2';
        clcd_putch('*',LINE2(p%4));
        p++;
    }
    if(p==8)
    {
        if(strncmp(pwd,&pwd[4],4)==0)
        {
            
            for(int i=0;i<4;i++)
            {
                eeprom_write(i,pwd[i]);
            }
            clear_screen();
            clcd_print(" Password Change",LINE1(0));
            clcd_print(" Successfully ",LINE2(0));
            __delay_ms(3000);
            return TASK_SUCCESS;
        }
        else
        {
            clear_screen();
            clcd_print(" Password Change",LINE1(0));
            clcd_print(" Failed ",LINE2(0));
            __delay_ms(3000);
            return TASK_SUCCESS;
        }
        
    }
}