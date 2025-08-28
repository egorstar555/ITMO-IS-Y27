using Itmo.ObjectOrientedProgramming.Lab4.TreeList.Visitors;

namespace Itmo.ObjectOrientedProgramming.Lab4.TreeList.FileSystemStructure;

public class FileFileSystemComponent : IFileSystemComponent
{
    public FileFileSystemComponent(string name)
    {
        Name = name;
    }

    public string Name { get; }

    public void Accept(IFileSystemComponentVisitor visitor)
    {
        visitor.Visit(this);
    }
}