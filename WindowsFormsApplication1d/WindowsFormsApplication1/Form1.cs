using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        SerialPort sp2 = new SerialPort();

        private delegate void SetTextDeleg(string text);
        public Form1()
        {
            InitializeComponent();
            sp2.BaudRate = 9600;
            sp2.Parity = Parity.None;
            sp2.DataBits = 8;
            sp2.StopBits = StopBits.One;
            sp2.PortName = "COM3";
            sp2.Handshake = Handshake.None;
        }
       

        void button1_Click(object sender, EventArgs e)
        {
            textBox1.Text += "Read click" + "\n";
        }
        private void si_DataReceived(string data)
        {
            Invoke(new Action(() =>
            {
                textBox1.Text = data;
            }));
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            sp2.Open();
            sp2.DataReceived += new SerialDataReceivedEventHandler(sp_DataReceived);
        }
        void sp_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {


            string data = sp2.ReadLine();
            this.BeginInvoke(new SetTextDeleg(si_DataReceived), new object[] { data });
        }
       
       
    }
}
