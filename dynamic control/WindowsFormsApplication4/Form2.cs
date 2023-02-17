using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication4
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            aa();
        }
        int a = 1;
        public System.Windows.Forms.Button aa()
        {
            System.Windows.Forms.Button txt = new System.Windows.Forms.Button();
            this.Controls.Add(txt);
            txt.Name ="abc"+ a.ToString();
            txt.Top = a * 28;
            txt.Left = 15;
            txt.Text = "button"+a.ToString();
            a = a + 1;
            txt.Location = new Point(297, 189);
            ControlExtension.Draggable(txt,true);
            return txt;
        }
        public System.Windows.Forms.TextBox tt()
        {
            System.Windows.Forms.TextBox txt = new System.Windows.Forms.TextBox();
            this.Controls.Add(txt);
            txt.Name = "abc" + a.ToString();
            txt.Top = a * 28;
            txt.Left = 15;
            txt.Text = "text" + a.ToString();
            txt.Location =new Point( 297,189);
            a = a + 1;
            ControlExtension.Draggable(txt, true);
            return txt;
        }

        private void Form2_Load(object sender, EventArgs e)
        {
           
        }

        private void button2_Click(object sender, EventArgs e)
        {
            tt();
        }
    }
}
