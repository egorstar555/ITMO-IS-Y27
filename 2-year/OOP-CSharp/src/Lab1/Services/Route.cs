using Itmo.ObjectOrientedProgramming.Lab1.Entities;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.ValueObjects;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Services;

public class Route
{
    private readonly List<IRouteSection> _routeSections;
    private readonly Speed _maxSpeed;

    public Route(IEnumerable<IRouteSection> routeSections, Speed maxSpeed)
    {
        _routeSections = routeSections.ToList();
        _maxSpeed = maxSpeed;
    }

    public void TrainPassing(Train train)
    {
        foreach (IRouteSection section in _routeSections)
        {
            section.TrainPassing(train);
            if (train.ResultPassing is not ResultPassing.Success) return;
        }

        train.CheckSpeedExceed(_maxSpeed);
    }
}