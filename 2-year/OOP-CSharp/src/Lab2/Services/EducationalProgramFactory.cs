using Itmo.ObjectOrientedProgramming.Lab2.Entities;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.LaboratoryWorks;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.LectureMaterial;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Subjects;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services;

public class EducationalProgramFactory
{
    private readonly User _author;

    public EducationalProgramFactory(User author)
    {
        _author = author;
    }

    public LaboratoryWork.Builder CreateLaboratoryWorkBuilder()
    {
        return LaboratoryWork.CreateBuilder.WithAuthor(_author);
    }

    public LectureMaterials.Builder CreateLectureMaterialsBuilder()
    {
        return LectureMaterials.CreateBuilder.WithAuthor(_author);
    }

    public Subject.Builder CreateSubjectBuilder()
    {
        return Subject.CreateBuilder.WithAuthor(_author);
    }
}