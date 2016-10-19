CXX		= g++
CXXFLAGS	= -O2 -Wall 
LDFLAGS		= -lgpib
SRCFILES	= MultiMeter.cpp MultiMeterMain.cpp ../gpib/gpib.cpp ../LogFileDev/LogDev.cpp
OBJFILES    = MultiMeter.o MultiMeterMain.o ../gpib/gpib.o ../LogFileDev/LogDev.o

MultiMeter:		$(OBJFILES)
		$(CXX) $(OBJFILES) $(LDFLAGS) -o $@

clean:
		rm -f $(OBJFILES) MultiMeter 

MultiMeter.o:	MultiMeter.cpp MultiMeter.h 

MultiMeterMain.o: MultiMeterMain.cpp MultiMeter.h 
