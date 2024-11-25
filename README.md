# Advanced Level Line Follower Software

Developer: Zubeyir BAKACAK

This code designed for "Türkrobokit" robot board.

Used Arduino Nano and program runs at 700-750Hz from 16MHz clock frequency.

Faster than classic Arduino functions.

We used drone motor runs at 50Hz PWM Signal and this fan speed commands doesn't effect on main program.

Interrupt Service Routine added for 50Hz PWM signal and code runs quickly.

You can re-design this code for your equipment.

PD runs at this code because integral value reaches out the limit of the variable quickly and
corrupts running of the code so we're use proportional and derivative controller on this code.

Added comments in Turkish.
