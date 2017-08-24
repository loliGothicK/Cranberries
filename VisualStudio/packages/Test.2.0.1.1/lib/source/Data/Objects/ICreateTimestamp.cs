using System;

namespace Dnp.Data.Objects
{
    public interface ICreateTimestamp
    {
        DateTime? LastUpdateTimestamp { get; set; }

        DateTime CreateTimestamp { get; set; }
    }
}
