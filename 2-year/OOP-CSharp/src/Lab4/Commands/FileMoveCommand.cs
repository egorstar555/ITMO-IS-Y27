namespace Itmo.ObjectOrientedProgramming.Lab4.Commands;

public class FileMoveCommand : ICommand
{
    private readonly string _sourcePath;

    private readonly string _destinationPath;

    private readonly Connection _connection;

    public FileMoveCommand(string sourcePath, string destinationPath, Connection connection)
    {
        _connection = connection;
        _sourcePath = Path.Combine(_connection.CurrentPath, sourcePath);
        _destinationPath = Path.Combine(_connection.CurrentPath, destinationPath);
    }

    public void Execute()
    {
        _connection.FileSystem.MoveFile(_sourcePath, _destinationPath);
    }

    public bool Equals(ICommand? other)
    {
        if (other is null) return false;
        if (ReferenceEquals(this, other)) return true;
        if (other.GetType() != GetType()) return false;
        return Equals((FileMoveCommand)other);
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(_sourcePath, _destinationPath, _connection);
    }

    protected bool Equals(FileMoveCommand other)
    {
        return _sourcePath == other._sourcePath && _destinationPath == other._destinationPath &&
               _connection.Equals(other._connection);
    }
}