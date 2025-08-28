namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Loggers;

public class Logger : ILogger
{
    public void Log(string message)
    {
        Console.WriteLine(message);
    }
}