using System;

namespace Dnp.ServiceModel
{
    public interface IServiceResponseContract
    {
        TResponse CallService<TContract, TResponse>( TContract contract,
                               Func<TContract, TResponse> predicate,
                               Func<TContract, Exception, TResponse> error = null );
    }
}