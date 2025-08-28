using Itmo.ObjectOrientedProgramming.Lab2.Entities;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Repository;

public class EducationalProgramRepository : RepositoryBase<EducationalProgram>
{
    public EducationalProgramRepository() : base() { }

    public EducationalProgramRepository(IEnumerable<EducationalProgram> users) : base(users) { }
}