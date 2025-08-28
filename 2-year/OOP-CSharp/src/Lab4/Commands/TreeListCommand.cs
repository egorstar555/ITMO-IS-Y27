using Itmo.ObjectOrientedProgramming.Lab4.TreeList.Visitors;
using Itmo.ObjectOrientedProgramming.Lab4.Writer;

namespace Itmo.ObjectOrientedProgramming.Lab4.Commands;

public class TreeListCommand : ICommand
{
    private readonly Connection _connection;

    public int Depth { get; set; }

    public TreeListCommand(Connection connection)
    {
        _connection = connection;
        Depth = 1;
    }

    public TreeListCommand(Connection connection, int depth)
    {
        _connection = connection;
        Depth = depth;
    }

    public void Execute()
    {
        IFileSystemComponentVisitor visitor = new ConsoleVisitor(new ConsoleWriter(), Depth, "F ", "D ", "   ");
        _connection.FileSystem.TreeList(_connection.CurrentPath, visitor);
    }

    public bool Equals(ICommand? other)
    {
        if (other is null) return false;
        if (ReferenceEquals(this, other)) return true;
        if (other.GetType() != GetType()) return false;
        return Equals((TreeListCommand)other);
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(_connection, Depth);
    }

    protected bool Equals(TreeListCommand other)
    {
        return _connection.Equals(other._connection) && Depth == other.Depth;
    }
}