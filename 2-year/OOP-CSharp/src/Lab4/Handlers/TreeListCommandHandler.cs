using Itmo.ObjectOrientedProgramming.Lab4.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Handlers.OptionalParametersHandlers;

namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers;

public class TreeListCommandHandler : CommandHandlerBase
{
    public OptionalParameterHandlerBase<TreeListCommand>? Handler { get; private set; }

    public override ICommand? Handle(IEnumerator<string> request, Connection connection)
    {
        if (request.Current is not "tree")
            return Next?.Handle(request, connection);

        if (request.MoveNext() is false)
            return null;

        if (request.Current is not "list")
        {
            request.Reset();
            request.MoveNext();
            return Next?.Handle(request, connection);
        }

        var command = new TreeListCommand(connection);

        if (request.MoveNext() is false)
            return command;

        Handler?.Handle(request, command);

        return command;
    }

    public TreeListCommandHandler AddOptional(OptionalParameterHandlerBase<TreeListCommand> handler)
    {
        if (Handler is null)
        {
            Handler = handler;
        }

        Handler.AddNext(handler);
        return this;
    }
}