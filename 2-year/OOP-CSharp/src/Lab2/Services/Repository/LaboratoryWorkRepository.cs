using Itmo.ObjectOrientedProgramming.Lab2.Entities.LaboratoryWorks;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Repository;

public class LaboratoryWorkRepository : RepositoryBase<ILaboratoryWork>
{
    public LaboratoryWorkRepository() : base() { }

    public LaboratoryWorkRepository(IEnumerable<ILaboratoryWork> laboratoryWorks) : base(laboratoryWorks) { }
}