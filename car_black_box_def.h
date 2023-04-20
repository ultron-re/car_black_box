/* 
 * File:   car_black_box_def.h
 * Author: nandu
 *
 * Created on 4 April, 2023, 5:01 AM
 */

#ifndef CAR_BLACK_BOX_DEF_H
#define	CAR_BLACK_BOX_DEF_H

void display_dash_board(char event[],unsigned char speed,unsigned char reset_flag);
void log_event(char event[],unsigned char speed);
void clear_screen(void);
unsigned char login(unsigned char reset_flag,unsigned char key);
unsigned char login_menu(unsigned char reset_flag,unsigned char key);
unsigned char view_log(unsigned char key,unsigned char reset_flag);
unsigned char clear_log(unsigned char reset_flag);
unsigned char set_time(unsigned char reset_flag,unsigned char key);
unsigned char change_password(unsigned char key,unsigned char reset_flag);

#endif	/* CAR_BLACK_BOX_DEF_H */

