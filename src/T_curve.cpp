#include "T_curve.hpp"
#include <cmath>

T_curve::T_curve(double _changeValue, int _changeMs, double _maxValue, double _minValue)
    : changeValue(_changeValue), changeMs(_changeMs), maxValue(_maxValue), minValue(_minValue)
{
}
void T_curve::request(double value)
{
    requestedValue = value;
}
void T_curve::instant(double value)
{
    request(value);
    outputValue = value;
}
double T_curve::calculate()
{
    if (requestedValue == outputValue)
        return outputValue;
    if (requestedValue > outputValue)
        outputValue += changeValue;
    else if (requestedValue < outputValue)
        outputValue -= changeValue;
    //limit Val
    if (outputValue > maxValue)
        outputValue = maxValue;
    if (outputValue < minValue)
        outputValue = minValue;
        
    if (std::abs(requestedValue - outputValue) < changeValue)
        outputValue = requestedValue; //if error is within changeValue then set to it exactly to stabilize/remove oscillation from the output
    return outputValue;
}
double T_curve::integrateToValue(double value)
{
    double h = outputValue;                                      //rpm
    double b = outputValue / changeValue * changeMs / 1000 / 60; //RPM/RPM *ms
    return h * b / 2;
}
double T_curve::getRequested()
{
    return requestedValue;
}
double T_curve::getChangeValue()
{
    return changeValue;
}
int T_curve::getChangeMs()
{
    return changeMs;
}
double T_curve::getMaxValue()
{
    return maxValue;
}
double T_curve::getMinValue()
{
    return minValue;
}
double T_curve::getOutput()
{
    return outputValue;
}