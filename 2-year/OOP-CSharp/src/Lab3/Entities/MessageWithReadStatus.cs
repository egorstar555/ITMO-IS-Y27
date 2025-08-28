namespace Itmo.ObjectOrientedProgramming.Lab3.Entities;

public class MessageWithReadStatus
{
    public Message Message { get; }

    public bool ReadStatus { get; private set; }

    public MessageWithReadStatus(Message message)
    {
        Message = message;
        ReadStatus = false;
    }

    public void Read()
    {
        ReadStatus = true;
    }
}