using Lab5.Application.Contracts.Users;
using Lab5.Application.Contracts.Users.OperationsResults;
using Spectre.Console;

namespace Lab5.Presentation.Console.Scenarios.PutMoney;

public class PutMoneyScenario : IScenario
{
    private readonly IUserService _userService;

    public PutMoneyScenario(IUserService userService)
    {
        _userService = userService;
    }

    public string Name => "Put money";

    public void Run()
    {
        long accountId = AnsiConsole.Ask<long>("Enter your account id: ");
        decimal amount = AnsiConsole.Ask<decimal>("Enter money amount: ");

        PutMoneyResult result = _userService.PutMoney(accountId, amount);

        string message = result switch
        {
            PutMoneyResult.Success => "Success!",
            PutMoneyResult.AccountNotFound => "Account not found",
            PutMoneyResult.NotLoginYet => "Please login to continue",
            PutMoneyResult.NegativeAmountOfMoney => "Amount of money cannot be negative",
            _ => throw new ArgumentOutOfRangeException(nameof(result)),
        };

        AnsiConsole.WriteLine(message);
        AnsiConsole.Ask<string>("Ok");
    }
}