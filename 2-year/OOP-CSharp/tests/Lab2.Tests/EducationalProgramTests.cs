using Itmo.ObjectOrientedProgramming.Lab2.Entities;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.LaboratoryWorks;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.LectureMaterial;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Subjects;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.SubjectTypes;
using Itmo.ObjectOrientedProgramming.Lab2.Models;
using Itmo.ObjectOrientedProgramming.Lab2.Services;
using Itmo.ObjectOrientedProgramming.Lab2.Services.Repository;
using Xunit;

namespace Lab2.Tests;

public class EducationalProgramTests
{
    [Fact]
    public void NotAuthorTryToChangeLabWork_Failure()
    {
        // Arrange
        var id = new Identifier();
        var id2 = new Identifier();
        var user = new User(id, "bob");
        var user2 = new User(id2, "tom");

        LaboratoryWork labWork = LaboratoryWork.CreateBuilder
            .WithAuthor(user)
            .WithName("OOP")
            .WithDescription("OOP Description")
            .WithId(id)
            .WithPoints(10)
            .WithEvaluationCriteria("good")
            .Build();

        // Act
        labWork.GetChanger(user2).SetName("tom");

        // Assert
        Assert.IsType<ObjectStatus.ChangeEntitiesNotByAuthor>(labWork.GetLastStatus());
    }

    [Fact]
    public void CopyShouldContainParentID_Success()
    {
        // Arrange
        var id = new Identifier();
        var user = new User(id, "bob");

        LaboratoryWork labWork = LaboratoryWork.CreateBuilder
            .WithAuthor(user)
            .WithName("OOP")
            .WithDescription("OOP Description")
            .WithId(id)
            .WithPoints(10)
            .WithEvaluationCriteria("good")
            .Build();

        // Act
        LaboratoryWork labWork2 = labWork.Clone();

        // Assert
        Assert.Equal(id, labWork2.ParentId);
    }

    [Fact]
    public void TryToCreateSubjectWithNo100PointsInSum_Failure()
    {
        // Arrange
        var id = new Identifier();
        var user = new User(id, "bob");

        LaboratoryWork labWork = LaboratoryWork.CreateBuilder
            .WithAuthor(user)
            .WithName("OOP")
            .WithDescription("OOP Description")
            .WithId(id)
            .WithPoints(10)
            .WithEvaluationCriteria("good")
            .Build();

        ObjectStatus status = Subject.CreateBuilder
            .WithAuthor(user)
            .WithName("oop")
            .WithId(id)
            .WithTestType(new Exam(20))
            .AddLaboratoryWork(labWork)
            .AddSemester(new Semester(id))
            .Build();

        // Act

        // Assert
        Assert.IsType<ObjectStatus.IncorrectNumberOfPoints>(status);
    }

    [Fact]
    public void CreateSubjectWith100PointsInSum_Success()
    {
        // Arrange
        var id = new Identifier();
        var user = new User(id, "bob");

        LaboratoryWork labWork = LaboratoryWork.CreateBuilder
            .WithAuthor(user)
            .WithName("OOP")
            .WithDescription("OOP Description")
            .WithId(id)
            .WithPoints(80)
            .WithEvaluationCriteria("good")
            .Build();

        ObjectStatus status = Subject.CreateBuilder
            .WithAuthor(user)
            .WithName("oop")
            .WithId(id)
            .WithTestType(new Exam(20))
            .AddLaboratoryWork(labWork)
            .AddSemester(new Semester(id))
            .Build();

        // Act

        // Assert
        Assert.IsType<ObjectStatus.SubjectSuccess>(status);
    }

    [Fact]
    public void AuthorCanChangeLabWork_Success()
    {
        // Arrange
        var id = new Identifier();
        var user = new User(id, "bob");

        LaboratoryWork labWork = LaboratoryWork.CreateBuilder
            .WithAuthor(user)
            .WithName("OOP")
            .WithDescription("OOP Description")
            .WithId(id)
            .WithPoints(10)
            .WithEvaluationCriteria("good")
            .Build();

        // Act
        labWork.GetChanger(user).SetDescription("OOP on C# Description");

        // Assert
        Assert.Equal("OOP on C# Description", labWork.Description);
    }

