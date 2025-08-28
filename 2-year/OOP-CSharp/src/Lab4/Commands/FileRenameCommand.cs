namespace Itmo.ObjectOrientedProgramming.Lab4.Commands;

public class FileRenameCommand : ICommand
{
    private readonly string _path;

    private readonly string _name;

    private readonly Connection _connection;

    public FileRenameCommand(string path, string name, Connection connection)
    {
        _name = name;
        _connection = connection;
        _path = Path.Combine(_connection.CurrentPath, path);
    }

    public void Execute()
    {
        _connection.FileSystem.RenameFile(_path, _name);
    }

    public bool Equals(ICommand? other)
    {
        if (other is null) return false;
        if (ReferenceEquals(this, other)) return true;
        if (other.GetType() != GetType()) return false;
        return Equals((FileRenameCommand)other);
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(_path, _name, _connection);
    }

    protected bool Equals(FileRenameCommand other)
    {
        return _path == other._path && _name == other._name && _connection.Equals(other._connection);
    }
}