using Itmo.ObjectOrientedProgramming.Lab2.Entities.Interfaces;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.LaboratoryWorks;

public class LaboratoryWork : ILaboratoryWork, IPrototype<LaboratoryWork>
{
    public Identifier Id { get; }

    public string Name { get; private set; }

    public string Description { get; private set; }

    public int Points { get; }

    public string EvaluationCriteria { get; private set; }

    public User Author { get; }

    public Identifier? ParentId { get; }

    public ObjectStatus Status { get; private set; }

    private LaboratoryWork(
        Identifier id,
        string name,
        string description,
        int points,
        string evaluationCriteria,
        User author,
        Identifier? parentId)
    {
        Id = id;
        Name = name;
        Description = description;
        Points = points;
        EvaluationCriteria = evaluationCriteria;
        Author = author;
        ParentId = parentId;
        Status = new ObjectStatus.Successful();
    }

    public static Builder CreateBuilder => new Builder();

    public LaboratoryWork Clone()
    {
        return new LaboratoryWork(new Identifier(), Name, Description, Points, EvaluationCriteria, Author.Clone(), Id);
    }

    public class Builder
    {
        private Identifier? _id;
        private string? _name;
        private string? _description;
        private int? _points;
        private string? _evaluationCriteria;
        private User? _author;

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

        public Builder WithDescription(string description)
        {
            _description = description;
            return this;
        }

        public Builder WithPoints(int points)
        {
            _points = points;
            return this;
        }

        public Builder WithEvaluationCriteria(string evaluationCriteria)
        {
            _evaluationCriteria = evaluationCriteria;
            return this;
        }

        public Builder WithAuthor(User author)
        {
            _author = author;
            return this;
        }

        public LaboratoryWork Build()
        {
            return new LaboratoryWork(
                _id ?? throw new ArgumentNullException(),
                _name ?? throw new ArgumentNullException(),
                _description ?? throw new ArgumentNullException(),
                _points ?? throw new ArgumentNullException(),
                _evaluationCriteria ?? throw new ArgumentNullException(),
                _author ?? throw new ArgumentNullException(),
                null);
        }
    }

    public ChangeLaboratoryWork GetChanger(User author)
    {
        Status = new ObjectStatus.Successful();
        if (author != Author)
        {
            Status = new ObjectStatus.ChangeEntitiesNotByAuthor();
        }

        return new ChangeLaboratoryWork(author, this);
    }

    public class ChangeLaboratoryWork
    {
        private readonly User _author;
        private readonly LaboratoryWork _laboratoryWork;

        public ChangeLaboratoryWork(User author, LaboratoryWork laboratoryWork)
        {
            _author = author;
            _laboratoryWork = laboratoryWork;
        }

        public ChangeLaboratoryWork SetName(string newName)
        {
            if (CheckAuthor())
            {
                _laboratoryWork.Name = newName;
            }

            return this;
        }

        public ChangeLaboratoryWork SetDescription(string newDescription)
        {
            if (CheckAuthor())
            {
                _laboratoryWork.Description = newDescription;
            }

            return this;
        }

        public ChangeLaboratoryWork SetEvaluationCriteria(string newEvaluationCriteria)
        {
            if (CheckAuthor())
            {
                _laboratoryWork.EvaluationCriteria = newEvaluationCriteria;
            }

            return this;
        }

        private bool CheckAuthor()
        {
            return _author == _laboratoryWork.Author;
        }
    }

    public int GetMaxPoints()
    {
        return Points;
    }

    public ObjectStatus GetLastStatus()
    {
        return Status;
    }
}