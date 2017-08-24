using System;
using System.Collections.Generic;
using System.Text;
using Dnp.Net;

namespace Dnp.Exceptions
{
    public class ExceptionEmailSender
    {
        public ExceptionEmailSender(ISmtpServer smptServer)
        {
            EmailSender = new EmailSender( smptServer );
        }

        private EmailSender EmailSender { get; set; }

        public void Send( string subject,
                          Exception exception,
                          string fromAddress,
                          IEnumerable<string> recipients )
        {
            Send( string.Format(
                    "Unhandled error at {0} for {1}\\{2}.",
                    DateTime.Now,
                    Environment.UserDomainName,
                    Environment.UserName ),
                    subject,
                    exception,
                    fromAddress,
                    recipients );
        }

        public void Send( string header,
                          string subject,
                          Exception exception,
                          string fromAddress,
                          IEnumerable<string> recipients )
        {
            var sb = new StringBuilder();
            sb.AppendLine( header );
            while ( exception != null )
            {
                sb.AppendLine( exception.ToString() );
                sb.AppendLine( "======================================" );
                exception = exception.InnerException;
            }

            string body = sb.ToString();
            EmailSender.Send( subject, body, fromAddress, recipients, false );
        }
    }
}
