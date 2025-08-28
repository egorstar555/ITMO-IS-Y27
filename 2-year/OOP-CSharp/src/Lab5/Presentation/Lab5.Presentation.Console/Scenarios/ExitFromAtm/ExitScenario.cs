namespace Lab5.Presentation.Console.Scenarios.ExitFromAtm;

public class ExitScenario : IScenario
{
    public string Name => "Exit";

    public void Run()
    {
        Environment.Exit(0);
    }
}