using Lab5.Application.Abstractions.Repositories;
using Lab5.Application.Contracts.Users.OperationsResults;
using Lab5.Application.Models.Users;
using Lab5.Application.Users;
using NSubstitute;
using Xunit;

namespace Lab5.Tests;

public class AtmTests
{
    private readonly User _user;
    private readonly CurrentUserManager _manager;

    public AtmTests()
    {
        _user = new User(1, "bob", "strongPassword", UserRole.User);
        _manager = new CurrentUserManager();
        _manager.User = _user;
    }

    [Fact]
    public void WithdrawMoneyFromAccount_Success()
    {
        // Arrange
        IUserRepository userRepositoryMock = Substitute.For<IUserRepository>();
        var userService = new UserService(userRepositoryMock, _manager);

        UserAccount userAccount = Substitute.For<UserAccount>(1, 1, 2.1m);
        userRepositoryMock.GetAccount(Arg.Any<long>(), Arg.Any<long>()).Returns(userAccount);

        // Act
        WithdrawMoneyResult result = userService.WithdrawMoney(1, 1);

        // Assert
        Assert.IsType<WithdrawMoneyResult.Success>(result);
        userRepositoryMock.Received().ChangeMoneyAmountOnAccount(Arg.Any<long>(), Arg.Any<long>(), -1);
    }

    [Fact]
    public void WithdrawMoneyFromAccountNotEnoughMoneyOnAccount_Failure()
    {
        // Arrange
        IUserRepository userRepositoryMock = Substitute.For<IUserRepository>();
        var userService = new UserService(userRepositoryMock, _manager);

        UserAccount userAccount = Substitute.For<UserAccount>(1, 1, 2.1m);
        userRepositoryMock.GetAccount(Arg.Any<long>(), Arg.Any<long>()).Returns(userAccount);

        // Act
        WithdrawMoneyResult result = userService.WithdrawMoney(1, 10);

        // Assert
        Assert.IsType<WithdrawMoneyResult.NotEnoughMoney>(result);
    }

    [Fact]
    public void PutMoneyToAccount_Success()
    {
        // Arrange
        IUserRepository userRepositoryMock = Substitute.For<IUserRepository>();
        var userService = new UserService(userRepositoryMock, _manager);

        UserAccount userAccount = Substitute.For<UserAccount>(1, 1, 2.1m);
        userRepositoryMock.GetAccount(Arg.Any<long>(), Arg.Any<long>()).Returns(userAccount);

        // Act
        PutMoneyResult result = userService.PutMoney(1, 10);

        // Assert
        Assert.IsType<PutMoneyResult.Success>(result);
        userRepositoryMock.Received().ChangeMoneyAmountOnAccount(Arg.Any<long>(), Arg.Any<long>(), 10);
    }
}