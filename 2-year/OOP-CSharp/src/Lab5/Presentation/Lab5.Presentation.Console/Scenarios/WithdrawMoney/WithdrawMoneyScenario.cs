using Lab5.Application.Contracts.Users;
using Lab5.Application.Contracts.Users.OperationsResults;
using Spectre.Console;

namespace Lab5.Presentation.Console.Scenarios.WithdrawMoney;

public class WithdrawMoneyScenario : IScenario
{
    private readonly IUserService _userService;

    public WithdrawMoneyScenario(IUserService userService)
    {
        _userService = userService;
    }

    public string Name => "Withdraw money";

    public void Run()
    {
        long accountId = AnsiConsole.Ask<long>("Enter your account id: ");
        decimal amount = AnsiConsole.Ask<decimal>("Enter money amount: ");

        WithdrawMoneyResult result = _userService.WithdrawMoney(accountId, amount);

        string message = result switch
        {
            WithdrawMoneyResult.Success => "Success!",
            WithdrawMoneyResult.NotEnoughMoney => "Not enough money!",
            WithdrawMoneyResult.AccountNotFound => "Account not found",
            WithdrawMoneyResult.NotLoginYet => "Please login to continue",
            WithdrawMoneyResult.NegativeAmountOfMoney => "Amount of money cannot be negative",
            _ => throw new ArgumentOutOfRangeException(nameof(result)),
        };

        AnsiConsole.WriteLine(message);
        AnsiConsole.Ask<string>("Ok");
    }
}