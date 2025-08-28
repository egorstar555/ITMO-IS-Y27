namespace Itmo.ObjectOrientedProgramming.Lab4.Commands;

public class TreeGotoCommand : ICommand
{
    private readonly string _path;

    private readonly Connection _connection;

    public TreeGotoCommand(string path, Connection connection)
    {
        _connection = connection;
        _path = Path.Combine(_connection.CurrentPath, path);
    }

    public void Execute()
    {
        _connection.CurrentPath = _path;
    }

    public bool Equals(ICommand? other)
    {
        if (other is null) return false;
        if (ReferenceEquals(this, other)) return true;
        if (other.GetType() != GetType()) return false;
        return Equals((TreeGotoCommand)other);
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(_path, _connection);
    }

    protected bool Equals(TreeGotoCommand other)
    {
        return _path == other._path && _connection.Equals(other._connection);
    }
}