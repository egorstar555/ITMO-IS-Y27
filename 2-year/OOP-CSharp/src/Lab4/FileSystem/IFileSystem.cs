using Itmo.ObjectOrientedProgramming.Lab4.TreeList.Visitors;
using Itmo.ObjectOrientedProgramming.Lab4.Writer;

namespace Itmo.ObjectOrientedProgramming.Lab4.FileSystem;

public interface IFileSystem
{
    void TreeList(string path, IFileSystemComponentVisitor visitor);

    void ShowFile(string filePath, IWriter writer);

    void MoveFile(string sourcePath, string destinationPath);

    void CopyFile(string sourcePath, string destinationPath);

    void DeleteFile(string filePath);

    void RenameFile(string filePath, string newName);
}