/*

SourecMeter.h


Created by Janine Müller on 07.10.2016


*/

#ifndef ____MultiMeter__
#define ____MultiMeter__

#include <string>
#include <vector>

using namespace std;


class MultiMeter
{

public:
	MultiMeter();
	~MultiMeter();
	int GetUD();
	void ResetDevice();
	void Initialize(int masterUD, int pad);
	void Initialize4Wire(int masterUD, int pad);

	void SetACcurrent();
	void SetDCcurrent();
	void SetACvoltage();
	void SetDCvoltage();
	void Set4WireFunction();
	void Set2WireFunction();
	void SetPeriodFunction();
	void SetFrequencyFunction();
	void SetTemperatureFunction();
	void SetDiodeFunction();
	void SetContinuityTest();
	void SetTriggerContinously();
	void SetAutorange4Wire();
	double Read();
	double Fetch();
	double MeasureACcurrent();
	double MeasureDCcurrent();
	double MeasureACvoltage();
	double MeasureDCvoltage();
	double Measure2WireRes();
	double Measure4WireRes();
	double MeasurePeriod();
	double MeasureFrequency();
	double MeasureTemperature();
	double MeasureDiode();
	double MeasureConinuity();


private:

	int _ud;
	double _value;
	double _temperature;


};


#endif /* defined(____MultiMeter__) */