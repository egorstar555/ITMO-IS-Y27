namespace Itmo.ObjectOrientedProgramming.Lab2.Entities;

public class Identifier
{
    public Guid Value { get; }

    public Identifier()
    {
        Value = Guid.NewGuid();
    }

    public static bool operator ==(Identifier left, Identifier right)
    {
        return left.Value == right.Value;
    }

    public static bool operator !=(Identifier left, Identifier right)
    {
        return !(left == right);
    }

    public override bool Equals(object? obj)
    {
        if (obj is null) return false;
        if (ReferenceEquals(this, obj)) return true;
        if (obj.GetType() != GetType()) return false;
        return Equals((Identifier)obj);
    }

    public override int GetHashCode()
    {
        return Value.GetHashCode();
    }

    protected bool Equals(Identifier other)
    {
        return Value.Equals(other.Value);
    }
}