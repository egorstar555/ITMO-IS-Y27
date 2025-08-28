using Lab5.Application.Contracts.Users.OperationsResults;

namespace Lab5.Application.Contracts.Users;

public interface IAdminService
{
    LoginResult LoginAsAdmin(string password);

    ChangePasswordResult ChangePassword(string newPassword);

    void Logout();
}