    [Fact]
    public void AuthorCanChangeLectureMaterial_Success()
    {
        // Arrange
        var id = new Identifier();
        var user = new User(id, "bob");

        LectureMaterials lectureMaterial = LectureMaterials.CreateBuilder
            .WithAuthor(user)
            .WithName("OOP")
            .WithDescription("OOP Description")
            .WithId(id)
            .WithContent("good")
            .Build();

        // Act
        lectureMaterial.GetChanger(user).SetDescription("OOP on C# Description");

        // Assert
        Assert.Equal("OOP on C# Description", lectureMaterial.Description);
    }

    [Fact]
    public void NotNeedToIndicateAuthorForEveryone_Success()
    {
        // Arrange
        var id = new Identifier();
        var user = new User(id, "bob");

        var educationalProgramFactory = new EducationalProgramFactory(user);

        LectureMaterials.Builder lectures = educationalProgramFactory.CreateLectureMaterialsBuilder();
        LaboratoryWork.Builder labWorks = educationalProgramFactory.CreateLaboratoryWorkBuilder();

        // Act
        LectureMaterials lectureMaterial = lectures
            .WithName("OOP")
            .WithDescription("OOP Description")
            .WithId(id)
            .WithContent("good")
            .Build();

        LaboratoryWork labWork = labWorks
            .WithName("OOP")
            .WithDescription("OOP Description")
            .WithId(id)
            .WithEvaluationCriteria("good")
            .WithPoints(10)
            .Build();

        // Assert
        Assert.Equal(labWork.Author, lectureMaterial.Author);
    }

    [Fact]
    public void CopyCloneShouldContainParentID_Success()
    {
        // Arrange
        var id = new Identifier();
        var user = new User(id, "bob");

        LaboratoryWork labWork = LaboratoryWork.CreateBuilder
            .WithAuthor(user)
            .WithName("OOP")
            .WithDescription("OOP Description")
            .WithId(id)
            .WithPoints(10)
            .WithEvaluationCriteria("good")
            .Build();

        // Act
        LaboratoryWork labWork2 = labWork.Clone();
        LaboratoryWork labWork3 = labWork2.Clone();

        // Assert
        Assert.Equal(labWork3.ParentId, labWork2.Id);
    }

    [Fact]
    public void ShouldFindLabWorkInRepoById_Success()
    {
        // Arrange
        var labWorkRepo = new LaboratoryWorkRepository();
        var id = new Identifier();
        var user = new User(id, "bob");

        LaboratoryWork labWork = LaboratoryWork.CreateBuilder
            .WithAuthor(user)
            .WithName("OOP")
            .WithDescription("OOP Description")
            .WithId(id)
            .WithPoints(10)
            .WithEvaluationCriteria("good")
            .Build();

        // Act
        labWorkRepo.Insert(labWork);

        // Assert
        Assert.Equal(labWorkRepo.FindById(labWork.Id), labWork);
    }

    [Fact]
    public void DontFindDeletedLabWorkInRepoById_Success()
    {
        // Arrange
        var labWorkRepo = new LaboratoryWorkRepository();
        var id = new Identifier();
        var user = new User(id, "bob");

        LaboratoryWork labWork = LaboratoryWork.CreateBuilder
            .WithAuthor(user)
            .WithName("OOP")
            .WithDescription("OOP Description")
            .WithId(id)
            .WithPoints(10)
            .WithEvaluationCriteria("good")
            .Build();

        // Act
        labWorkRepo.Insert(labWork);
        labWorkRepo.Delete(labWork.Id);

        // Assert
        Assert.Null(labWorkRepo.FindById(labWork.Id));
    }
}