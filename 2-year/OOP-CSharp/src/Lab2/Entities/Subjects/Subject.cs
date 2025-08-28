using Itmo.ObjectOrientedProgramming.Lab2.Entities.Interfaces;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.LaboratoryWorks;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.LectureMaterial;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.SubjectTypes;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Subjects;

public class Subject : ISubject, IPrototype<Subject>
{
    public Identifier Id { get; }

    public string Name { get; private set; }

    public User Author { get; }

    public ISubjectType TestType { get; }

    public Identifier? ParentId { get; private set; }

    public ObjectStatus Status { get; private set; }

    private readonly List<ILaboratoryWork> _laboratoryWorks;

    private readonly List<ILectureMaterials> _lectureMaterials;

    private readonly List<Semester> _semesters;

    private Subject(
        Identifier id,
        string name,
        User user,
        ISubjectType testType,
        IEnumerable<ILaboratoryWork> laboratoryWorks,
        IEnumerable<ILectureMaterials> lectureMaterials,
        IEnumerable<Semester> semesters,
        Identifier? parentId)
    {
        Id = id;
        Name = name;
        Author = user;
        TestType = testType;
        _laboratoryWorks = laboratoryWorks.ToList();
        _lectureMaterials = lectureMaterials.ToList();
        _semesters = semesters.ToList();
        ParentId = parentId;
        Status = new ObjectStatus.Successful();
    }

    public static Builder CreateBuilder => new Builder();

    public Subject Clone()
    {
        return new Subject(
            new Identifier(),
            Name,
            Author,
            TestType,
            new List<ILaboratoryWork>(_laboratoryWorks),
            new List<ILectureMaterials>(_lectureMaterials),
            new List<Semester>(_semesters),
            Id);
    }

    public class Builder
    {
        private readonly List<ILaboratoryWork> _laboratoryWorks = [];

        private readonly List<ILectureMaterials> _lectureMaterials = [];

        private readonly List<Semester> _semesters = [];

        private Identifier? _id;

        private string? _name;

        private User? _author;

        private ISubjectType? _testType;

        public Builder WithId(Identifier id)
        {
            _id = id;
            return this;
        }

        public Builder WithName(string name)
        {
            _name = name;
            return this;
        }

        public Builder WithAuthor(User author)
        {
            _author = author;
            return this;
        }

        public Builder WithTestType(ISubjectType testType)
        {
            _testType = testType;
            return this;
        }

        public Builder AddLaboratoryWork(ILaboratoryWork laboratoryWork)
        {
            _laboratoryWorks.Add(laboratoryWork);
            return this;
        }

        public Builder AddLectureMaterials(ILectureMaterials lectureMaterial)
        {
            _lectureMaterials.Add(lectureMaterial);
            return this;
        }

        public Builder AddSemester(Semester semester)
        {
            _semesters.Add(semester);
            return this;
        }

        public ObjectStatus Build()
        {
            if (_testType is null)
            {
                throw new ArgumentNullException();
            }

            int totalPoints = _laboratoryWorks.Sum(x => x.GetMaxPoints()) + _testType.GetPoints();
            if (totalPoints != Constants.MaxPoints)
            {
                return new ObjectStatus.IncorrectNumberOfPoints();
            }

            var subject = new Subject(
                _id ?? throw new ArgumentNullException(),
                _name ?? throw new ArgumentNullException(),
                _author ?? throw new ArgumentNullException(),
                _testType,
                _laboratoryWorks,
                _lectureMaterials,
                _semesters,
                null);

            return new ObjectStatus.SubjectSuccess(subject);
        }
    }

    public ChangeSubject GetChanger(User author)
    {
        Status = new ObjectStatus.Successful();
        if (author != Author)
        {
            Status = new ObjectStatus.ChangeEntitiesNotByAuthor();
        }

        return new ChangeSubject(author, this);
    }

    public class ChangeSubject
    {
        private readonly User _author;
        private readonly Subject _subject;

        public ChangeSubject(User author, Subject subject)
        {
            _author = author;
            _subject = subject;
        }

        public ChangeSubject SetName(string newName)
        {
            if (CheckAuthor())
            {
                _subject.Name = newName;
            }

            return this;
        }

        public ChangeSubject SetLectureMaterials(IEnumerable<LectureMaterials> lectureMaterials)
        {
            if (CheckAuthor())
            {
                _subject._lectureMaterials.Clear();
                _subject._lectureMaterials.AddRange(lectureMaterials);
            }

            return this;
        }

        public ChangeSubject SetSemesters(IEnumerable<Semester> semesters)
        {
            if (CheckAuthor())
            {
                _subject._semesters.Clear();
                _subject._semesters.AddRange(semesters);
            }

            return this;
        }

        private bool CheckAuthor()
        {
            return _author == _subject.Author;
        }
    }

    public ObjectStatus GetLastStatus()
    {
        return Status;
    }
}