namespace Lab5.Application.Contracts.Users.OperationsResults;

public abstract record class WithdrawMoneyResult
{
    private WithdrawMoneyResult() { }

    public sealed record Success : WithdrawMoneyResult;

    public sealed record NotLoginYet : WithdrawMoneyResult;

    public sealed record AccountNotFound : WithdrawMoneyResult;

    public sealed record NotEnoughMoney : WithdrawMoneyResult;

    public sealed record NegativeAmountOfMoney : WithdrawMoneyResult;
}