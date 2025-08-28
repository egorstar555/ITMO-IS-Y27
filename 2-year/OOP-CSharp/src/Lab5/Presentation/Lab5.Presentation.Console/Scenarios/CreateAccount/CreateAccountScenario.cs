using Lab5.Application.Contracts.Users;
using Lab5.Application.Contracts.Users.OperationsResults;
using Spectre.Console;

namespace Lab5.Presentation.Console.Scenarios.CreateAccount;

public class CreateAccountScenario : IScenario
{
    private readonly IUserService _userService;

    public CreateAccountScenario(IUserService userService)
    {
        _userService = userService;
    }

    public string Name => "Create account";

    public void Run()
    {
        CreateAccountResult result = _userService.CreateAccount();

        string message = result switch
        {
            CreateAccountResult.Success success => $"Your account Id is: {success.AccountId.ToString()}",
            CreateAccountResult.NotLoginYet => "Please login to continue",
            _ => throw new ArgumentOutOfRangeException(nameof(result)),
        };

        AnsiConsole.WriteLine(message);
        AnsiConsole.Ask<string>("Ok");
    }
}