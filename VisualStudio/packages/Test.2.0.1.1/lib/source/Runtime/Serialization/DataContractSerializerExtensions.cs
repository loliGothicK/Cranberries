using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Xml;

namespace Dnp.Runtime.Serialization
{
    public static class DataContractSerializerExtensions
    {
        public static string ContractToString( this DataContractSerializer dataContractSerializer, object contract )
        {
            using ( var stringWriter = new StringWriter() )
            {
                using ( var xmlTextWriter = new XmlTextWriter( stringWriter ) )
                {
                    dataContractSerializer.WriteObject( xmlTextWriter, contract );
                    string result = stringWriter.ToString();
                    return result;
                }
            }
        }
    }
}
