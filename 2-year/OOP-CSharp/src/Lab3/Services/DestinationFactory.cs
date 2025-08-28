using Itmo.ObjectOrientedProgramming.Lab3.Entities.Destination;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Devices;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Loggers;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messengers;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Users;

namespace Itmo.ObjectOrientedProgramming.Lab3.Services;

public class DestinationFactory
{
    private readonly ILogger? _logger;

    private readonly int _filter;

    public DestinationFactory(ILogger? logger, int filter)
    {
        _logger = logger;
        _filter = filter > 0 ? filter : 0;
    }

    public IDestination CreateUser(IUser user)
    {
        IDestination newUser = new DestinationUser(user);
        return SetDecorators(newUser);
    }

    public IDestination CreateDisplay(IDeviceAdapter deviceAdapter)
    {
        IDestination newDisplay = new DestinationDisplay(deviceAdapter);
        return SetDecorators(newDisplay);
    }

    public IDestination CreateMessenger(IMessengerAdapter messengerAdapter)
    {
        IDestination newMessenger = new DestinationMessenger(messengerAdapter);
        return SetDecorators(newMessenger);
    }

    private IDestination SetDecorators(IDestination destination)
    {
        if (_logger is not null)
        {
            destination = new DestinationLogger(destination, _logger);
        }

        if (_filter != 0)
        {
            destination = new DestinationFilter(destination, _filter);
        }

        return destination;
    }
}