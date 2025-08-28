namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.ValueObjects;

public class Accuracy
{
    public double Value { get; }

    public Accuracy(double value)
    {
        if (value < 0)
        {
            throw new ArgumentException("Accuracy cannot be negative.", nameof(value));
        }

        Value = value;
    }
}