using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;



namespace WindowsFormsApplication1
{
    public partial class Form2 : Form
    {
          public Form2()
        {
            InitializeComponent();
          
        }
    
        int a;
        private void button1_Click(object sender, EventArgs e)
        {
           
                    Form1 f = new Form1();
                  //  f.textBox4.Text = textBox3.Text;
                    f.textBox5.Text = textBox4.Text;
                    f.Show();
                    this.Hide();
              
           
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
            this.ActiveControl = textBox4;
        }
    }
}
