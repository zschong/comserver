
SRC += ../config/comconfig.cpp
SRC += ../config/varconfig.cpp
SRC += ../comserver/comserver.cpp
SRC += ../comserver/comreader.cpp
SRC += ../uart/comport.cpp
SRC += ../tcpserver/modbustcp.cpp
SRC += ../tcpserver/tcpserver.cpp

test: $(SRC) *.cpp  
	$(CC) $(CFLAG) $(LDFAGS) $+

clean:
	@rm -rfv a.out
