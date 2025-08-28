using Itmo.ObjectOrientedProgramming.Lab1.Entities;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.ValueObjects;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Services;

public class StationSection : IRouteSection
{
    private readonly Speed _maxSpeed;
    private readonly double _occupancy;

    public StationSection(Speed maxSpeed, double occupancy)
    {
        _maxSpeed = maxSpeed;
        _occupancy = occupancy;
    }

    public void TrainPassing(Train train)
    {
        train.CheckSpeedExceed(_maxSpeed);
        if (train.ResultPassing is not ResultPassing.Success) return;
        train.IncreaseTravelTime(_occupancy);
    }
}