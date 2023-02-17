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
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'DataSet1.DEMO_CUSTOMERS' table. You can move, or remove it, as needed.
            this.DEMO_CUSTOMERSTableAdapter.Fill(this.DataSet1.DEMO_CUSTOMERS);

            this.reportViewer1.RefreshReport();
        }
    }
}
