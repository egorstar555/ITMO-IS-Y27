namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Destination;

public class DestinationFilter : IDestination
{
    private readonly IDestination _destination;

    private readonly int _importance;

    public DestinationFilter(IDestination destination, int importance)
    {
        _importance = importance;
        _destination = destination;
    }

    public void ReceiveMessage(Message message)
    {
        if (message.Importance >= _importance)
            _destination.ReceiveMessage(message);
    }
}