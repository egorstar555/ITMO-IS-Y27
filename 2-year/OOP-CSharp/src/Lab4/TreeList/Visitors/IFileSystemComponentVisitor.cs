using Itmo.ObjectOrientedProgramming.Lab4.TreeList.FileSystemStructure;

namespace Itmo.ObjectOrientedProgramming.Lab4.TreeList.Visitors;

public interface IFileSystemComponentVisitor
{
    void Visit(FileFileSystemComponent component);

    void Visit(DirectoryFileSystemComponent component);
}