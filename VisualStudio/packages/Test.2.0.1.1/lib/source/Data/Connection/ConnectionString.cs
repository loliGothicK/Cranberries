using System.Text;

namespace Dnp.Data.Connection
{
    public class ConnectionString
    {
        public string UserID { get; set; }
        public string Password { get; set; }
        public string Server { get; set; }
        public bool? Direct { get; set; }
        public string Sid { get; set; }
        public bool? PersistSecurityInfo { get; set; }
        public string AdditionalConnectionParameters { get; set; }

        public ConnectionString( string userId = null,
                                 string password = null,
                                 string server = null,
                                 bool? direct = null,
                                 string sid = null,
                                 bool? persistSecurityInfo = null,
                                 string additionalConnectionParameters = null
                )
        {
            UserID = userId;
            Password = password;
            Server = server;
            Direct = direct;
            Sid = sid;
            PersistSecurityInfo = persistSecurityInfo;
            AdditionalConnectionParameters = additionalConnectionParameters;
        }

        public static string Create( string userId = null,
                                     string password = null,
                                     string server = null,
                                     bool? direct = null,
                                     string sid = null,
                                     bool? persistSecurityInfo = null,
                                     string additionalConnectionParameters = null )
        {
            return new ConnectionString(userId, password, server, direct, sid, persistSecurityInfo, additionalConnectionParameters)
                .ToString();
        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder();
            result.Append( string.IsNullOrEmpty( UserID ) ? string.Empty : string.Format( "User ID={0};", UserID ) );
            result.Append( string.IsNullOrEmpty( Password ) ? string.Empty : string.Format( "Password={0};", Password ) );
            result.Append( string.IsNullOrEmpty( Server ) ? string.Empty : string.Format( "Server={0};", Server));
            result.Append( ReferenceEquals( Direct, null ) ? string.Empty : string.Format("Direct={0};", Direct));
            result.Append( string.IsNullOrEmpty( Sid ) ? string.Empty : string.Format("Sid={0};", Sid));
            result.Append( ReferenceEquals( PersistSecurityInfo, null )
                                   ? string.Empty
                                   : string.Format( "Persist Security Info={0};", PersistSecurityInfo) );
            result.Append(AdditionalConnectionParameters);

            return result.ToString();
        }
    }
}