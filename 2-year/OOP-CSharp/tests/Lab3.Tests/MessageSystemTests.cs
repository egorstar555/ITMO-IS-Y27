using Itmo.ObjectOrientedProgramming.Lab3.Entities;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Destination;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Devices;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Loggers;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messengers;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Users;
using Itmo.ObjectOrientedProgramming.Lab3.Models;
using Itmo.ObjectOrientedProgramming.Lab3.Services;
using Moq;
using Xunit;

namespace Lab3.Tests;

public class MessageSystemTests
{
    private readonly Message _message;
    private readonly User _user;

    public MessageSystemTests()
    {
        _message = new Message("hello", "bob", 100);
        _user = new User("bob");
    }

    [Fact]
    public void WhenUserReceiveMessageItSaveWithStatusNotReadYet_Success()
    {
        // Arrange

        // Act
        _user.ReceiveMessage(_message);

        // Assert
        Assert.IsType<MessageStatus.NotReadYet>(_user.GetMessageStatus("hello"));
    }

    [Fact]
    public void WhenUserReadMessageItChangeStatusToAlreadyRead_Success()
    {
        // Arrange
        _user.ReceiveMessage(_message);

        // Act
        _user.TryReadMessage("hello");

        // Assert
        Assert.IsType<MessageStatus.AlreadyRead>(_user.GetMessageStatus("hello"));
    }

    [Fact]
    public void WhenUserTryReadAlreadyReadMessageAlreadyReadStatusReturned_Success()
    {
        // Arrange
        _user.ReceiveMessage(_message);
        _user.TryReadMessage("hello");

        // Act
        _user.TryReadMessage("hello");

        // Assert
        Assert.IsType<MessageStatus.AlreadyRead>(_user.GetMessageStatus("hello"));
    }

    [Fact]
    public void AnUnimportantMessageDoesNotReachTheDestination_Success()
    {
        // Arrange
        int receivedMessageCount = 0;
        var mock = new Mock<IDestination>();
        mock.Setup(destination => destination.ReceiveMessage(It.IsAny<Message>()))
            .Callback(() => receivedMessageCount++);

        var destinationFilter = new DestinationFilter(mock.Object, 200);

        // Act
        destinationFilter.ReceiveMessage(_message);

        // Assert
        Assert.Equal(0, receivedMessageCount);
    }

    [Fact]
    public void DestinationLoggerShouldDoLogWhenReceiveMessage_Success()
    {
        // Arrange
        int logCount = 0;
        var destinationUser = new DestinationUser(_user);
        var mock = new Mock<ILogger>();
        mock.Setup(destination => destination.Log(It.IsAny<string>())).Callback(() => logCount++);

        var destinationLogger = new DestinationLogger(destinationUser, mock.Object);

        // Act
        destinationLogger.ReceiveMessage(_message);

        // Assert
        Assert.Equal(1, logCount);
    }

    [Fact]
    public void MessengerShouldWriteMessage_Success()
    {
        // Arrange
        int writeCount = 0;

        var mock = new Mock<IMessenger>();
        mock.Setup(messenger => messenger.WriteMessage(It.IsAny<string>())).Callback(() => writeCount++);

        var myMessengerAdapter = new MyMessengerAdapter(mock.Object);

        // Act
        myMessengerAdapter.ReceiveMessage(_message);

        // Assert
        Assert.Equal(1, writeCount);
    }

    [Fact]
    public void OneOfTwoMessagesReceived_Success()
    {
        // Arrange
        int receivedMessageCount = 0;
        var mock = new Mock<IUser>();
        var destinationUser = new DestinationUser(mock.Object);
        var destinationUser2 = new DestinationUser(mock.Object);

        mock.Setup(user => user.ReceiveMessage(It.IsAny<Message>()))
            .Callback(() => receivedMessageCount++);

        var destinationFilter = new DestinationFilter(destinationUser, 200);

        // Act
        destinationFilter.ReceiveMessage(_message);
        destinationUser2.ReceiveMessage(_message);

        // Assert
        Assert.Equal(1, receivedMessageCount);
    }

    [Fact]
    public void DisplayDriverShouldWriteText_Success()
    {
        // Arrange
        int textPrintCount = 0;
        var mock = new Mock<IDisplayDriver>();
        var displayAdapter = new DisplayDriverAdapter(mock.Object);
        var destinationDisplay = new DestinationDisplay(displayAdapter);

        mock.Setup(display => display.WriteText(It.IsAny<string>()))
            .Callback(() => textPrintCount++);

        // Act
        destinationDisplay.ReceiveMessage(_message);

        // Assert
        Assert.Equal(1, textPrintCount);
    }

    [Fact]
    public void FileWriterShouldWriteText_Success()
    {
        // Arrange
        int textPrintCount = 0;
        var mock = new Mock<IFileWriter>();
        var fileSystemAdapter = new FileSystemAdapter(mock.Object);
        var destinationDisplay = new DestinationDisplay(fileSystemAdapter);

        mock.Setup(fileWriter => fileWriter.WriteToFile(It.IsAny<string>()))
            .Callback(() => textPrintCount++);

        // Act
        destinationDisplay.ReceiveMessage(_message);

        // Assert
        Assert.Equal(1, textPrintCount);
    }

    [Fact]
    public void FactoryCreateUserWithLoggerAndFilterUserNotReceiveUnimportantMessage_Success()
    {
        // Arrange
        int receivedMessageCount = 0;
        var mock = new Mock<IUser>();
        var destinationLogger = new Logger();

        mock.Setup(destination => destination.ReceiveMessage(It.IsAny<Message>()))
            .Callback(() => receivedMessageCount++);

        var destinationFactory = new DestinationFactory(destinationLogger, 200);
        IDestination destinationUser = destinationFactory.CreateUser(mock.Object);

        // Act
        destinationUser.ReceiveMessage(_message);

        // Assert
        Assert.Equal(0, receivedMessageCount);
    }

    [Fact]
    public void FactoryCreateUserWithLoggerAndFilterImportantMessageReceivedAndLogged_Success()
    {
        // Arrange
        int receivedMessageCount = 0;
        var mockUser = new Mock<IUser>();
        var mockLogger = new Mock<ILogger>();

        mockUser.Setup(destination => destination.ReceiveMessage(It.IsAny<Message>()))
            .Callback(() => receivedMessageCount++);
        mockLogger.Setup(logger => logger.Log(It.IsAny<string>()))
            .Callback(() => receivedMessageCount++);

        var destinationFactory = new DestinationFactory(mockLogger.Object, 99);
        IDestination destinationUser = destinationFactory.CreateUser(mockUser.Object);

        // Act
        destinationUser.ReceiveMessage(_message);

        // Assert
        Assert.Equal(2, receivedMessageCount);
    }
}