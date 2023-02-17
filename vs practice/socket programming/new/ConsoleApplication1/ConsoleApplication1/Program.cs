using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace ConsoleApplication1
{
    class Program
    {
        static byte[] Buffer { get; set; }
        static Socket sck;
        static void Main(string[] args)
        {
            sck = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            sck.Bind(new IPEndPoint(0, 1994));
            sck.Listen(100);
            Socket accepted = sck.Accept();
            byte[] buffer = Encoding.Default.GetBytes("Hello");
            accepted.Send(buffer,0,buffer.Length,0);
            buffer = new byte[255];
            int rec = accepted.Receive(buffer,0,buffer.Length,0);
            Array.Resize(ref buffer,rec);
            Console.WriteLine("receive: {0}",Encoding.Default.GetString(buffer));
            //sck.Close();
            //accepted.Close();
            Console.Read();
            
           /* Buffer = new byte[accepted.SendBufferSize];
            int bytesRead = accepted.Receive(Buffer);
            byte[] formatted = new byte[bytesRead];
            for (int i = 0; i < bytesRead; i++)
            {
                formatted[i] = Buffer[i];
            }
            string strData = Encoding.ASCII.GetString(formatted);
            Console.Write(strData + "\r\n");
            Console.Read();
            sck.Close();
            accepted.Close();*/
        }
    }
}
