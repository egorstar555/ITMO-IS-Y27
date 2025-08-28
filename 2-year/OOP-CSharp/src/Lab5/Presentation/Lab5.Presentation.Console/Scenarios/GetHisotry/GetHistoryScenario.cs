using Lab5.Application.Contracts.Users;
using Lab5.Application.Models.Users;
using Spectre.Console;
using System.Globalization;

namespace Lab5.Presentation.Console.Scenarios.GetHisotry;

public class GetHistoryScenario : IScenario
{
    private readonly IUserService _userService;

    public GetHistoryScenario(IUserService userService)
    {
        _userService = userService;
    }

    public string Name => "Get history";

    public void Run()
    {
        IEnumerable<UserOperation> result = _userService.GetOperationsHistory();
        var table = new Table();
        table.AddColumn("OperationId");
        table.AddColumn("AccountId");
        table.AddColumn("MoneyAmount");
        table.AddColumn("OperationName");

        foreach (UserOperation operation in result)
        {
            table.AddRow(
                operation.OperationId.ToString(),
                operation.AccountId.ToString(),
                operation.MoneyAmount.ToString(CultureInfo.CurrentCulture),
                operation.OperationName);
        }

        AnsiConsole.Write(table);
        AnsiConsole.Ask<string>("Ok");
    }
}