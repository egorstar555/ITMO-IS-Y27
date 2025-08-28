using Itmo.ObjectOrientedProgramming.Lab1.Entities;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.ValueObjects;
using Itmo.ObjectOrientedProgramming.Lab1.Models;
using Itmo.ObjectOrientedProgramming.Lab1.Services;
using Xunit;

namespace Lab1.Tests;

public class TestsRouts
{
    private readonly Train _train;

    public TestsRouts()
    {
        _train = new Train(new Weight(3), new Force(100), new Accuracy(1));
    }

    [Fact]
    public void AccelerationBelowMaximumRouteSpeed_Success()
    {
        // Arrange
        var section1 = new ForceSection(new Distance(10), new Force(2));
        var section2 = new SimpleSection(new Distance(12));

        var route = new Route([section1, section2], new Speed(40));

        // Act
        route.TrainPassing(_train);

        // Assert
        Assert.IsType<ResultPassing.Success>(_train.ResultPassing);
    }

    [Fact]
    public void AccelerationAboveMaximumRouteSpeed_Invalid()
    {
        // Arrange
        var section1 = new ForceSection(new Distance(10), new Force(61));
        var section2 = new SimpleSection(new Distance(12));

        var route = new Route([section1, section2], new Speed(20));

        // Act
        route.TrainPassing(_train);

        // Assert
        Assert.IsType<ResultPassing.SpeedExceeded>(_train.ResultPassing);
    }

    [Fact]
    public void AccelerationBelowMaximumRouteAndStationSpeed_Success()
    {
        // Arrange
        var section1 = new ForceSection(new Distance(10), new Force(40));
        var section2 = new SimpleSection(new Distance(12));
        var section3 = new StationSection(new Speed(50), 10);
        var section4 = new SimpleSection(new Distance(12));

        var route = new Route([section1, section2, section3, section4], new Speed(40));

        // Act
        route.TrainPassing(_train);

        // Assert
        Assert.IsType<ResultPassing.Success>(_train.ResultPassing);
    }

    [Fact]
    public void AccelerationAboveMaximumStationSpeed_Invalid()
    {
        // Arrange
        var section1 = new ForceSection(new Distance(10), new Force(40));
        var section2 = new StationSection(new Speed(10), 10);
        var section3 = new SimpleSection(new Distance(12));

        var route = new Route([section1, section2, section3], new Speed(40));

        // Act
        route.TrainPassing(_train);

        // Assert
        Assert.IsType<ResultPassing.SpeedExceeded>(_train.ResultPassing);
    }

    [Fact]
    public void AccelerationAboveRouteButBelowStationMaximumSpeed_Invalid()
    {
        // Arrange
        var section1 = new ForceSection(new Distance(10), new Force(40));
        var section2 = new SimpleSection(new Distance(12));
        var section3 = new StationSection(new Speed(14), 10);
        var section4 = new SimpleSection(new Distance(12));

        var route = new Route([section1, section2, section3, section4], new Speed(10));

        // Act
        route.TrainPassing(_train);

        // Assert
        Assert.IsType<ResultPassing.SpeedExceeded>(_train.ResultPassing);
    }

    [Fact]
    public void AccelerationsAboveAndDecelerationsBelowRouteAndStationMaximumSpeed_Success()
    {
        // Arrange
        var section1 = new ForceSection(new Distance(10), new Force(40));
        var section2 = new SimpleSection(new Distance(12));
        var section3 = new ForceSection(new Distance(10), new Force(-10));
        var section4 = new StationSection(new Speed(14), 10);
        var section5 = new SimpleSection(new Distance(12));
        var section6 = new ForceSection(new Distance(10), new Force(50));
        var section7 = new SimpleSection(new Distance(12));
        var section8 = new ForceSection(new Distance(10), new Force(-50));

        var route = new Route(
            [section1, section2, section3, section4, section5, section6, section7, section8],
            new Speed(15));

        // Act
        route.TrainPassing(_train);

        // Assert
        Assert.IsType<ResultPassing.Success>(_train.ResultPassing);
    }

    [Fact]
    public void NoAcceleration_Invalid()
    {
        // Arrange
        var section1 = new SimpleSection(new Distance(12));

        var route = new Route([section1], new Speed(15));

        // Act
        route.TrainPassing(_train);

        // Assert
        Assert.IsType<ResultPassing.NotEnoughSpeedToCompleteRoute>(_train.ResultPassing);
    }

    [Fact]
    public void SlowingDownMoreThanAccelerating_Invalid()
    {
        // Arrange
        var section1 = new ForceSection(new Distance(10), new Force(40));
        var section2 = new ForceSection(new Distance(10), new Force(-80));

        var route = new Route([section1, section2], new Speed(15));

        // Act
        route.TrainPassing(_train);

        // Assert
        Assert.IsType<ResultPassing.NotEnoughSpeedToCompleteRoute>(_train.ResultPassing);
    }

    [Fact]
    public void TrainMaxForceExceeded_Invalid()
    {
        // Arrange
        var section1 = new ForceSection(new Distance(10), new Force(400));

        var route = new Route([section1], new Speed(15));

        // Act
        route.TrainPassing(_train);

        // Assert
        Assert.IsType<ResultPassing.TrainMaxForceExceeded>(_train.ResultPassing);
    }
}