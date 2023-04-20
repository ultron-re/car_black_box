## Project: Car Black Box

## Requirements:
• Default Screen
◦ When the system is in Operation Mode, it would act like a dash board which
would show the current time, speed of vehicle and the latest occurred event.
• Login Screen
◦ On press of the UP or DOWN (User Keys) keys the system should prompt for
password
◦ The password would be the combination of 4 presses (User Keys).
◦ Each press should be denoted a "*" symbol
◦ Every wrong entry would, re prompt for password (Max 3 times for every 15
minutes)
◦ Incomplete key press (pause of 3 seconds) would lead to Default Screen
• Main Menu
◦ The main menu should contain 5 option

View Log

Clear Log

Reset time

Change password
◦ The UP / DOWN keys are used to navigate
◦ A long press of UP Key should enter the selected menu
◦ A long press of DOWN Key should logout
◦ Idle screen for more than 5 secs should logout


### View Log
◦ Should display all the events captured with log index starting from 0, like
“EVENT NUMBER” “EVENT SIGNATURE” “EVENT TIME” “SPEED AT THE EVENT”
◦ The UP and DOWN key will be used to scroll the entries
◦ Rollover on reaching the max log entries
◦ The system should be live (capture events occurred) even while viewing the log
◦ A long press of UP Key should take you back to main menu
◦ A long press of DOWN Key should logout
### Clear log:
-> clear the logged data in the external eeprom
### Change password:
-> should ask user to Enter password:
-> should ask user to re - enter password:
->if passwords are matching update the password in EEPROM
### Set Time
◦ Should show the current time. The Secs field should blink indicating the field to
be changed
◦ The UP key should be used to increment the time. Rollover on reaching max
◦ The DOWN key will be used to choose the field.
◦ A long press of UP Key should take you back to main menu
• Event Capture
◦ Required events have to be captured and stored in the memory
◦ Every event should have a format as
“EVENT SIGNATURE” “EVENT TIME” “SPEED AT THE EVENT”
◦ The events should be captured real time, no matter which mode you are in
## Prerequisite:
• Embedded C programming
• I2C protocols
Level:
• Medium
Design:
Source:
