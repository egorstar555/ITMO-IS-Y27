using Itmo.ObjectOrientedProgramming.Lab1.Entities.ValueObjects;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities;

public class Train
{
    public ResultPassing ResultPassing { get; private set; }

    private readonly Accuracy _accuracy;

    private readonly Weight _weight;

    private readonly Force _maxForce;

    private Speed _speed;

    private double _acceleration;

    private double _travelTime;

    public Train(Weight weight, Force maxForce, Accuracy accuracy)
    {
        _weight = weight;
        _speed = new Speed(0);
        _acceleration = 0;
        _maxForce = maxForce;
        _accuracy = accuracy;
        _travelTime = 0;
        ResultPassing = new ResultPassing.Success(_travelTime);
    }

    public Distance CalculateDistance(Distance distance)
    {
        return new Distance(distance.Value - (_speed.Value * _accuracy.Value));
    }

    public void IncreaseTravelTime()
    {
        _travelTime += _accuracy.Value;
        ResultPassing = new ResultPassing.Success(_travelTime);
    }

    public void IncreaseTravelTime(double occupancy)
    {
        _travelTime += occupancy;
        ResultPassing = new ResultPassing.Success(_travelTime);
    }

    public void UpdateSpeed()
    {
        _speed = new Speed(_speed.Value + (_acceleration * _accuracy.Value));
        if (_speed.Value <= 0)
        {
            ResultPassing = new ResultPassing.NotEnoughSpeedToCompleteRoute();
        }
    }

    public void CheckEnoughSpeedToCompleteSection()
    {
        if (ResultPassing is not ResultPassing.Success) return;
        if (_acceleration <= 0 && _speed.Value <= 0)
        {
            ResultPassing = new ResultPassing.NotEnoughSpeedToCompleteRoute();
        }
    }

    public void CheckSpeedExceed(Speed maxSpeed)
    {
        if (_speed > maxSpeed)
        {
            ResultPassing = new ResultPassing.SpeedExceeded();
        }
    }

    public void TryApplyForce(Force force)
    {
        if (force > _maxForce)
        {
            ResultPassing = new ResultPassing.TrainMaxForceExceeded();
            return;
        }

        _acceleration = force.Value / _weight.Value;
    }
}