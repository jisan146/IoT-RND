using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using Oracle.DataAccess.Client;
using Oracle.DataAccess.Types;
using System.IO;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        SerialPort sp2 = new SerialPort();

        private delegate void SetTextDeleg(string text);
        OracleConnection conn = new OracleConnection("DATA SOURCE=www.srbuildersbd.com:1521/orcl;PASSWORD=Samsung123#$;USER ID=c11");
        public Form1()
        {
            InitializeComponent();
           
        }

        public void get_string(string text)
        {
            try
            {
                if (conn.State != ConnectionState.Open)
                    conn.Open();
                OracleCommand cmd = new OracleCommand();
                cmd.Connection = conn;
                cmd.CommandText = text; cmd.CommandType = CommandType.Text;
                OracleDataReader dr = cmd.ExecuteReader();
                dr.Read();
                textBox2.Text = dr.GetString(0);
                conn.Close();
            }
            catch (Exception m) { textBox2.Clear();  }
           

        }

        public void data_ins(string text)
        {
            try
            {
                if (conn.State != ConnectionState.Open)
                    conn.Open();
                OracleCommand cmd = new OracleCommand();
                cmd.Connection = conn;
                cmd.CommandText = text; cmd.CommandType = CommandType.Text;
                cmd.ExecuteNonQuery();
                conn.Close();
            }
            catch (Exception m) { textBox2.Clear(); }


        }

        void button1_Click(object sender, EventArgs e)
        {
            get_string("select name from student_information where sl="+textBox3.Text+" and client_id='" + textBox4.Text + "'");
            data_ins("update student_information set CARD_NO=replace(replace('"+textBox1.Text+"',chr(10),''),' ','') where client_id='" + textBox4.Text + "' and sl=" + textBox3.Text);
            textBox2.Clear();
            textBox3.Clear();
        
        }
        private void si_DataReceived(string data)
        {
            Invoke(new Action(() =>
            {
               
                textBox1.Text = data;
                get_string("select sl||'  '||name from student_information where  CARD_NO=replace(replace('" + textBox1.Text + "',chr(10),''),' ','')");
               if (checkBox1.Checked)
                {
                    button3.PerformClick();
                }
               
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
            try
            {

                conn.Open();

                //Retrieve BLOB from database into DataSet.
                OracleCommand cmd = new OracleCommand("SELECT sl, image FROM student_information where (CARD_NO=replace(replace('" + textBox1.Text + "',chr(10),''),' ','') or sl='"+textBox3.Text+"') and client_id='" + textBox4.Text + "'", conn);
                OracleDataAdapter da = new OracleDataAdapter(cmd);
                DataSet ds = new DataSet();
                da.Fill(ds, "student_information");
                int c = ds.Tables["student_information"].Rows.Count;

                if (c > 0)
                {   //BLOB is read into Byte array, then used to construct MemoryStream,
                    //then passed to PictureBox.
                    Byte[] byteBLOBData = new Byte[0];
                    byteBLOBData = (Byte[])(ds.Tables["student_information"].Rows[c - 1]["image"]);
                    MemoryStream stmBLOBData = new MemoryStream(byteBLOBData);
                    pictureBox1.Image = Image.FromStream(stmBLOBData);
                }
                conn.Close();
            }
            catch (Exception ex)
            { pictureBox1.ImageLocation = "2.jpg"; }
        }
       
       
    }
}
