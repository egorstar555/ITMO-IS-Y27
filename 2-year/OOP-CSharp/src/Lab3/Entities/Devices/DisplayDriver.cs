using System.Drawing;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Devices;

public class DisplayDriver : IDisplayDriver
{
    private readonly IDisplay _display;

    private Color _color;

    public DisplayDriver(IDisplay display)
    {
        _color = Color.Green;
        _display = display;
    }

    public DisplayDriver(Color color, IDisplay display)
    {
        _color = color;
        _display = display;
    }

    public void Clear()
    {
        Console.Clear();
    }

    public void SetColor(Color color)
    {
        _color = color;
    }

    public void WriteText(string text)
    {
        string outString = Crayon.Output.Rgb(_color.R, _color.G, _color.B).Text(text);
        Clear();
        _display.PrintText(outString);
    }
}