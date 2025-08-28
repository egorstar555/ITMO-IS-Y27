using Lab5.Presentation.Console.Scenarios;
using Lab5.Presentation.Console.Scenarios.AdminLogin;
using Lab5.Presentation.Console.Scenarios.AdminLogOut;
using Lab5.Presentation.Console.Scenarios.ChangePassword;
using Lab5.Presentation.Console.Scenarios.CreateAccount;
using Lab5.Presentation.Console.Scenarios.ExitFromAtm;
using Lab5.Presentation.Console.Scenarios.GetBalance;
using Lab5.Presentation.Console.Scenarios.GetHisotry;
using Lab5.Presentation.Console.Scenarios.Login;
using Lab5.Presentation.Console.Scenarios.LogOut;
using Lab5.Presentation.Console.Scenarios.PutMoney;
using Lab5.Presentation.Console.Scenarios.WithdrawMoney;
using Microsoft.Extensions.DependencyInjection;

namespace Lab5.Presentation.Console.Extensions;

public static class ServiceCollectionExtensions
{
    public static IServiceCollection AddPresentationConsole(this IServiceCollection collection)
    {
        collection.AddScoped<ScenarioRunner>();
        collection.AddScoped<IScenarioProvider, LoginScenarioProvider>();
        collection.AddScoped<IScenarioProvider, GetBalanceScenarioProvider>();
        collection.AddScoped<IScenarioProvider, WithdrawMoneyScenarioProvider>();
        collection.AddScoped<IScenarioProvider, PutMoneyScenarioProvider>();
        collection.AddScoped<IScenarioProvider, CreateAccountScenarioProvider>();
        collection.AddScoped<IScenarioProvider, GetHistoryScenarioProvider>();
        collection.AddScoped<IScenarioProvider, LogOutScenarioProvider>();

        collection.AddScoped<IScenarioProvider, AdminLoginScenarioProvider>();
        collection.AddScoped<IScenarioProvider, ChangePasswordScenarioProvider>();
        collection.AddScoped<IScenarioProvider, AdminLogOutScenarioProvider>();

        collection.AddScoped<IScenarioProvider, ExitScenarioProvider>();

        return collection;
    }
}