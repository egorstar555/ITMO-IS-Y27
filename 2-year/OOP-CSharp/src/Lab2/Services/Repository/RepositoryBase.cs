using Itmo.ObjectOrientedProgramming.Lab2.Entities;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Interfaces;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Repository;

public abstract class RepositoryBase<T> where T : IUniqueIdentifier
{
    private readonly List<T> _items;

    protected RepositoryBase()
    {
        _items = [];
    }

    protected RepositoryBase(IEnumerable<T> items)
    {
        _items = items.ToList();
    }

    public T? FindById(Identifier identifier)
    {
        return _items.Find(x => x.Id == identifier);
    }

    public void Insert(T entity)
    {
        _items.Add(entity);
    }

    public void Delete(Identifier identifier)
    {
        _items.RemoveAll(x => x.Id == identifier);
    }

    public void Update(T entity)
    {
        T? item = FindById(entity.Id);
        if (item != null)
        {
            item = entity;
        }
    }
}