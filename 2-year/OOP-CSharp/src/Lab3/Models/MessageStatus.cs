namespace Itmo.ObjectOrientedProgramming.Lab3.Models;

public abstract record MessageStatus
{
    private MessageStatus() { }

    public sealed record SuccessfullyRead : MessageStatus;

    public sealed record NotReadYet : MessageStatus;

    public sealed record AlreadyRead : MessageStatus;

    public sealed record NotFound : MessageStatus;
}