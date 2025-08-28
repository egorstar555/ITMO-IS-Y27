using Itmo.Dev.Platform.Postgres.Connection;
using Itmo.Dev.Platform.Postgres.Extensions;
using Lab5.Application.Abstractions.Repositories;
using Lab5.Application.Models.Users;
using Npgsql;

namespace Lab5.Infrastructure.DataAccess.Repositories;

public class UserRepository : IUserRepository
{
    private readonly IPostgresConnectionProvider _connectionProvider;

    public UserRepository(IPostgresConnectionProvider connectionProvider)
    {
        _connectionProvider = connectionProvider;
    }

    public User? FindUserByUsername(string username)
    {
        const string sql = """
                           select user_id, user_name, user_password, user_role
                           from users
                           where user_name = :username;
                           """;

        NpgsqlConnection connection = _connectionProvider
            .GetConnectionAsync(default)
            .AsTask()
            .GetAwaiter()
            .GetResult();

        using NpgsqlCommand command = new NpgsqlCommand(sql, connection)
            .AddParameter("username", username);

        using NpgsqlDataReader reader = command.ExecuteReader();

        if (reader.Read() is false)
            return null;

        var user = new User(
            Id: reader.GetInt64(0),
            Username: reader.GetString(1),
            Password: reader.GetString(2),
            Role: reader.GetFieldValue<UserRole>(3));

        connection.Close();

        return user;
    }

    public UserAccount? GetAccount(long accountId, long userId)
    {
        const string sql = """
                           select account_id, user_id, money_amount
                           from users_accounts
                           where account_id = :accountId and user_id = :userId;
                           """;

        NpgsqlConnection connection = _connectionProvider
            .GetConnectionAsync(default)
            .AsTask()
            .GetAwaiter()
            .GetResult();

        using NpgsqlCommand command = new NpgsqlCommand(sql, connection)
            .AddParameter("accountId", accountId)
            .AddParameter("userId", userId);

        using NpgsqlDataReader reader = command.ExecuteReader();

        if (reader.Read() is false)
            return null;

        var userAccount = new UserAccount(
            Id: reader.GetInt64(0),
            UserId: reader.GetInt64(1),
            MoneyAmount: reader.GetInt64(2));

        connection.Close();

        return userAccount;
    }

    public void ChangeMoneyAmountOnAccount(long accountId, long userId, decimal amount)
    {
        const string sql = """
                           UPDATE users_accounts 
                           SET money_amount = money_amount + :amount
                           where account_id = :accountId and user_id = :userId;
                           """;

        NpgsqlConnection connection = _connectionProvider
            .GetConnectionAsync(default)
            .AsTask()
            .GetAwaiter()
            .GetResult();

        using NpgsqlCommand command = new NpgsqlCommand(sql, connection)
            .AddParameter("accountId", accountId)
            .AddParameter("userId", userId)
            .AddParameter("amount", amount);

        command.ExecuteNonQuery();

        connection.Close();
    }

    public IEnumerable<UserOperation> GetUserOperationHistory(long userId)
    {
        const string sql = """
                           select o.operation_id, o.account_id, o.money_amount, o.operation_name
                           from operations as o join users_accounts as ua 
                               on ua.account_id = o.account_id
                           where ua.user_id = :userId;
                           """;

        NpgsqlConnection connection = _connectionProvider
            .GetConnectionAsync(default)
            .AsTask()
            .GetAwaiter()
            .GetResult();

        using NpgsqlCommand command = new NpgsqlCommand(sql, connection)
            .AddParameter("userId", userId);

        using NpgsqlDataReader reader = command.ExecuteReader();

        while (reader.Read())
        {
            yield return new UserOperation(
                reader.GetInt64(0),
                reader.GetInt64(1),
                reader.GetInt64(2),
                reader.GetString(3));
        }

        connection.Close();
    }

    public long CreateAccount(long userId)
    {
        const string sql = """
                           INSERT INTO users_accounts
                               (user_id, money_amount) 
                           values (:userId, 0)
                           RETURNING account_id;
                           """;

        NpgsqlConnection connection = _connectionProvider
            .GetConnectionAsync(default)
            .AsTask()
            .GetAwaiter()
            .GetResult();

        using NpgsqlCommand command = new NpgsqlCommand(sql, connection)
            .AddParameter("userId", userId);

        NpgsqlDataReader reader = command.ExecuteReader();
        reader.Read();

        long accountId = reader.GetInt64(0);
        connection.Close();

        return accountId;
    }

    public void LogOperation(long accountId, decimal amount, string operationName)
    {
        const string sql = """
                           INSERT INTO operations
                               (account_id, money_amount, operation_name) 
                           values (:accountId, :amount, :operationName);
                           """;

        NpgsqlConnection connection = _connectionProvider
            .GetConnectionAsync(default)
            .AsTask()
            .GetAwaiter()
            .GetResult();

        using NpgsqlCommand command = new NpgsqlCommand(sql, connection)
            .AddParameter("accountId", accountId)
            .AddParameter("amount", amount)
            .AddParameter("operationName", operationName);

        command.ExecuteNonQuery();
        connection.Close();
    }
}