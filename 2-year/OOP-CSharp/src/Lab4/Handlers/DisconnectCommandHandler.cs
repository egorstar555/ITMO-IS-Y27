using Itmo.ObjectOrientedProgramming.Lab4.Commands;

namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers;

public class DisconnectCommandHandler : CommandHandlerBase
{
    public override ICommand? Handle(IEnumerator<string> request, Connection connection)
    {
        if (request.Current is not "disconnect")
            return Next?.Handle(request, connection);

        return new DisconnectCommand(connection);
    }
}