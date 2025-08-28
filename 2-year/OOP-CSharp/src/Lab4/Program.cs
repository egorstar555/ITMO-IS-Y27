using Itmo.ObjectOrientedProgramming.Lab4.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Handlers;
using Itmo.ObjectOrientedProgramming.Lab4.Handlers.OptionalParametersHandlers;

namespace Itmo.ObjectOrientedProgramming.Lab4;

public class Program
{
    public static void Run(IEnumerable<string> args, Connection connection)
    {
        using IEnumerator<string> request = args.GetEnumerator();

        ICommandHandler handler = new ConnectCommandHandler()
            .AddNext(new DisconnectCommandHandler()
                .AddNext(new FileCopyCommandHandler()
                    .AddNext(new FileDeleteCommandHandler()
                        .AddNext(new FileMoveCommandHandler()
                            .AddNext(new FileRenameCommandHandler()
                                .AddNext(new FileShowCommandHandler()
                                    .AddOptional(new FileShowOptionalModeHandler())
                                    .AddNext(new TreeGotoCommandHandler()
                                        .AddNext(new TreeListCommandHandler()
                                            .AddOptional(new TreeListOptionalDepthHandler())))))))));
        while (request.MoveNext())
        {
            ICommand? nextCommand = handler.Handle(request, connection);

            if (nextCommand is not null)
            {
                nextCommand.Execute();
            }
        }
    }

    public static void Main()
    {
        string? line = string.Empty;
        var connection = new Connection();
        while (true)
        {
            line = Console.ReadLine();
            if (line is not null)
                Run(line.Split(" "), connection);
        }
    }
}