using Itmo.ObjectOrientedProgramming.Lab4.Commands;

namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers.OptionalParametersHandlers;

public class TreeListOptionalDepthHandler : OptionalParameterHandlerBase<TreeListCommand>
{
    public override void Handle(IEnumerator<string> request, TreeListCommand command)
    {
        if (request.Current is not "-d")
            return;

        if (request.MoveNext() is false)
            return;

        if (!int.TryParse(request.Current, out int depth))
            depth = 1;

        command.Depth = depth;
    }
}