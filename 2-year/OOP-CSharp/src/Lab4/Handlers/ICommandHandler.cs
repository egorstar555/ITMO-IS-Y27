using Itmo.ObjectOrientedProgramming.Lab4.Commands;

namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers;

public interface ICommandHandler
{
    ICommandHandler AddNext(ICommandHandler handler);

    ICommand? Handle(IEnumerator<string> request, Connection connection);
}