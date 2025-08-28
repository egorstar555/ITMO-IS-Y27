using Lab5.Application.Models.Users;

namespace Lab5.Application.Abstractions.Repositories;

public interface IUserRepository
{
    User? FindUserByUsername(string username);

    UserAccount? GetAccount(long accountId, long userId);

    void ChangeMoneyAmountOnAccount(long accountId, long userId, decimal amount);

    IEnumerable<UserOperation> GetUserOperationHistory(long userId);

    long CreateAccount(long userId);

    void LogOperation(long accountId, decimal amount, string operationName);
}