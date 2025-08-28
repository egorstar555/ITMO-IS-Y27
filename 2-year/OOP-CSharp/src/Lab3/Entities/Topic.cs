using Itmo.ObjectOrientedProgramming.Lab3.Entities.Destination;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities;

public class Topic
{
    public string Name { get; }

    private readonly List<IDestination> _destination;

    public Topic(string name, IEnumerable<IDestination> destination)
    {
        Name = name;
        _destination = destination.ToList();
    }

    public void Send(Message message)
    {
        foreach (IDestination destination in _destination)
        {
            destination.ReceiveMessage(message);
        }
    }
}