namespace Lab5.Application.Contracts.Users.OperationsResults;

public abstract record class GetBalanceResult
{
    private GetBalanceResult() { }

    public sealed record Success(decimal Balance) : GetBalanceResult;

    public sealed record AccountNotFound : GetBalanceResult;

    public sealed record NotLoginYet : GetBalanceResult;
}