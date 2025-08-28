using Itmo.ObjectOrientedProgramming.Lab2.Entities.Interfaces;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities;

public class User : IUniqueIdentifier
{
    public Identifier Id { get; }

    public string Name { get; }

    public User(Identifier id, string name)
    {
        Id = id;
        Name = name;
    }

    public User Clone()
    {
        return new User(Id, Name);
    }
}