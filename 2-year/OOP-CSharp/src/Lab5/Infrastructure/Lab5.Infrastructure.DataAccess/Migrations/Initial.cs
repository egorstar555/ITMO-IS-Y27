using FluentMigrator;
using Itmo.Dev.Platform.Postgres.Migrations;

namespace Lab5.Infrastructure.DataAccess.Migrations;

[Migration(1, "Initial")]
public class Initial : SqlMigration
{
    protected override string GetUpSql(IServiceProvider serviceProvider) =>
        """
        create type user_role as enum
        (
            'admin',
            'user'
        );

        create table users
        (
            user_id bigint primary key generated always as identity,
            user_name text not null,
            user_password text not null,
            user_role user_role not null 
        );

        create table users_accounts
        (
            account_id bigint primary key generated always as identity,
            user_id bigint not null references users(user_id),
            money_amount numeric not null 
        );

        create table operations
        (
            operation_id bigint primary key generated always as identity,
            account_id bigint not null references users_accounts(account_id),
            money_amount numeric not null,
            operation_name text not null
        );
        """;

    protected override string GetDownSql(IServiceProvider serviceProvider) =>
        """
        drop table users;
        drop table users_accounts;
        drop table operations;

        drop type user_role;
        """;
}