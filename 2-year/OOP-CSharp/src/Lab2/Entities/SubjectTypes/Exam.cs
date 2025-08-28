namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.SubjectTypes;

public class Exam : ISubjectType
{
    public int ExamPoints { get; }

    public Exam(int examPoints)
    {
        ExamPoints = examPoints;
    }

    public int GetPoints()
    {
        return ExamPoints;
    }
}