using Lab5.Application.Contracts.Users;
using Spectre.Console;

namespace Lab5.Presentation.Console.Scenarios.AdminLogOut;

public class AdminLogOutScenario : IScenario
{
    private readonly IAdminService _adminService;

    public AdminLogOutScenario(IAdminService adminService)
    {
        _adminService = adminService;
    }

    public string Name => "Log out";

    public void Run()
    {
        _adminService.Logout();

        string message = "Log out successful!";

        AnsiConsole.WriteLine(message);
        AnsiConsole.Ask<string>("Ok");
    }
}