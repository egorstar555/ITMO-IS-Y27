using Itmo.ObjectOrientedProgramming.Lab4.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Handlers.OptionalParametersHandlers;

namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers;

public class FileShowCommandHandler : CommandHandlerBase
{
    public OptionalParameterHandlerBase<FileShowCommand>? Handler { get; private set; }

    public override ICommand? Handle(IEnumerator<string> request, Connection connection)
    {
        if (request.Current is not "file")
            return Next?.Handle(request, connection);

        if (request.MoveNext() is false)
            return null;

        if (request.Current is not "show")
        {
            request.Reset();
            request.MoveNext();
            return Next?.Handle(request, connection);
        }

        if (request.MoveNext() is false)
            return null;

        string path = request.Current;

        var command = new FileShowCommand(path, connection);

        if (request.MoveNext() is false)
            return command;

        Handler?.Handle(request, command);

        return command;
    }

    public FileShowCommandHandler AddOptional(OptionalParameterHandlerBase<FileShowCommand> handler)
    {
        if (Handler is null)
        {
            Handler = handler;
        }

        Handler.AddNext(handler);
        return this;
    }
}