using Itmo.ObjectOrientedProgramming.Lab2.Entities;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Repository;

public class UserRepository : RepositoryBase<User>
{
    public UserRepository() : base() { }

    public UserRepository(IEnumerable<User> users) : base() { }
}