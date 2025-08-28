using Itmo.ObjectOrientedProgramming.Lab4.Commands;

namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers.OptionalParametersHandlers;

public class FileShowOptionalModeHandler : OptionalParameterHandlerBase<FileShowCommand>
{
    public override void Handle(IEnumerator<string> request, FileShowCommand command)
    {
        if (request.Current is not "-m")
            return;

        if (request.MoveNext() is false)
            return;

        string mode = request.Current;

        command.Mode = mode;
    }
}