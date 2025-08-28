namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.SubjectTypes;

public class Credit : ISubjectType
{
    public int MinPointsToPass { get; }

    public Credit(int minPointsToPass)
    {
        MinPointsToPass = minPointsToPass;
    }

    public int GetPoints()
    {
        return 0;
    }
}