using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;
using System.Net;
using System.Text;
using System.Net.Sockets;

namespace WebApplication2
{
    public partial class WebForm2 : System.Web.UI.Page
    {
        private void msgbox(string msg)
        {
            ScriptManager.RegisterClientScriptBlock(this.Page, this.Page.GetType(), "CallMyFunction", "alert('" + msg + "')", true);
        }
        protected void Page_Load(object sender, EventArgs e)
        {
            var client = new MqttClient(IPAddress.Parse("192.168.0.102"));

            // register to message received
            client.MqttMsgPublishReceived += client_MqttMsgPublishReceived;

            var clientId = Guid.NewGuid().ToString();
            client.Connect(clientId);

            // subscribe to the topic "/home/temperature" with QoS 2
            client.Subscribe(
                new string[] { "test" },
                new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });
        }
        string s;
        protected void Button1_Click(object sender, EventArgs e)
        {
         
          
        }
        void client_MqttMsgPublishReceived(
     object sender, MqttMsgPublishEventArgs e)
        {
            // handle message received  e.Message.ToString()
           // TextBox1.Text = Encoding.UTF8.GetString(e.Message);
            msgbox(Encoding.UTF8.GetString(e.Message));

            // MessageBox.Show(s);
            //Console.WriteLine("message=" + Encoding.UTF8.GetString(e.Message));



        }

        protected void Button2_Click(object sender, EventArgs e)
        {
            TextBox1.Text = s;
        }
    }
}