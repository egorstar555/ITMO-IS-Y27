namespace Lab5.Application.Contracts.Users.OperationsResults;

public abstract record class LoginResult
{
    private LoginResult() { }

    public sealed record Success : LoginResult;

    public sealed record NotFound : LoginResult;

    public sealed record IncorrectPassword : LoginResult;
}