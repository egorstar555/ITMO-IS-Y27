namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Devices;

public class Display : IDisplay
{
    public void PrintText(string text)
    {
        Console.WriteLine(text);
    }
}