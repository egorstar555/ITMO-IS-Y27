using Itmo.ObjectOrientedProgramming.Lab4.TreeList.Visitors;

namespace Itmo.ObjectOrientedProgramming.Lab4.TreeList.FileSystemStructure;

public interface IFileSystemComponent
{
    string Name { get; }

    void Accept(IFileSystemComponentVisitor visitor);
}