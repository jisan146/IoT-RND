using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace ConsoleApplication2
{
    class Program
    {
        static Socket sck;
        static void Main(string[] args)
        {
            Socket sck = new Socket(AddressFamily.InterNetwork,SocketType.Stream, ProtocolType.Tcp);
            IPEndPoint endpoint = new IPEndPoint(IPAddress.Parse("192.168.0.102"),1994);
            sck.Connect(endpoint);
            Console.Write("Enter");
            string msg = Console.ReadLine();
            byte[] msgBuffer = Encoding.Default.GetBytes(msg);
            sck.Send(msgBuffer,0,msgBuffer.Length,0);
            byte[] buffer = new byte[255];
            int rec = sck.Receive(buffer,0,buffer.Length,0);
            Array.Resize(ref buffer,rec);
            Console.WriteLine("receive {0}",Encoding.Default.GetString(buffer));
            Console.Read();
        }
    }
}
