#! /bin/bash

 #check if setserial is installed
 
 pkg=$(dpkg -s setserial | awk '/Status:/ {print}')
 

 if [[ $pkg == "Status: install ok installed" ]];
 then
 	echo set serial is installed!
 else
 	echo ERROR! set serial is not installed
 	echo Install setserial to proceed?
 	
 	read ans
 	
 	if [ $ans == 'y' ]
 	then
 	echo Installing set serial!
 	echo Performing update before install!
 	
 	sudo apt-get update
 	sudo apt-get install setserial
 	
 	else
 	
 	echo ok bye! 
 	
 	exit 1
 	fi
 
 fi
 
 echo Checking sytems serial ports!
 
 sudo setserial -g /dev/ttyS[01]
 
 echo Which of the above serial ports would you like to use?
 echo Enter the '/dev/ttyS*' you would like to use
 
 read ans 
 
 if [ [$ans == "/dev/ttyS0" ] || [ $ans == "/dev/ttyS1" ] || [ $ans == "/dev/ttyS2" ];
 
 	then 
 	echo Valid Port
	 
 fi
 
