using Lab5.Application.Contracts.Users;
using Spectre.Console;

namespace Lab5.Presentation.Console.Scenarios.LogOut;

public class LogOutScenario : IScenario
{
    private readonly IUserService _userService;

    public LogOutScenario(IUserService userService)
    {
        _userService = userService;
    }

    public string Name => "Log out";

    public void Run()
    {
        _userService.Logout();

        string message = "Log out successful!";

        AnsiConsole.WriteLine(message);
        AnsiConsole.Ask<string>("Ok");
    }
}