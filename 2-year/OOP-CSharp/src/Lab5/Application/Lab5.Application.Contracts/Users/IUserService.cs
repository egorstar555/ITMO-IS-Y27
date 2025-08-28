using Lab5.Application.Contracts.Users.OperationsResults;
using Lab5.Application.Models.Users;

namespace Lab5.Application.Contracts.Users;

public interface IUserService
{
    LoginResult Login(string username, string password);

    void Logout();

    GetBalanceResult GetBalance(long accountId);

    WithdrawMoneyResult WithdrawMoney(long accountId, decimal amount);

    PutMoneyResult PutMoney(long accountId, decimal amount);

    IEnumerable<UserOperation> GetOperationsHistory();

    CreateAccountResult CreateAccount();
}