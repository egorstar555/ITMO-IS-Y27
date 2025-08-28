using Itmo.ObjectOrientedProgramming.Lab4.TreeList.FileSystemStructure;
using Itmo.ObjectOrientedProgramming.Lab4.TreeList.Visitors;
using Itmo.ObjectOrientedProgramming.Lab4.Writer;

namespace Itmo.ObjectOrientedProgramming.Lab4.FileSystem;

public class LocalFileSystem : IFileSystem
{
    public void TreeList(string path, IFileSystemComponentVisitor visitor)
    {
        var factory = new FileSystemComponentFactory();
        IFileSystemComponent component = factory.Create(path);

        component.Accept(visitor);
    }

    public void ShowFile(string filePath, IWriter writer)
    {
        string text = File.ReadAllText(filePath);
        writer.Write(text);
    }

    public void MoveFile(string sourcePath, string destinationPath)
    {
        if (File.Exists(sourcePath))
            File.Move(sourcePath, destinationPath);
    }

    public void CopyFile(string sourcePath, string destinationPath)
    {
        if (File.Exists(sourcePath))
            File.Copy(sourcePath, destinationPath);
    }

    public void DeleteFile(string filePath)
    {
        if (File.Exists(filePath))
            File.Delete(filePath);
    }

    public void RenameFile(string filePath, string newName)
    {
        if (File.Exists(filePath))
        {
            File.Move(
                filePath,
                Path.Combine(Path.GetDirectoryName(filePath) ?? throw new InvalidOperationException(), newName));
        }
    }
}