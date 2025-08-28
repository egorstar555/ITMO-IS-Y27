namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Devices;

public class DisplayDriverAdapter : IDeviceAdapter
{
    private readonly IDisplayDriver _displayDriver;

    public DisplayDriverAdapter(IDisplayDriver displayDriver)
    {
        _displayDriver = displayDriver;
    }

    public void WriteMessage(Message message)
    {
        _displayDriver.WriteText($"Title: {message.Title}\n" +
                                 $"Body: {message.Body}\n" +
                                 $"Importance: {message.Importance}");
    }
}