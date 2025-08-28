namespace Lab5.Application.Contracts.Users.OperationsResults;

public abstract record ChangePasswordResult
{
    private ChangePasswordResult() { }

    public sealed record Success : ChangePasswordResult;

    public sealed record NotLoginYet : ChangePasswordResult;
}