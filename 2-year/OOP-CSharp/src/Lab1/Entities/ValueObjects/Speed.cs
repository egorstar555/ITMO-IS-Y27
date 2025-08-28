namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.ValueObjects;

public class Speed
{
    public double Value { get; }

    public Speed(double value)
    {
        if (value < 0)
        {
            Value = 0;
        }

        Value = value;
    }

    public static bool operator >(Speed speed1, Speed speed2)
    {
        return speed1.Value > speed2.Value;
    }

    public static bool operator <(Speed speed1, Speed speed2)
    {
        return speed1.Value < speed2.Value;
    }
}