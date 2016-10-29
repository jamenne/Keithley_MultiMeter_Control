/*

MultiMeter.cpp


Created by Janine Müller on 07.10.2016

*/

#include <gpib/ib.h>
#include <gpib/gpib_user.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include "time.h"
#include <sstream>

#include "MultiMeter.h"
#include "../LogFileDev/LogDev.h"
#include "../gpib/gpib.h"

using namespace std;

//---------------------MultiMeter class---------------------//

// Constructor
MultiMeter::MultiMeter() : 
	_ud(0),
	_LogFile()
{ 

}

//Destructor
MultiMeter::~MultiMeter(){}

int MultiMeter::GetUD(){

	return _ud;
}

void MultiMeter::Initialize(int masterUD, int pad){

	stringstream ss;
	ss << "MultiMeter_" << pad;

	this->_LogFile.Initialize(ss.str().c_str());

	cout << "START MultiMeter::Initialize" << endl;
	this->_LogFile.Write("START MultiMeter::Initialize");
	cout << "-----------------------------" << endl;
	this->_LogFile.Write("-----------------------------");
	cout << "Try to open MultiMeter device with pad=" << pad << endl;

	// int ibdev(int board_index, int pad, int sad, int timeout, int send_eoi, int eos);
	//_________________________________________________________________________________//
	// ibdev() is used to obtain a device descriptor, which can then be used by other functions in the library.
	// The argument board_index specifies which GPIB interface board the device is connected to. 
	// The pad and sad arguments specify the GPIB address of the device to be opened (see ibpad() and ibsad()).
	// The timeout for io operations is specified by timeout (see ibtmo()). 
	// If send_eoi is nonzero, then the EOI line will be asserted with the last byte sent during writes (see ibeot()).
	// Finally, the eos argument specifies the end-of-string character and whether or not its reception should terminate reads (see ibeos()).
	int ud=ibdev(0,pad,0,20,1,/*1024*/1034);

	if (iberr==0)
	{
		cout << "Success: ud="<< ud << endl;
		this->_LogFile.Write("Success!");
	}
	else{
		cout << "Initialization FAILED!" << endl;
		this->_LogFile.Write("Initialization FAILED!");
		exit (EXIT_FAILURE);
	}

	this->_ud=ud;

	// int ibln(int ud, int pad, int sad, short *found_listener);
	//____________________________________________________________//
	// ibln() checks for the presence of a device, by attempting to address it as a listener.
	// ud specifies the GPIB interface board which should check for listeners. If ud is a device descriptor, then the device's access board is used.
	// The GPIB address to check is specified by the pad and sad arguments. pad specifies the primary address, 0 through 30 are valid values.
	// sad gives the secondary address, and may be a value from 0x60 through 0x7e (96 through 126), or one of the constants NO_SAD or ALL_SAD. 
	// NO_SAD indicates that no secondary addressing is to be used, and ALL_SAD indicates that all secondary addresses should be checked.
	// If the board finds a listener at the specified GPIB address(es), then the variable specified by the pointer found_listener is set to a nonzero value.
	// If no listener is found, the variable is set to zero.
	// The board must be controller-in-charge to perform this function. 
	// Also, it must have the capability to monitor the NDAC bus line (see iblines()).
	short *foundLstn = new short;

	ibln(masterUD, pad, 0,foundLstn);
	if((*foundLstn)==0)
	{
	  cout << "Device not found for ud=" << ud << " and pad=" << pad << endl;
	  this->_LogFile.Write("Device not found!");
	  cout << "Initialization FAILED!" << endl;
	  this->_LogFile.Write("Initialization FAILED!");
	  return;
	}
	else{ 
		cout << "Listener found." << endl;
		this->_LogFile.Write("Listener found");
	}

	// clear device
	int returnval=ibclr(ud);
	cout << "Device clear sent " << returnval << endl;

	this->ResetDevice();

	cout << "MultiMeter Initialization finished." << endl;
	cout << "------------------------------------" << endl;
	this->_LogFile.Write("MultiMeter Initialization finished.");
	this->_LogFile.Write("------------------------------------");
}


void MultiMeter::ResetDevice(){
	int returnval = ibwrt_string(this->_ud, "*RST");
	cout << "Set default settings: " << returnval << endl;
	this->_LogFile.Write("Set default settings.");

}
// :CONFigure:<function>
// Places the Model 2010 in a “one-shot” measurement mode for the specified function.
// These commands configure the instrument for subsequent measurements on the specified function.
// This command places the instrument in a “one-shot” measurement mode. 
// You can then use the :READ? command to trigger a measurement and acquire a reading (see :READ?).
// ------------------------------------------------------------------------------------------------//

// AC Current
void MultiMeter::SetACcurrent(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":CONFigure:CURRent:AC");
	out << "Set AC current function: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());
}

//DC Current
void MultiMeter::SetDCcurrent(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":CONFigure:CURRent[:DC]");
	out << "Set DC current function: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());	
}

// AC Voltage
void MultiMeter::SetACvoltage(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":CONFigure:VOLTage:AC");
	out << "Set AC voltage function: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());
}

// DC Voltage
void MultiMeter::SetDCvoltage(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":CONFigure:VOLTage[:DC]");
	out << "Set DC voltage function: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());	
}

// Four-wire resistance
void MultiMeter::Set4WireFunction(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":CONFigure:FRESistance");
	out << "Set 4-wire-ohm function: " << returnval ;	
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());
}

