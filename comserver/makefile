
SRC += ../config/comconfig.cpp
SRC += ../config/varconfig.cpp
SRC += ../uart/comport.cpp
test: $(SRC) *.cpp  
	$(CC) $(CFLAG) $(LDFAGS) $+

clean:
	@rm -rfv a.out
