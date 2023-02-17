using System;
using System.Collections.Generic;
using System.ComponentModel;

using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

using System.IO;
using System.Net;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        SerialPort sp2 = new SerialPort();

        private delegate void SetTextDeleg(string text);
         public Form1()
        {
            InitializeComponent();
           
        }

      

       

        void button1_Click(object sender, EventArgs e)
        {
            webBrowser1.Url = new Uri("http://id.srbuildersbd.com/?sl=" + textBox3.Text + "&cd=" + textBox1.Text + "&u=1");
            textBox3.Clear();
        
        }
        private void si_DataReceived(string data)
        {
            Invoke(new Action(() =>
            {
               
                textBox1.Text = data;
                webBrowser2.Url = new Uri("http://id.srbuildersbd.com:8086/?p=" + textBox1.Text );
                webBrowser1.Url = new Uri("http://id.srbuildersbd.com/?cd=" + textBox1.Text + "&u=0&sl=0");
              

               
             
              
               
            }));
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            this.ActiveControl = textBox3;
            sp2.BaudRate = 9600;
            sp2.Parity = Parity.None;
            sp2.DataBits = 8;
            sp2.StopBits = StopBits.One;
            sp2.PortName = "COM" + textBox5.Text;
            sp2.Handshake = Handshake.None;
            sp2.Open();
            sp2.DataReceived += new SerialDataReceivedEventHandler(sp_DataReceived);
        }
        void sp_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            
            string data = sp2.ReadLine();

            WebClient w = new WebClient();
            string p = w.DownloadString("http://id.srbuildersbd.com/WebForm2.aspx?cd=" + data + "&u=0&sl=0");
            sp2.WriteLine(p);
            this.BeginInvoke(new SetTextDeleg(si_DataReceived), new object[] { data });


        }

        private void textBox3_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                button1.PerformClick();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void button3_Click(object sender, EventArgs e)
        {
           
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            webBrowser1.Url = new Uri("http://id.srbuildersbd.com/?sl="+textBox3.Text+"&u=0&cd=0");
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }
       
       
    }
}
