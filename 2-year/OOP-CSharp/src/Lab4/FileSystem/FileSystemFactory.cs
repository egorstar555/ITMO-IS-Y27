namespace Itmo.ObjectOrientedProgramming.Lab4.FileSystem;

public class FileSystemFactory
{
    public static IFileSystem Create(string mode)
    {
        return mode switch
        {
            "local" => new LocalFileSystem(),
            _ => new LocalFileSystem(),
        };
    }
}