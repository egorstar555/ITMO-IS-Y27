using Itmo.ObjectOrientedProgramming.Lab4.FileSystem;

namespace Itmo.ObjectOrientedProgramming.Lab4.Commands;

public class ConnectCommand : ICommand
{
    private readonly string _address;

    private readonly IFileSystem _fileSystem;

    private readonly Connection _connection;

    public ConnectCommand(string address, Connection connection)
    {
        _address = address;
        _fileSystem = new LocalFileSystem();
        _connection = connection;
    }

    public ConnectCommand(string address, IFileSystem fileSystem, Connection connection)
    {
        _address = address;
        _fileSystem = fileSystem;
        _connection = connection;
    }

    public void Execute()
    {
        _connection.CurrentPath = _address;
        _connection.FileSystem = _fileSystem;
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(_address, _fileSystem, _connection);
    }

    public bool Equals(ICommand? other)
    {
        if (other is null) return false;
        if (ReferenceEquals(this, other)) return true;
        if (other.GetType() != GetType()) return false;
        return Equals((ConnectCommand)other);
    }

    protected bool Equals(ConnectCommand other)
    {
        return _address == other._address && _fileSystem.Equals(other._fileSystem) &&
               _connection.Equals(other._connection);
    }
}