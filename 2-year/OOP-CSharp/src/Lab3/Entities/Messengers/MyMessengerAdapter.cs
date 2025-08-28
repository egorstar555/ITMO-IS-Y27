namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Messengers;

public class MyMessengerAdapter : IMessengerAdapter
{
    private readonly IMessenger _messenger;

    public MyMessengerAdapter(IMessenger messenger)
    {
        _messenger = messenger;
    }

    public void ReceiveMessage(Message message)
    {
        _messenger.WriteMessage($"Title: {message.Title}\n" +
                                $"Body: {message.Body}\n" +
                                $"Importance: {message.Importance}");
    }
}