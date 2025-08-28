using System.Diagnostics.CodeAnalysis;

namespace Lab5.Presentation.Console.Scenarios.ExitFromAtm;

public class ExitScenarioProvider : IScenarioProvider
{
    public bool TryGetScenario(
        [NotNullWhen(true)] out IScenario? scenario)
    {
        scenario = new ExitScenario();
        return true;
    }
}