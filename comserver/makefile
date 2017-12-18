
SRC += ../config/comconfig.cpp
SRC += ../config/varconfig.cpp
SRC += ../uart/comport.cpp
comserver: $(SRC) *.cpp  
	$(CC) $(CFLAG) $(LDFAGS) $+ -o $@
	$(STRIP) -s $@

clean:
	@rm -rfv a.out comserver
