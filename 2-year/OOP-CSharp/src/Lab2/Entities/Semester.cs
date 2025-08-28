namespace Itmo.ObjectOrientedProgramming.Lab2.Entities;

public class Semester
{
    public Identifier Id { get; set; }

    public Semester(Identifier id)
    {
        Id = id;
    }
}