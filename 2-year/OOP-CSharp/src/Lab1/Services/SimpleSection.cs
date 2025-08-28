using Itmo.ObjectOrientedProgramming.Lab1.Entities;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.ValueObjects;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Services;

public class SimpleSection : IRouteSection
{
    private readonly Distance _distance;

    public SimpleSection(Distance distance)
    {
        _distance = distance;
    }

    public void TrainPassing(Train train)
    {
        train.CheckEnoughSpeedToCompleteSection();
        if (train.ResultPassing is not ResultPassing.Success) return;

        Distance tempDistance = _distance;

        while (tempDistance.Value > 0)
        {
            train.IncreaseTravelTime();
            tempDistance = train.CalculateDistance(tempDistance);
        }
    }
}