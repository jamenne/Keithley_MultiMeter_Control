CXX		= g++
CXXFLAGS	= -O2 -Wall 
LDFLAGS		= -lgpib
SRCFILES	= MultiMeter.cpp MultiMeterMain.cpp ../gpib/gpib.cpp
OBJFILES    = MultiMeter.o MultiMeterMain.o ../gpib/gpib.o

MultiMeter:		$(OBJFILES)
		$(CXX) $(OBJFILES) $(LDFLAGS) -o $@

clean:
		rm -f $(OBJFILES) MultiMeter 

MultiMeter.o:	MultiMeter.cpp MultiMeter.h ../gpib/gpib.h

gpib.o:	../gpib/gpib.cpp ../gpib/gpib.h

MultiMeterMain.o: MultiMeterMain.cpp MultiMeter.h ../gpib/gpib.h
