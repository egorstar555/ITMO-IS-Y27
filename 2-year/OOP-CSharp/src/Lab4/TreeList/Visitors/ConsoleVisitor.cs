using Itmo.ObjectOrientedProgramming.Lab4.TreeList.FileSystemStructure;
using Itmo.ObjectOrientedProgramming.Lab4.Writer;

namespace Itmo.ObjectOrientedProgramming.Lab4.TreeList.Visitors;

public class ConsoleVisitor : IFileSystemComponentVisitor
{
    private readonly int _maxDepth;

    private readonly IWriter _writer;

    private readonly string _fileSymbol;

    private readonly string _directorySymbol;

    private readonly string _indentSymbol;

    private int _depth;

    public ConsoleVisitor(IWriter writer, string fileSymbol, string directorySymbol, string indentSymbol) : this(
        writer,
        1,
        fileSymbol,
        directorySymbol,
        indentSymbol) { }

    public ConsoleVisitor(IWriter writer, int maxDepth, string fileSymbol, string directorySymbol, string indentSymbol)
    {
        _depth = 0;
        _maxDepth = maxDepth;
        _fileSymbol = fileSymbol;
        _directorySymbol = directorySymbol;
        _indentSymbol = indentSymbol;
        _writer = writer;
    }

    public void Visit(FileFileSystemComponent component)
    {
        WriteIndented(component.Name, _fileSymbol);
    }

    public void Visit(DirectoryFileSystemComponent component)
    {
        WriteIndented(component.Name, _directorySymbol);

        _depth += 1;

        if (_depth >= _maxDepth)
        {
            _depth -= 1;
            return;
        }

        foreach (IFileSystemComponent innerComponent in component.Components)
        {
            innerComponent.Accept(this);
        }

        _depth -= 1;
    }

    private void WriteIndented(string value, string symbol)
    {
        if (_depth is not 0)
        {
            _writer.Write(string.Concat(Enumerable.Repeat(_indentSymbol, _depth)));
        }

        _writer.Write(symbol);
        _writer.WriteLine(value);
    }
}