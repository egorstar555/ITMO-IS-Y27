using Lab5.Application.Contracts.Users;
using Lab5.Application.Contracts.Users.OperationsResults;
using Spectre.Console;

namespace Lab5.Presentation.Console.Scenarios.ChangePassword;

public class ChangePasswordScenario : IScenario
{
    private readonly IAdminService _adminService;

    public ChangePasswordScenario(IAdminService adminService)
    {
        _adminService = adminService;
    }

    public string Name => "Change admin password";

    public void Run()
    {
        string password = AnsiConsole.Prompt(
            new TextPrompt<string>("Enter new password: ").Secret());

        ChangePasswordResult result = _adminService.ChangePassword(password);

        string message = result switch
        {
            ChangePasswordResult.Success => "Password changed successfully.",
            ChangePasswordResult.NotLoginYet => "You are not logged in.",
            _ => throw new ArgumentOutOfRangeException(nameof(result)),
        };

        AnsiConsole.WriteLine(message);
        AnsiConsole.Ask<string>("Ok");
    }
}