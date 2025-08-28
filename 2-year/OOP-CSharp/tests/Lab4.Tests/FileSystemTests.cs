using Itmo.ObjectOrientedProgramming.Lab4;
using Itmo.ObjectOrientedProgramming.Lab4.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Handlers;
using Itmo.ObjectOrientedProgramming.Lab4.Handlers.OptionalParametersHandlers;
using Xunit;

namespace Lab4.Tests;

public class FileSystemTests
{
    private readonly ICommandHandler _handler;

    private readonly Connection _connection;

    public FileSystemTests()
    {
        _connection = new Connection();
        _handler = new ConnectCommandHandler()
            .AddNext(new DisconnectCommandHandler()
                .AddNext(new FileCopyCommandHandler()
                    .AddNext(new FileDeleteCommandHandler()
                        .AddNext(new FileMoveCommandHandler()
                            .AddNext(new FileRenameCommandHandler()
                                .AddNext(new FileShowCommandHandler()
                                    .AddOptional(new FileShowOptionalModeHandler())
                                    .AddNext(new TreeGotoCommandHandler()
                                        .AddNext(new TreeListCommandHandler()
                                            .AddOptional(new TreeListOptionalDepthHandler())))))))));
    }

    [Fact]
    public void ConnectCommandCreate_Success()
    {
        // Arrange
        IEnumerable<string> s = ["connect", "D:\\directory", "-m", "local"];

        using IEnumerator<string> request = s.GetEnumerator();

        request.MoveNext();

        // Act
        ICommand? res = _handler.Handle(request, _connection);

        // Assert
        Assert.IsType<ConnectCommand>(res);
    }

    [Fact]
    public void DisconnectCommandCreate_Success()
    {
        // Arrange
        IEnumerable<string> s = ["disconnect"];

        using IEnumerator<string> request = s.GetEnumerator();

        request.MoveNext();

        // Act
        ICommand? res = _handler.Handle(request, _connection);

        // Assert
        Assert.IsType<DisconnectCommand>(res);
    }

    [Fact]
    public void TreeGotoCommandCreate_Success()
    {
        // Arrange
        var actualCommand = new TreeGotoCommand("D:\\directory", _connection);

        IEnumerable<string> s = ["tree", "goto", "D:\\directory"];

        using IEnumerator<string> request = s.GetEnumerator();
        request.MoveNext();

        // Act
        ICommand? res = _handler.Handle(request, _connection);

        // Assert
        Assert.IsType<TreeGotoCommand>(res);
        Assert.True(res.Equals(actualCommand));
    }

    [Fact]
    public void TreeListCommandCreate_Success()
    {
        // Arrange
        var actualCommand = new TreeListCommand(_connection, 100);

        IEnumerable<string> s = ["tree", "list", "-d", "100"];

        using IEnumerator<string> request = s.GetEnumerator();
        request.MoveNext();

        // Act
        ICommand? res = _handler.Handle(request, _connection);

        // Assert
        Assert.IsType<TreeListCommand>(res);
        Assert.True(res.Equals(actualCommand));
    }

    [Fact]
    public void FileShowCommandCreate_Success()
    {
        // Arrange
        var actualCommand = new FileShowCommand("D:\\abc", _connection);

        IEnumerable<string> s = ["file", "show", "D:\\abc", "-m", "console"];

        using IEnumerator<string> request = s.GetEnumerator();
        request.MoveNext();

        // Act
        ICommand? res = _handler.Handle(request, _connection);

        // Assert
        Assert.IsType<FileShowCommand>(res);
        Assert.True(res.Equals(actualCommand));
    }

    [Fact]
    public void FileMoveCommandCreate_Success()
    {
        // Arrange
        var actualCommand = new FileMoveCommand("D:\\abc", "D:\\bcd", _connection);

        IEnumerable<string> s = ["file", "move", "D:\\abc", "D:\\bcd"];

        using IEnumerator<string> request = s.GetEnumerator();
        request.MoveNext();

        // Act
        ICommand? res = _handler.Handle(request, _connection);

        // Assert
        Assert.IsType<FileMoveCommand>(res);
        Assert.True(res.Equals(actualCommand));
    }

    [Fact]
    public void FileCopyCommandCreate_Success()
    {
        // Arrange
        var actualCommand = new FileCopyCommand("D:\\abc", "D:\\bcd", _connection);

        IEnumerable<string> s = ["file", "copy", "D:\\abc", "D:\\bcd"];

        using IEnumerator<string> request = s.GetEnumerator();
        request.MoveNext();

        // Act
        ICommand? res = _handler.Handle(request, _connection);

        // Assert
        Assert.IsType<FileCopyCommand>(res);
        Assert.True(res.Equals(actualCommand));
    }

    [Fact]
    public void FileDeleteCommandCreate_Success()
    {
        // Arrange
        var actualCommand = new FileDeleteCommand("D:\\abc", _connection);

        IEnumerable<string> s = ["file", "delete", "D:\\abc"];

        using IEnumerator<string> request = s.GetEnumerator();
        request.MoveNext();

        // Act
        ICommand? res = _handler.Handle(request, _connection);

        // Assert
        Assert.IsType<FileDeleteCommand>(res);
        Assert.True(res.Equals(actualCommand));
    }

    [Fact]
    public void FileRenameCommandCreate_Success()
    {
        // Arrange
        var actualCommand = new FileRenameCommand("D:\\abc\\tom.txt", "bob.txt", _connection);

        IEnumerable<string> s = ["file", "rename", "D:\\abc\\tom.txt", "bob.txt"];

        using IEnumerator<string> request = s.GetEnumerator();
        request.MoveNext();

        // Act
        ICommand? res = _handler.Handle(request, _connection);

        // Assert
        Assert.IsType<FileRenameCommand>(res);
        Assert.True(res.Equals(actualCommand));
    }
}