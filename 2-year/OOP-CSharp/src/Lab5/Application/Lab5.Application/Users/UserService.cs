using Lab5.Application.Abstractions.Repositories;
using Lab5.Application.Contracts.Users;
using Lab5.Application.Contracts.Users.OperationsResults;
using Lab5.Application.Models.Users;

namespace Lab5.Application.Users;

public class UserService : IUserService
{
    private readonly IUserRepository _userRepository;

    private readonly CurrentUserManager _currentUserManager;

    public UserService(IUserRepository userRepository, CurrentUserManager currentUserManager)
    {
        _userRepository = userRepository;
        _currentUserManager = currentUserManager;
    }

    public LoginResult Login(string username, string password)
    {
        User? user = _userRepository.FindUserByUsername(username);
        if (user is null)
        {
            return new LoginResult.NotFound();
        }

        if (user.Password != password)
        {
            return new LoginResult.IncorrectPassword();
        }

        _currentUserManager.User = user;
        return new LoginResult.Success();
    }

    public void Logout()
    {
        _currentUserManager.User = null;
    }

    public GetBalanceResult GetBalance(long accountId)
    {
        if (_currentUserManager.User is null)
        {
            return new GetBalanceResult.NotLoginYet();
        }

        UserAccount? userAccount = _userRepository.GetAccount(accountId, _currentUserManager.User.Id);

        if (userAccount is null)
        {
            return new GetBalanceResult.AccountNotFound();
        }

        return new GetBalanceResult.Success(userAccount.MoneyAmount);
    }

    public WithdrawMoneyResult WithdrawMoney(long accountId, decimal amount)
    {
        if (_currentUserManager.User is null)
        {
            return new WithdrawMoneyResult.NotLoginYet();
        }

        if (amount < 0)
        {
            return new WithdrawMoneyResult.NegativeAmountOfMoney();
        }

        UserAccount? userAccount = _userRepository.GetAccount(accountId, _currentUserManager.User.Id);

        if (userAccount is null)
        {
            return new WithdrawMoneyResult.AccountNotFound();
        }

        if (userAccount.MoneyAmount < amount)
        {
            return new WithdrawMoneyResult.NotEnoughMoney();
        }

        _userRepository.ChangeMoneyAmountOnAccount(accountId, _currentUserManager.User.Id, -amount);
        _userRepository.LogOperation(accountId, amount, "WithdrawMoney");

        return new WithdrawMoneyResult.Success();
    }

    public PutMoneyResult PutMoney(long accountId, decimal amount)
    {
        if (_currentUserManager.User is null)
        {
            return new PutMoneyResult.NotLoginYet();
        }

        if (amount < 0)
        {
            return new PutMoneyResult.NegativeAmountOfMoney();
        }

        UserAccount? userAccount = _userRepository.GetAccount(accountId, _currentUserManager.User.Id);

        if (userAccount is null)
        {
            return new PutMoneyResult.AccountNotFound();
        }

        _userRepository.ChangeMoneyAmountOnAccount(accountId, _currentUserManager.User.Id, amount);
        _userRepository.LogOperation(accountId, amount, "PutMoney");

        return new PutMoneyResult.Success();
    }

    public IEnumerable<UserOperation> GetOperationsHistory()
    {
        if (_currentUserManager.User is null)
        {
            return Enumerable.Empty<UserOperation>();
        }

        return _userRepository.GetUserOperationHistory(_currentUserManager.User.Id);
    }

    public CreateAccountResult CreateAccount()
    {
        if (_currentUserManager.User is null)
        {
            return new CreateAccountResult.NotLoginYet();
        }

        long accountId = _userRepository.CreateAccount(_currentUserManager.User.Id);
        return new CreateAccountResult.Success(accountId);
    }
}