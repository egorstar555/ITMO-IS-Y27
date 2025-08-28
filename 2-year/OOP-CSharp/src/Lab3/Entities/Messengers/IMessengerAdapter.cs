namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Messengers;

public interface IMessengerAdapter
{
    void ReceiveMessage(Message message);
}