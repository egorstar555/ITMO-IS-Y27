using Itmo.ObjectOrientedProgramming.Lab2.Entities.Interfaces;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.LectureMaterial;

public class LectureMaterials : ILectureMaterials, IPrototype<LectureMaterials>
{
    public Identifier Id { get; }

    public string Name { get; private set; }

    public string Description { get; private set; }

    public string Content { get; private set; }

    public User Author { get; }

    public Identifier? ParentId { get; }

    public ObjectStatus Status { get; private set; }

    private LectureMaterials(
        Identifier id,
        string name,
        string description,
        string content,
        User author,
        Identifier? parentId)
    {
        Id = id;
        Name = name;
        Description = description;
        Content = content;
        Author = author;
        ParentId = parentId;
        Status = new ObjectStatus.Successful();
    }

    public static Builder CreateBuilder => new Builder();

    public LectureMaterials Clone()
    {
        return new LectureMaterials(
            new Identifier(),
            Name,
            Description,
            Content,
            Author,
            Id);
    }

    public class Builder
    {
        private Identifier? _id;
        private string? _name;
        private string? _description;
        private string? _content;
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

        public Builder WithContent(string content)
        {
            _content = content;
            return this;
        }

        public Builder WithAuthor(User author)
        {
            _author = author;
            return this;
        }

        public LectureMaterials Build()
        {
            return new LectureMaterials(
                _id ?? throw new ArgumentNullException(),
                _name ?? throw new ArgumentNullException(),
                _description ?? throw new ArgumentNullException(),
                _content ?? throw new ArgumentNullException(),
                _author ?? throw new ArgumentNullException(),
                null);
        }
    }

    public ChangeLectureMaterials GetChanger(User author)
    {
        Status = new ObjectStatus.Successful();
        if (author != Author)
        {
            Status = new ObjectStatus.ChangeEntitiesNotByAuthor();
        }

        return new ChangeLectureMaterials(author, this);
    }

    public class ChangeLectureMaterials
    {
        private readonly User _author;
        private readonly LectureMaterials _lectureMaterials;

        public ChangeLectureMaterials(User author, LectureMaterials lectureMaterials)
        {
            _author = author;
            _lectureMaterials = lectureMaterials;
        }

        public ChangeLectureMaterials SetName(string newName)
        {
            if (CheckAuthor())
            {
                _lectureMaterials.Name = newName;
            }

            return this;
        }

        public ChangeLectureMaterials SetDescription(string newDescription)
        {
            if (CheckAuthor())
            {
                _lectureMaterials.Description = newDescription;
            }

            return this;
        }

        public ChangeLectureMaterials SetContent(string newContent)
        {
            if (CheckAuthor())
            {
                _lectureMaterials.Content = newContent;
            }

            return this;
        }

        private bool CheckAuthor()
        {
            return _author == _lectureMaterials.Author;
        }
    }
}