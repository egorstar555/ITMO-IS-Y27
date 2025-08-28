using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messengers;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Destination;

public class DestinationMessenger : IDestination
{
    private readonly IMessengerAdapter _messengerAdapter;

    public DestinationMessenger(IMessengerAdapter messengerAdapter)
    {
        _messengerAdapter = messengerAdapter;
    }

    public void ReceiveMessage(Message message)
    {
        _messengerAdapter.ReceiveMessage(message);
    }
}