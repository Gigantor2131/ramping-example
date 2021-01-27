#pragma once
class T_curve
{
private:
    double changeValue;
    int changeMs;
    double maxValue;
    double minValue;
    double requestedValue, outputValue;

public:
    T_curve(double _changeValue, int _changeMs, double _maxValue, double _minValue);
    void request(double value);
    void instant(double value);
    double calculate();
    double integrateToValue(double value);

    double getRequested();
    double getChangeValue();
    int getChangeMs();
    double getMaxValue();
    double getMinValue();
    double getOutput();
};

