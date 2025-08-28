using Lab5.Application.Models.Users;

namespace Lab5.Application.Abstractions.Repositories;

public interface IAdminRepository
{
    User? FindUserByUsername(string username);

    void ChangePassword(long userId, string newPassword);
}