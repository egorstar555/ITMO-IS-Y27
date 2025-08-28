using Itmo.ObjectOrientedProgramming.Lab1.Entities;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.ValueObjects;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Services;

public class ForceSection : IRouteSection
{
    private readonly Distance _distance;
    private readonly Force _force;

    public ForceSection(Distance distance, Force force)
    {
        _distance = distance;
        _force = force;
    }

    public void TrainPassing(Train train)
    {
        train.TryApplyForce(_force);
        train.CheckEnoughSpeedToCompleteSection();
        if (train.ResultPassing is not ResultPassing.Success) return;

        Distance tempDistance = _distance;

        while (tempDistance.Value > 0 && train.ResultPassing is ResultPassing.Success)
        {
            train.IncreaseTravelTime();
            train.UpdateSpeed();

            tempDistance = train.CalculateDistance(tempDistance);
        }

        train.TryApplyForce(new Force(0));
    }
}