namespace Itmo.ObjectOrientedProgramming.Lab4.Handlers.OptionalParametersHandlers;

public abstract class OptionalParameterHandlerBase<T>
{
    protected OptionalParameterHandlerBase<T>? Next { get; private set; }

    public OptionalParameterHandlerBase<T> AddNext(OptionalParameterHandlerBase<T> handler)
    {
        if (Next is null)
        {
            Next = handler;
        }
        else
        {
            Next.AddNext(handler);
        }

        return this;
    }

    public abstract void Handle(IEnumerator<string> request, T command);
}