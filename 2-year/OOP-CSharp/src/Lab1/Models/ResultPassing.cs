namespace Itmo.ObjectOrientedProgramming.Lab1.Models;

public abstract record ResultPassing
{
    private ResultPassing() { }

    public sealed record Success(double TravelTime) : ResultPassing;

    public sealed record TrainMaxForceExceeded : ResultPassing;

    public sealed record SpeedExceeded : ResultPassing;

    public sealed record NotEnoughSpeedToCompleteRoute : ResultPassing;
}