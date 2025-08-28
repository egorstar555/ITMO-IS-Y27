namespace Itmo.ObjectOrientedProgramming.Lab4.Commands;

public interface ICommand : IEquatable<ICommand>
{
    void Execute();
}