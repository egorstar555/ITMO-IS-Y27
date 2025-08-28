using System.Drawing;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Devices;

public interface IDisplayDriver
{
    void Clear();

    void SetColor(Color color);

    void WriteText(string text);
}