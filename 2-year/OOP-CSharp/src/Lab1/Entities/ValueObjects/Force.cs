namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.ValueObjects;

public class Force(double value)
{
    public double Value { get; } = value;

    public static bool operator >(Force a, Force b)
    {
        return a.Value > b.Value;
    }

    public static bool operator <(Force a, Force b)
    {
        return a.Value < b.Value;
    }
}