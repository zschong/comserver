CC := g++
CC := arm-linux-g++
CFLAG := -I$(shell pwd)/config
CFLAG += -I$(shell pwd)/uart
CFLAG += -I$(shell pwd)/thread
CFLAG += -I$(shell pwd)/socket
CFLAG += -I$(shell pwd)/entity
CFLAG += -I$(shell pwd)/modbus
CFLAG += -I$(shell pwd)/tcpserver
CFLAG += -I$(shell pwd)/comserver
SRC := $(shell find $$(pwd)/config/ | grep ".c$$")
SRC += $(shell find $$(pwd)/uart/   | grep ".c$$")
SRC += $(shell find $$(pwd)/socket/ | grep ".c$$")
SRC += $(shell find $$(pwd)/modbus/ | grep ".c$$")
SRC += $(shell find $$(pwd)/thread/ | grep ".cpp$$")
LDFAGS := -lpthread -g
export CC CFLAG SRC LDFAGS

default:
	@for i in $(SRC);do echo $$i;done

tcpserver.exe:
	@make -C tcpserver/

comserver.exe:
	@make -C comserver/

config.exe:
	@make -C config/

manager.exe:
	@make -C manager/

clean:
	@make -C tcpserver/ clean
	@make -C comserver/ clean
	@make -C config/	clean
	@make -C manager/	clean
