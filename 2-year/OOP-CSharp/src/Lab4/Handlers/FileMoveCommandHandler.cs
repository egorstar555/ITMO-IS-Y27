using Itmo.ObjectOrientedProgramming.Lab4.Commands;

namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers;

public class FileMoveCommandHandler : CommandHandlerBase
{
    public override ICommand? Handle(IEnumerator<string> request, Connection connection)
    {
        if (request.Current is not "file")
            return Next?.Handle(request, connection);

        if (request.MoveNext() is false)
            return null;

        if (request.Current is not "move")
        {
            request.Reset();
            request.MoveNext();
            return Next?.Handle(request, connection);
        }

        if (request.MoveNext() is false)
            return null;

        string sourcePath = request.Current;

        if (request.MoveNext() is false)
            return null;

        string destinationPath = request.Current;

        return new FileMoveCommand(sourcePath, destinationPath, connection);
    }
}