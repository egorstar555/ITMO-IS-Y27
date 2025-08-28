namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Messengers;

public class MyMessenger : IMessenger
{
    public void WriteMessage(string message)
    {
        Console.WriteLine($"MyMessenger - {message}");
    }
}