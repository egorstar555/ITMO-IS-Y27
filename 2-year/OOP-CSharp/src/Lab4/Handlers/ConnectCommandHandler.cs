using Itmo.ObjectOrientedProgramming.Lab4.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.FileSystem;

namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers;

public class ConnectCommandHandler : CommandHandlerBase
{
    public override ICommand? Handle(IEnumerator<string> request, Connection connection)
    {
        if (request.Current is not "connect")
            return Next?.Handle(request, connection);

        if (request.MoveNext() is false)
            return null;

        string address = request.Current;

        if (request.MoveNext() is false)
            return null;

        if (request.Current is not "-m")
            return new ConnectCommand(address, connection);

        if (request.MoveNext() is false)
            return null;

        IFileSystem fileSystem = FileSystemFactory.Create(request.Current);

        return new ConnectCommand(address, fileSystem, connection);
    }
}