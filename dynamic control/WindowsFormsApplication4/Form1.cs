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
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            INIT();
        }
        private Point firstPoint = new Point();
        private Button bb = new Button();
        public void INIT()
        {
          
         
          
            bb.MouseDown += (ss, ee) =>
            {
               
                    if(ee.Button==System.Windows.Forms.MouseButtons.Left)
                    { firstPoint = Control.MousePosition; }
                   
                };
            bb.MouseMove += (ss, ee) =>
            {
                if (ee.Button == System.Windows.Forms.MouseButtons.Left) 
                {
                    Point temp = Control.MousePosition;
                    Point res = new Point(firstPoint.X-temp.X,firstPoint.Y-temp.Y);
                    bb.Location = new Point(bb.Location.X - res.X, bb.Location.Y - res.Y);
                    firstPoint = temp;
                }
            };
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            Button bbe = new Button();
            bbe.Name = "button2";
            ControlExtension.Draggable(bbe,true);
            bbe.Text = "fsd";
        }

        private void panel1_Click(object sender, EventArgs e)
        {
            //bb.Name = "button2";
         //   Button ccbc = sender as Button;
           // ccbc.Name = "button1";
            textBox1.Text = button2.Location.ToString();
          
           // MessageBox.Show("demo");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int a, b;
            a = int.Parse(textBox2.Text);
            b = int.Parse(textBox3.Text);

            bb.Location = new Point(a, b);
        }
    }
}
