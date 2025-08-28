using Itmo.ObjectOrientedProgramming.Lab4.Commands;

namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers;

public class FileDeleteCommandHandler : CommandHandlerBase
{
    public override ICommand? Handle(IEnumerator<string> request, Connection connection)
    {
        if (request.Current is not "file")
            return Next?.Handle(request, connection);

        if (request.MoveNext() is false)
            return null;

        if (request.Current is not "delete")
        {
            request.Reset();
            request.MoveNext();
            return Next?.Handle(request, connection);
        }

        if (request.MoveNext() is false)
            return null;

        string path = request.Current;

        return new FileDeleteCommand(path, connection);
    }
}