CPU
===

CPU/Memory monitor for Linux + Arduino UNO /SainSmart LCD

This is a fun project that is useful for monitoring your machines state without
taking up valuable screen real estate. It uses an Arduino Uno ($20) and an easy
to use LCD display by SainSmart and a simple driver using Perl and top.

SETUP:
- Top
  Top needs some special configurations to display all cpu's correctly

  1) run 'top' from the command line
  2) press '1' this will list all CPU's
  3) press 'W' this will write the configuration file

- Script
  After that you simply need to run the driver script.
  It assumes the Arduino is listening on either /dev/ttyACM0 or /dev/ttyACM1
  If this is not the case you can simply edit the file to reflect the correct
  device

- Running
  Your user will need write access to your device (/dev/ttyACM0) so you
  might need to add your user to the uucp or dialout group depending on 
  how your system is setup.

  Running the driver in either a tmux or screen session is useful

  $ tmux -L cpu\
  $ ./CPU/cpu.pl\
  \#C,8,17,16,14,4,7,12,13,7,M,11346,16331,D,11:34 AM\#
  \#C,8,14,18,4,3,11,19,8,7,M,11346,16331,D,11:34 AM\#
  \#C,8,24,14,3,9,8,12,10,7,M,11345,16331,D,11:34 AM\#
  \#C,8,19,18,4,3,7,20,7,7,M,11345,16331,D,11:34 AM\#
  \#C,8,36,16,2,3,7,5,6,7,M,11346,16331,D,11:34 AM\#


SainSmart LCD: 
http://www.amazon.com/dp/B006OVYI1G/ref=pe_175190_21431760_M3T1_ST1_dp_1




