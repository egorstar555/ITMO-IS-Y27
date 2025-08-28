using Itmo.ObjectOrientedProgramming.Lab3.Models;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Users;

public class User : IUser
{
    public string Name { get; }

    private readonly List<MessageWithReadStatus> _messages;

    public User(string name)
    {
        Name = name;
        _messages = [];
    }

    public void ReceiveMessage(Message message)
    {
        _messages.Add(new MessageWithReadStatus(message));
    }

    public MessageStatus TryReadMessage(string title)
    {
        MessageWithReadStatus? findMessage = _messages.Find(message => message.Message.Title == title);

        if (findMessage is null)
        {
            return new MessageStatus.NotFound();
        }

        if (findMessage.ReadStatus)
        {
            return new MessageStatus.AlreadyRead();
        }

        findMessage.Read();
        return new MessageStatus.SuccessfullyRead();
    }

    public MessageStatus GetMessageStatus(string title)
    {
        MessageWithReadStatus? findMessage = _messages.Find(message => message.Message.Title == title);

        if (findMessage is null)
        {
            return new MessageStatus.NotFound();
        }

        if (findMessage.ReadStatus)
        {
            return new MessageStatus.AlreadyRead();
        }

        return new MessageStatus.NotReadYet();
    }
}