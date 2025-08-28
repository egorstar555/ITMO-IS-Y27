namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.ValueObjects;

public class Distance
{
    public double Value { get; }

    public Distance(double value)
    {
        if (value < 0)
        {
            Value = 0;
        }

        Value = value;
    }
}