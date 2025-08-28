using Itmo.ObjectOrientedProgramming.Lab4.FileSystem;

namespace Itmo.ObjectOrientedProgramming.Lab4.Commands;

public class DisconnectCommand : ICommand
{
    private readonly Connection _connection;

    public DisconnectCommand(Connection connection)
    {
        _connection = connection;
    }

    public void Execute()
    {
        _connection.CurrentPath = string.Empty;
        _connection.FileSystem = new NullFileSystem();
    }

    public bool Equals(ICommand? other)
    {
        if (other is null) return false;
        if (ReferenceEquals(this, other)) return true;
        if (other.GetType() != GetType()) return false;
        return Equals((DisconnectCommand)other);
    }

    public override int GetHashCode()
    {
        return _connection.GetHashCode();
    }

    protected bool Equals(DisconnectCommand other)
    {
        return _connection.Equals(other._connection);
    }
}