using Itmo.ObjectOrientedProgramming.Lab4.Commands;

namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers;

public class FileRenameCommandHandler : CommandHandlerBase
{
    public override ICommand? Handle(IEnumerator<string> request, Connection connection)
    {
        if (request.Current is not "file")
            return Next?.Handle(request, connection);

        if (request.MoveNext() is false)
            return null;

        if (request.Current is not "rename")
        {
            request.Reset();
            request.MoveNext();
            return Next?.Handle(request, connection);
        }

        if (request.MoveNext() is false)
            return null;

        string path = request.Current;

        if (request.MoveNext() is false)
            return null;

        string name = request.Current;

        return new FileRenameCommand(path, name, connection);
    }
}