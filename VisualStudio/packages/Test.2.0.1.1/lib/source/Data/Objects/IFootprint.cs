using System;

namespace Dnp.Data.Objects
{
    public interface IFootprint
    {
        DateTime? LastUpdateTimestamp { get; set; }

        DateTime? CreateTimestamp { get; set; }
    }
}
