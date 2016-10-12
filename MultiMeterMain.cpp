/*

MultiMeterMain.cpp

Created by Janine Müller on 07.10.2016

*/

#include <iostream>

#include "MultiMeter.h"
#include "../gpib/gpib.h"



int main(int argc, char const *argv[])
{

	int masterUD = InitializeMaster();

	MultiMeter* Keithley = new MultiMeter();

	Keithley->Initialize(masterUD, 18);

	Keithley->Set4WireFunction();

	Keithley->SetAutorange4Wire();

	Keithley->SetTriggerContinously();

	for (int i = 0; i < 10; i++)
	{
		Keithley->Fetch();
		sleep(2);

	}
	


	
	return 0;
}
