using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;

namespace WindowsFormsApplication2
{
    public partial class Form1 : Form
    {
        Socket sck;
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            sck = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try 
            {
                sck.Connect(new IPEndPoint(IPAddress.Parse("192.168.0.102"),int.Parse(textBox2.Text)));

            }

            catch
            {
                MessageBox.Show("Fail");
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            byte[] data = Encoding.Default.GetBytes(textBox1.Text);
            sck.Send(BitConverter.GetBytes(data.Length),0,4,0);
            sck.Send(data);
           // sck.Close();

        }
    }
}
