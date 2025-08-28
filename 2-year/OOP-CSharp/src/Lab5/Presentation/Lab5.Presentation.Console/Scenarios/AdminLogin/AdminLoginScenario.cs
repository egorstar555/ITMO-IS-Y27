using Lab5.Application.Contracts.Users;
using Lab5.Application.Contracts.Users.OperationsResults;
using Spectre.Console;

namespace Lab5.Presentation.Console.Scenarios.AdminLogin;

public class AdminLoginScenario : IScenario
{
    private readonly IAdminService _adminService;

    public AdminLoginScenario(IAdminService adminService)
    {
        _adminService = adminService;
    }

    public string Name => "Login as admin";

    public void Run()
    {
        string password = AnsiConsole.Prompt(
            new TextPrompt<string>("Enter system password: ").Secret());

        LoginResult result = _adminService.LoginAsAdmin(password);

        if (result is not LoginResult.Success)
        {
            Environment.Exit(0);
        }

        AnsiConsole.WriteLine("Successful login");
        AnsiConsole.Ask<string>("Ok");
    }
}