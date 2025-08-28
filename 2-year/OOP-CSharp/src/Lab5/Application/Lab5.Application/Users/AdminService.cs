using Lab5.Application.Abstractions.Repositories;
using Lab5.Application.Contracts.Users;
using Lab5.Application.Contracts.Users.OperationsResults;
using Lab5.Application.Models.Users;

namespace Lab5.Application.Users;

public class AdminService : IAdminService
{
    private readonly IAdminRepository _adminRepository;

    private readonly CurrentUserManager _currentAdminManager;

    private readonly string _adminName;

    public AdminService(IAdminRepository adminRepository, CurrentUserManager currentAdminManager)
    {
        _adminRepository = adminRepository;
        _currentAdminManager = currentAdminManager;
        _adminName = "ADMIN";
    }

    public LoginResult LoginAsAdmin(string password)
    {
        User? user = _adminRepository.FindUserByUsername(_adminName);
        if (user is null)
        {
            return new LoginResult.NotFound();
        }

        if (user.Password != password)
        {
            return new LoginResult.IncorrectPassword();
        }

        _currentAdminManager.User = user;
        return new LoginResult.Success();
    }

    public ChangePasswordResult ChangePassword(string newPassword)
    {
        if (_currentAdminManager.User is null)
        {
            return new ChangePasswordResult.NotLoginYet();
        }

        _adminRepository.ChangePassword(_currentAdminManager.User.Id, newPassword);
        return new ChangePasswordResult.Success();
    }

    public void Logout()
    {
        _currentAdminManager.User = null;
    }
}