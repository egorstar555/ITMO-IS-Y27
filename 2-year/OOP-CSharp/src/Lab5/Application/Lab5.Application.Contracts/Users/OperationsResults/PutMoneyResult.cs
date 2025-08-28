namespace Lab5.Application.Contracts.Users.OperationsResults;

public abstract record class PutMoneyResult
{
    private PutMoneyResult() { }

    public sealed record Success : PutMoneyResult;

    public sealed record NotLoginYet : PutMoneyResult;

    public sealed record AccountNotFound : PutMoneyResult;

    public sealed record NegativeAmountOfMoney : PutMoneyResult;
}