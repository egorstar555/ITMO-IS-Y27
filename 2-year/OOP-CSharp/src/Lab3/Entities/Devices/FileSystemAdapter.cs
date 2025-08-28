namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Devices;

public class FileSystemAdapter : IDeviceAdapter
{
    private readonly IFileWriter _fileSystem;

    public FileSystemAdapter(IFileWriter fileSystem)
    {
        _fileSystem = fileSystem;
    }

    public void WriteMessage(Message message)
    {
        _fileSystem.WriteToFile($"Title: {message.Title}\n" +
                                $"Body: {message.Body}\n" +
                                $"Importance: {message.Importance}");
    }
}