
SRC += ../config/comconfig.cpp
SRC += ../config/varconfig.cpp
SRC += ../comserver/comserver.cpp
SRC += ../comserver/comreader.cpp
SRC += ../comserver/storage.cpp
SRC += ../uart/comport.cpp
SRC += ../tcpserver/modbustcp.cpp
SRC += ../tcpserver/tcpserver.cpp

comserver: $(SRC) *.cpp  
	$(CC) $(CFLAG) $(LDFAGS) $+ -o $@ 
	$(STRIP) -s $@

clean:
	@rm -rfv comserver
