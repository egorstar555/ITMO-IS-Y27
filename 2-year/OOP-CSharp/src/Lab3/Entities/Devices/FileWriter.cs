namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Devices;

public class FileWriter : IFileWriter
{
    private readonly string _filePath;

    public FileWriter(string filePath)
    {
        _filePath = filePath;
    }

    public void WriteToFile(string content)
    {
        File.WriteAllText(_filePath, content);
    }
}