// Two-wire resistance
void MultiMeter::Set2WireFunction(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":CONFigure:RESistance");
	out << "Set 2-wire-ohm function: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());
}

// Period
void MultiMeter::SetPeriodFunction(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":CONFigure:PERiod");
	out << "Set period function: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());
}

// Frequency
void MultiMeter::SetFrequencyFunction(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":CONFigure:FREQuency");
	out << "Set frequency function: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());
}

// Temperature
void MultiMeter::SetTemperatureFunction(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":CONFigure:TEMPerature");
	out << "Set temperature function: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());
}

// Diode
void MultiMeter::SetDiodeFunction(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":CONFigure:DIODe");
	out << "Set diode function: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());
}

// Continuity test
void MultiMeter::SetContinuityTest(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":CONFigure:CONTinuity");
	out << "Set continuity function: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());
}


// This command is typically used with the instrument in the “one-shot” measurement mode 
// to trigger and acquire a specified number of readings.
double MultiMeter::Read(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":READ?");
	out << "Sent read command: " << returnval;
	//cout << out.str() << endl;
	//this->_LogFile.Write(out.str());
	double result = ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;
	return result;
}

// This command requests the latest post-processed reading. 
// After sending this command and addressing the Model 2010 to talk, the reading is sent to the computer. 
// This command does not affect the instrument setup.
double MultiMeter::Fetch(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":FETCh?");
	out << "Sent fetch command: " << returnval;
	//cout << out.str() << endl;
	//this->_LogFile.Write(out.str());
	double result = ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;
	return result;
}

// When continuous initiation is selected (ON), the instrument is taken out of the idle state. 
// At the conclusion of all programmed operations, the instrument returns to the top of the trigger model.
// With continuous initiation enabled (ON), you cannot use the :READ? command or set sample count greater than one (see :SAMPle:COUNt).
// Use MultiMeter::Fetch() to get data
void MultiMeter::SetTriggerContinously(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, ":INITiate:CONTinuous ON");
	out << "Enable continuous initiation: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());

}

// These commands are used to control auto ranging. 
// With auto ranging enabled, the instrument automatically goes to the most sensitive range to perform the measurement.
void MultiMeter::SetAutorange4Wire(){
	stringstream out;
	int returnval = ibwrt_string(this->_ud, "[:SENSe[1]]:FRESistance:RANGe:AUTO ON");
	out << "Enable autrange for 4 wire resistant: " << returnval;
	cout << out.str() << endl;
	this->_LogFile.Write(out.str());

}

// :MEASure:<function>?
// This command combines all of the other signal oriented measurement commands to perform a “one-shot” measurement and acquire the reading.
// When this command is sent, the following commands execute in the order that they are presented.
// :ABORt:CONFigure:<function>:READ?
double MultiMeter::MeasureACcurrent(){

	int returnval = ibwrt_string(this->_ud, ":MEASure:CURRent:AC?");
	//cout << "Sent measure command: " << returnval << endl;
	double result=ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;

	return result;
}

double MultiMeter::MeasureDCcurrent(){

	int returnval = ibwrt_string(this->_ud, ":MEASure:CURRent[:DC]?");
	//cout << "Sent measure command: " << returnval << endl;
	double result=ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;

	return result;
}

double MultiMeter::MeasureACvoltage(){

	int returnval = ibwrt_string(this->_ud, ":MEASure:VOLTage:AC?");
	//cout << "Sent measure command: " << returnval << endl;
	double result=ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;

	return result;
}

double MultiMeter::MeasureDCvoltage(){

	int returnval = ibwrt_string(this->_ud, ":MEASure:VOLTage[:DC]?");
	//cout << "Sent measure command: " << returnval << endl;
	double result=ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;

	return result;
}

double MultiMeter::Measure2WireRes(){

	int returnval = ibwrt_string(this->_ud, ":MEASure:RESistance?");
	//cout << "Sent measure command: " << returnval << endl;
	double result=ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;

	return result;
}

double MultiMeter::Measure4WireRes(){

	int returnval = ibwrt_string(this->_ud, ":MEASure:FRESistance?");
	//cout << "Sent measure command: " << returnval << endl;
	double result=ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;

	return result;
}

double MultiMeter::MeasurePeriod(){

	int returnval = ibwrt_string(this->_ud, ":MEASure:PERiod?");
	//cout << "Sent measure command: " << returnval << endl;
	double result=ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;

	return result;
}

double MultiMeter::MeasureFrequency(){

	int returnval = ibwrt_string(this->_ud, ":MEASure:FREQuency?");
	//cout << "Sent measure command: " << returnval << endl;
	double result=ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;

	return result;
}

double MultiMeter::MeasureTemperature(){

	int returnval = ibwrt_string(this->_ud, ":MEASure:TEMPerature?");
	//cout << "Sent measure command: " << returnval << endl;
	double result=ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;

	return result;
}

double MultiMeter::MeasureDiode(){

	int returnval = ibwrt_string(this->_ud, ":MEASure:DIODe?");
	//cout << "Sent measure command: " << returnval << endl;
	double result=ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;

	return result;
}

double MultiMeter::MeasureConinuity(){

	int returnval = ibwrt_string(this->_ud, ":MEASure:CONTinuity?");
	//cout << "Sent measure command: " << returnval << endl;
	double result=ibrd_double(this->_ud);
	//cout << "Read result: " << result << endl;

	return result;
}

