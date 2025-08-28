using Itmo.ObjectOrientedProgramming.Lab4.TreeList.Visitors;
using Itmo.ObjectOrientedProgramming.Lab4.Writer;

namespace Itmo.ObjectOrientedProgramming.Lab4.FileSystem;

public class NullFileSystem : IFileSystem
{
    public void TreeList(string path, IFileSystemComponentVisitor visitor) { }

    public void ShowFile(string filePath, IWriter writer) { }

    public void MoveFile(string sourcePath, string destinationPath) { }

    public void CopyFile(string sourcePath, string destinationPath) { }

    public void DeleteFile(string filePath) { }

    public void RenameFile(string filePath, string newName) { }
}