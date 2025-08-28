namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Destination;

public class DestinationGroup : IDestination
{
    private readonly List<IDestination> _destinations;

    public DestinationGroup(IEnumerable<IDestination> destinations)
    {
        _destinations = destinations.ToList();
    }

    public void ReceiveMessage(Message message)
    {
        foreach (IDestination destination in _destinations)
        {
            destination.ReceiveMessage(message);
        }
    }
}