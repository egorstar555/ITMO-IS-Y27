namespace Itmo.ObjectOrientedProgramming.Lab3.Entities;

public class Message
{
    public string Title { get; }

    public string Body { get; }

    public int Importance { get; }

    public Message(string title, string body, int importance)
    {
        Title = title;
        Body = body;
        Importance = importance;
    }
}