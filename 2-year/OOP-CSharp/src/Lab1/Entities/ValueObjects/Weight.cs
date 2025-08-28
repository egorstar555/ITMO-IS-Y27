namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.ValueObjects;

public class Weight
{
    public double Value { get; }

    public Weight(double value)
    {
        if (value < 0)
        {
            throw new ArgumentException("Weight cannot be negative.", nameof(value));
        }

        Value = value;
    }
}