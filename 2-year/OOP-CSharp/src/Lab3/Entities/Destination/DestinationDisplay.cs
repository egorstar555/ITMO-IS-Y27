using Itmo.ObjectOrientedProgramming.Lab3.Entities.Devices;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Destination;

public class DestinationDisplay : IDestination
{
    private readonly IDeviceAdapter _deviceAdapter;

    public DestinationDisplay(IDeviceAdapter deviceAdapter)
    {
        _deviceAdapter = deviceAdapter;
    }

    public void ReceiveMessage(Message message)
    {
        _deviceAdapter.WriteMessage(message);
    }
}
