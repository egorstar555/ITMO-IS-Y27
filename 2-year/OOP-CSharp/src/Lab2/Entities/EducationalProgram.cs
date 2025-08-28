using Itmo.ObjectOrientedProgramming.Lab2.Entities.Interfaces;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Subjects;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities;

public class EducationalProgram : IUniqueIdentifier
{
    public Identifier Id { get; }

    public string Name { get; private set; }

    public User Head { get; private set; }

    private readonly List<ISubject> _subjects;

    public EducationalProgram(
        Identifier id,
        string name,
        IEnumerable<ISubject> subjects,
        User head)
    {
        Id = id;
        Name = name;
        Head = head;
        _subjects = subjects.ToList();
    }

    public void AddSubject(ISubject subject)
    {
        _subjects.Add(subject);
    }
}