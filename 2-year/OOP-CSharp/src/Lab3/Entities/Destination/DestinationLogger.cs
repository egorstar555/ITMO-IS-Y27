using Itmo.ObjectOrientedProgramming.Lab3.Entities.Loggers;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Destination;

public class DestinationLogger : IDestination
{
    private readonly IDestination _destination;
    private readonly ILogger _logger;

    public DestinationLogger(IDestination destination, ILogger logger)
    {
        _destination = destination;
        _logger = logger;
    }

    public void ReceiveMessage(Message message)
    {
        _logger.Log($"Received message: {message}");
        _destination.ReceiveMessage(message);
    }
}