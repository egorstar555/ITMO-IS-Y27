namespace Lab5.Application.Contracts.Users.OperationsResults;

public abstract record CreateAccountResult
{
    private CreateAccountResult() { }

    public sealed record Success(long AccountId) : CreateAccountResult;

    public sealed record NotLoginYet : CreateAccountResult;
}