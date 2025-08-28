using Lab5.Application.Contracts.Users;
using Lab5.Application.Contracts.Users.OperationsResults;
using Spectre.Console;
using System.Globalization;

namespace Lab5.Presentation.Console.Scenarios.GetBalance;

public class GetBalanceScenario : IScenario
{
    private readonly IUserService _userService;

    public GetBalanceScenario(IUserService userService)
    {
        _userService = userService;
    }

    public string Name => "Get Balance";

    public void Run()
    {
        long accountId = AnsiConsole.Ask<long>("Enter your account id: ");

        GetBalanceResult result = _userService.GetBalance(accountId);

        string message = result switch
        {
            GetBalanceResult.Success success => success.Balance.ToString(CultureInfo.CurrentCulture),
            GetBalanceResult.AccountNotFound => "Account not found",
            GetBalanceResult.NotLoginYet => "Please login to continue",
            _ => throw new ArgumentOutOfRangeException(nameof(result)),
        };

        AnsiConsole.WriteLine(message);
        AnsiConsole.Ask<string>("Ok");
    }
}