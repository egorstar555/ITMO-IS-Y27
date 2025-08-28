using Itmo.ObjectOrientedProgramming.Lab4.Commands;

namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers;

public class TreeGotoCommandHandler : CommandHandlerBase
{
    public override ICommand? Handle(IEnumerator<string> request, Connection connection)
    {
        if (request.Current is not "tree")
            return Next?.Handle(request, connection);

        if (request.MoveNext() is false)
            return null;

        if (request.Current is not "goto")
        {
            request.Reset();
            request.MoveNext();
            return Next?.Handle(request, connection);
        }

        if (request.MoveNext() is false)
            return null;

        string path = request.Current;

        return new TreeGotoCommand(path, connection);
    }
}