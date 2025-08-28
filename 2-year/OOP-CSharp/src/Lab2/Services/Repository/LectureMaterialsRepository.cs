using Itmo.ObjectOrientedProgramming.Lab2.Entities.LectureMaterial;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Repository;

public class LectureMaterialsRepository : RepositoryBase<ILectureMaterials>
{
    public LectureMaterialsRepository() : base() { }

    public LectureMaterialsRepository(IEnumerable<ILectureMaterials> lectureMaterials) : base(lectureMaterials) { }
}