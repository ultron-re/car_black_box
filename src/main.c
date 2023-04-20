/*
 * File:   main.c
 * Author: nandu
 *
 * Created on 4 April, 2023, 4:43 AM
 */


#include "main.h"

#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)

static void init_config(void) 
{
    //initialization 
    init_clcd();
    init_digital_keypad();
    init_adc();
    init_i2c(100000);
    init_ds1307();
    init_timer2();
    
    GIE = 1;
    PEIE = 1;
}

void main(void) 
{
    init_config();
    unsigned char control_flag = DASH_BOARD_FLAG,reset_flag;
    unsigned char speed=0;
    unsigned char key;
    char event[3] = "ON";
    char *gear[]= {"GN","GR","G1","G2","G3","G4"};
    int gr=0;
    unsigned char menu_pos;
    
    log_event(event,speed);
    
    ext_eeprom_24C02_str_write(0x00,"1010");
    
    while (1) 
    {
        key = read_digital_keypad(STATE);
        
        speed = read_adc()/10;
        if(speed > 99)
        {
            speed = 99;
        }
        
        if( key == SW1)
        {
            strcpy(event,"C ");
            log_event(event,speed);
        }
        else if( key == SW2 && gr < 6)
        {
            strcpy(event,gear[gr]);
            gr++;
            log_event(event,speed);
        }
        else if( key == SW3 && gr > 0)
        {
            gr--;
            strcpy(event,gear[gr]);  
            log_event(event,speed);
        }
        else if((key == SW4 || key == SW5) && (control_flag == DASH_BOARD_FLAG))
        {
            clear_screen();
            control_flag = LOGIN_FLAG;
            reset_flag = RESET_PASSWORD;
            clcd_print("Enter Password ",LINE1(1));
            clcd_putch(' ',LINE2(2));
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            TMR2ON = 1;
        }
        else if((key == SW6) && (control_flag == LOGIN_MENU_FLAG))
        {
            switch(menu_pos)
            {
                case 0:
                    clear_screen();
                    clcd_print("# TIME     E  SP",LINE1(0));
                    control_flag=VIEW_LOG_FLAG;
                    reset_flag=RESET_VIEW_LOG_POS;
                    break;
                case 1:
                    clear_screen();
                    control_flag = CLEAR_LOG_FLAG;
                    reset_flag=RESET_MEMORY;
                    break;
                case 2:
                    control_flag = DOWNLOAD_LOG_FLAG;
                    break;
                case 3:
                    clear_screen();
                    clcd_print(" SET TIME",LINE1(0));
                    control_flag = SET_TIME_FLAG;
                    reset_flag=RESET_TIME;
                    break;
                case 4:
                    clear_screen();
                    control_flag=CHANGE_PASSWORD_FLAG;
                    reset_flag=RESET_CHANGE_PASSWORD;
                    break;  
            }
        }
        switch(control_flag)
        {
            case DASH_BOARD_FLAG: //dash board screen
                display_dash_board(event,speed,reset_flag);
                break;
            case LOGIN_FLAG: //login screen
                switch(login(reset_flag,key))
                {
                    case RETURN_BACK:
                        control_flag=DASH_BOARD_FLAG;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON=0;
                        break;
                    case LOGIN_SUCCESS:
                        control_flag=LOGIN_MENU_FLAG;
                        reset_flag=RESET_LOGIN_MENU;
                        continue;
                        break;
                }
                break;
            case LOGIN_MENU_FLAG:
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                 __delay_us(100);
                menu_pos=login_menu(reset_flag,key);
                break;
            case VIEW_LOG_FLAG:
                if(view_log(key,reset_flag)==RETURN_BACK)
                {
                    clear_screen();
                    control_flag=LOGIN_MENU_FLAG;
                    reset_flag=RESET_LOGIN_MENU;
                    TMR2ON=0;
                    continue;
                }
                break;
            case CLEAR_LOG_FLAG:
                if(clear_log(reset_flag)==TASK_SUCCESS)
                {
                    clear_screen();
                    control_flag=LOGIN_MENU_FLAG;
                    reset_flag=RESET_LOGIN_MENU;
                    continue;
                }
                break;
            case DOWNLOAD_LOG_FLAG:
                break;
            case SET_TIME_FLAG:
                if(set_time(reset_flag,key)==RETURN_BACK)
                {
                    clear_screen();
                    control_flag=LOGIN_MENU_FLAG;
                    reset_flag=RESET_LOGIN_MENU;
                    TMR2ON=0;
                    continue;
                }
                break;
            case CHANGE_PASSWORD_FLAG:
                switch(change_password(key,reset_flag))
                {
                    case TASK_SUCCESS:
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                        __delay_us(100);
                        control_flag=LOGIN_MENU_FLAG;
                        reset_flag=RESET_LOGIN_MENU;
                        continue;
                        break;
                }
                break;
                
        }
        reset_flag = RESET_NOTHING;
    }
    return;
}

