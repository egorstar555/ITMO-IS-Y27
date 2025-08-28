using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Interfaces;

public interface IStatus
{
    public ObjectStatus GetLastStatus();
}