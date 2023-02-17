using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Oracle.DataAccess.Client;
using Oracle.DataAccess.Types;
using System.Data;

namespace WindowsFormsApplication1
{
    public partial class Form2 : Form
    {
        OracleConnection conn = new OracleConnection("DATA SOURCE=www.srbuildersbd.com:1521/orcl;PASSWORD=Samsung123#$;USER ID=c11");
        public Form2()
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
                textBox3.Text = dr.GetString(0);
                conn.Close();
            }
            catch (Exception m) { textBox3.Text ="-1"; }


        }
        int a;
        private void button1_Click(object sender, EventArgs e)
        {
            get_string("select to_char(u.CLIENT_ID) from users u, CLIENT_INFORMATION c  WHERE u.client_id=c.client_id and user_id='"+textBox1.Text+"' AND password =DBMS_OBFUSCATION_TOOLKIT.md5 (input => UTL_RAW.cast_to_raw('"+textBox2.Text+"')) AND u.active='1' and c.active='1' ");
            try {
                a = int.Parse(textBox3.Text);
                if (a >= 0)
                {
                    Form1 f = new Form1();
                    f.textBox4.Text = textBox3.Text;
                    f.textBox5.Text = textBox4.Text;
                    this.Hide();
                    f.Show();
                }
            }
            catch { }
           
        }

        private void textBox4_KeyDown(object sender, KeyEventArgs e)
        {

            if (e.KeyCode == Keys.Enter)
            {
                button1.PerformClick();
            }
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                textBox2.Focus();
            }
        }

        private void textBox2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                textBox4.Focus();
            }
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            this.ActiveControl = textBox1;
        }
    }
}
