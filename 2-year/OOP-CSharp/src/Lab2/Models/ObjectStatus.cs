using Itmo.ObjectOrientedProgramming.Lab2.Entities.Subjects;

namespace Itmo.ObjectOrientedProgramming.Lab2.Models;

public abstract record ObjectStatus
{
    private ObjectStatus() { }

    public sealed record Successful : ObjectStatus;

    public sealed record IncorrectNumberOfPoints : ObjectStatus;

    public sealed record SubjectSuccess(Subject Subject) : ObjectStatus;

    public sealed record ChangeEntitiesNotByAuthor : ObjectStatus;
}