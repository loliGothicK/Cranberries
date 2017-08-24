using System.Collections.Generic;
using System.Linq;
using System.Net.Mail;

namespace Dnp.Net
{
    public class EmailSender
    {
        private readonly ISmtpServer _SmtpServer;

        public EmailSender( ISmtpServer smtpServer )
        {
            _SmtpServer = smtpServer;
        }

        public void Send( string subject,
                          string body,
                          string fromAddress,
                          IEnumerable<string> recipients,
                          bool isBodyHtml = true )
        {
            using ( MailMessage mailMessage = GetMailMessage( subject, body, fromAddress, recipients ) )
            {
                mailMessage.IsBodyHtml = isBodyHtml;
                _SmtpServer.Send( mailMessage );
            }
        }

        private static MailMessage GetMailMessage( string subject,
                                                   string body,
                                                   string fromAddress,
                                                   IEnumerable<string> recipients )
        {
            var mailMessage = new MailMessage { From = new MailAddress( fromAddress ) };
            foreach ( MailAddress recipientAddress in recipients.Select( recipient => new MailAddress( recipient ) ) )
            {
                mailMessage.To.Add( recipientAddress );
            }
            mailMessage.Body = body;
            mailMessage.Subject = subject;
            return mailMessage;
        }
    }
}