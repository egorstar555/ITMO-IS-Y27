namespace Lab5.Application.Models.Users;

public record class UserOperation(long OperationId, long AccountId, decimal MoneyAmount, string OperationName);