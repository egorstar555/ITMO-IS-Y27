using Itmo.ObjectOrientedProgramming.Lab3.Models;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Users;

public interface IUser
{
    public void ReceiveMessage(Message message);

    public MessageStatus TryReadMessage(string title);
}