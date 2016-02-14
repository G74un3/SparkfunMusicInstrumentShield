#fra https://github.com/sudar/Arduino-Makefile


#Kan findes ved at køre make show_boards
BOARD_TAG = uno

#Space separated set of libraries that are used by your sketch
# Liobraries shoudl be placed in /usr/share/arduino/hardware/arduino/cores/arduino/PID_AutoTune_v0 as well as ~/sketchbook/libraries
 
ARDUINO_LIBS = LiquidCrystal SoftwareSerial

#Use dmesg if unsure about this
ARDUINO_PORT = /dev/ttyACM3


AVR_TOOLS_DIR = /usr/share/arduino/hardware/tools/avr


#TJEK OG UPDATE
USER_LIB_PATH += /home/daniel/libraries


include $(ARDMK_DIR)/Arduino.mk


# compile med: make
#uplaod med: make upload
#Serial monitor med: make monitor
#Log i monitor: https://stackoverflow.com/questions/14208001/save-screen-program-output-to-a-file

# se http://hardwarefun.com/tutorials/compiling-arduino-sketches-using-makefile
