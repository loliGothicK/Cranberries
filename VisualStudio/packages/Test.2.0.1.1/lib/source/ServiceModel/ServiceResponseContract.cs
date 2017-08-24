using System;

namespace Dnp.ServiceModel
{
    public class ServiceResponseContract : IServiceResponseContract
    {
        #region Implementation of IServiceResponseContract<TContract,TResponse>

        public TResponse CallService<TContract, TResponse>( TContract contract,
                                      Func<TContract, TResponse> predicate,
                                      Func<TContract, Exception, TResponse> error = null )
        {
            try
            {
                return predicate( contract );
            }
            catch ( Exception exception )
            {
                if ( ReferenceEquals( error, null ) )
                {
                    throw;
                }
                return error( contract, exception );
            }
        }

        #endregion
    }
}