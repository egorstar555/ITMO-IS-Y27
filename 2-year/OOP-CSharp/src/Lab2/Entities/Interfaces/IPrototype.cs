namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Interfaces;

public interface IPrototype<T> where T : IPrototype<T>
{
    T Clone();
}