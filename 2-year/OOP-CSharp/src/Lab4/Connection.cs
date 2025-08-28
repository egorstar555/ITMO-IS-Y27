using Itmo.ObjectOrientedProgramming.Lab4.FileSystem;
using Itmo.ObjectOrientedProgramming.Lab4.Writer;

namespace Itmo.ObjectOrientedProgramming.Lab4;

public class Connection
{
    public string CurrentPath { get; set; }

    public IFileSystem FileSystem { get; set; }

    public IWriter Writer { get; set; }

    public Connection(IFileSystem fileSystem, string currentPath, IWriter writer)
    {
        CurrentPath = currentPath;
        FileSystem = fileSystem;
        Writer = writer;
    }

    public Connection() : this(new LocalFileSystem(), string.Empty, new ConsoleWriter()) { }

    public Connection(IFileSystem fileSystem) : this(fileSystem, string.Empty, new ConsoleWriter()) { }

    public Connection(IFileSystem fileSystem, string currentPath) : this(fileSystem, currentPath, new ConsoleWriter()) { }
}