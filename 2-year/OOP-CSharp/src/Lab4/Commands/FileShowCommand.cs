namespace Itmo.ObjectOrientedProgramming.Lab4.Commands;

public class FileShowCommand : ICommand
{
    private readonly string _path;

    private readonly Connection _connection;

    public string Mode { get; set; }

    public FileShowCommand(string path, Connection connection)
    {
        _connection = connection;
        _path = Path.Combine(_connection.CurrentPath, path);
        Mode = "console";
    }

    public void Execute()
    {
        _connection.FileSystem.ShowFile(_path, _connection.Writer);
    }

    public bool Equals(ICommand? other)
    {
        if (other is null) return false;
        if (ReferenceEquals(this, other)) return true;
        if (other.GetType() != GetType()) return false;
        return Equals((FileShowCommand)other);
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(_path, _connection, Mode);
    }

    protected bool Equals(FileShowCommand other)
    {
        return _path == other._path && _connection.Equals(other._connection) && Mode == other.Mode;
    }
}