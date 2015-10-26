using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Lync.Model;
using System.IO.Ports;

namespace LyncLights
{
    class Program
    {
        public static LyncClient lyncClient;
        public static ContactAvailability currentPresence = ContactAvailability.None;

        static void Main(string[] args)
        {
            Console.WriteLine("Hello!");

            try
            {
                lyncClient = LyncClient.GetClient();
            }
            catch (ClientNotFoundException clientNotFoundException)
            {
                Console.WriteLine(clientNotFoundException);
                return;
            }
            catch (NotStartedByUserException notStartedByUserException)
            {
                Console.Out.WriteLine(notStartedByUserException);
                return;
            }
            catch (LyncClientException lyncClientException)
            {
                Console.Out.WriteLine(lyncClientException);
                return;
            }
            catch (SystemException systemException)
            {
                if (IsLyncException(systemException))
                {
                    // Log the exception thrown by the Lync Model API.
                    Console.WriteLine("Error: " + systemException);
                    return;
                }
                else
                {
                    // Rethrow the SystemException which did not come from the Lync Model API.
                    throw;
                }
            }

            // Initial display
            DisplayContactPresence();

            // Register an event handler so that when the status changes certain code is executed
            lyncClient.Self.Contact.ContactInformationChanged += new EventHandler<ContactInformationChangedEventArgs>(SelfStatusHasChanged);
            
            // Close when the user hits enter
            Console.ReadLine();
        }

        private static void SelfStatusHasChanged(object sender, ContactInformationChangedEventArgs e)
        {
            // Console.WriteLine("FOOBAR");
            if (GetLyncAvailability() != currentPresence)
            {
                DisplayContactPresence();
            }
        }


        private static void DisplayContactPresence() 
        {
            ContactAvailability myStatus = GetLyncAvailability();
            Char myStatusChar = GetAvailabilityChar(myStatus);
            Console.WriteLine("Status: " + myStatusChar);
            SendSerialData(myStatusChar);
            currentPresence = myStatus;
        }

        private static ContactAvailability GetLyncAvailability()
        {
            
            //Get the current availability value from Lync
            ContactAvailability currentAvailability = 0;
            try
            {
                currentAvailability = (ContactAvailability) lyncClient.Self.Contact.GetContactInformation(ContactInformationType.Availability);
            }
            catch (LyncClientException e)
            {
                Console.WriteLine(e);
            }
            catch (SystemException systemException)
            {
                if (IsLyncException(systemException))
                {
                    // Log the exception thrown by the Lync Model API.
                    Console.WriteLine("Error: " + systemException);
                }
                else
                {
                    // Rethrow the SystemException which did not come from the Lync Model API.
                    throw;
                }
            }


            return currentAvailability;
        }

        private static char GetAvailabilityChar(ContactAvailability currentAvailability)
        {
            switch (currentAvailability)
            {
                case ContactAvailability.Free:
                    return 'a';
                case ContactAvailability.Away:
                    return 'w';
                case ContactAvailability.Busy:
                    return 'b';
                case ContactAvailability.BusyIdle:
                    return 'b';
                case ContactAvailability.DoNotDisturb:
                    return 'd';
                case ContactAvailability.FreeIdle:
                    return 'a';
                case ContactAvailability.TemporarilyAway:
                    return 'w';
                case ContactAvailability.Offline:
                    return 'r';
                default:
                    return 'r';
            }
        }

        private static bool IsLyncException(SystemException ex)
        {
            return
                ex is NotImplementedException ||
                ex is ArgumentException ||
                ex is NullReferenceException ||
                ex is NotSupportedException ||
                ex is ArgumentOutOfRangeException ||
                ex is IndexOutOfRangeException ||
                ex is InvalidOperationException ||
                ex is TypeLoadException ||
                ex is TypeInitializationException ||
                //ex is InvalidComObjectException ||
                ex is InvalidCastException;
        }

        private static void SendSerialData(char data)
        {
            // Instantiate the communications
            // port with some basic settings
            SerialPort port = new SerialPort(
              "COM12", 9600, Parity.None, 8, StopBits.One);

            // Open the port for communications
            port.Open();

            // Write a string
            port.Write(data.ToString());

            // Write a set of bytes
            //port.Write(new byte[] { 0x0A, 0xE2, 0xFF }, 0, 3);

            // Close the port
            port.Close();
        }
    }
}
