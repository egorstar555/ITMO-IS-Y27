using Itmo.ObjectOrientedProgramming.Lab2.Entities.Subjects;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Repository;

public class SubjectRepository : RepositoryBase<ISubject>
{
    public SubjectRepository() : base() { }

    public SubjectRepository(IEnumerable<ISubject> subjects) : base(subjects) { }
}