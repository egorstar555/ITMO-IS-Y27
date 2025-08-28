namespace Lab5.Application.Models.Users;

public record class User(long Id, string Username, string Password, UserRole Role);
