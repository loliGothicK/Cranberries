using System.Net;
using System.Net.Mail;

namespace Dnp.Net
{
    public class SmtpServer : ISmtpServer
    {
        private readonly SmtpClient _SmtpClient;

        public SmtpServer( string host,
                           int port = 25,
                           bool useDefaultCredentials = true,
                           string user = "",
                           string password = "" )
        {
            _SmtpClient = new SmtpClient( host, port );
            if ( !useDefaultCredentials )
            {
                _SmtpClient.Credentials = new NetworkCredential( user, password );
            }
        }

        #region ISmtpServer Members

        public void Send( MailMessage mailMessage )
        {
            _SmtpClient.Send( mailMessage );
        }

        #endregion
    }
}