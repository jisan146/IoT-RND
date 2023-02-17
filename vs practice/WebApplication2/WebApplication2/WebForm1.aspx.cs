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

namespace WebApplication2
{
    public partial class WebForm1 : System.Web.UI.Page
    {
        string s;
        protected void Page_Load(object sender, EventArgs e)
        {

        }

        protected void Button1_Click(object sender, EventArgs e)
        {
            MqttClient client;
            string clientId;
            client = new MqttClient(IPAddress.Parse("192.168.0.102"));
            clientId = Guid.NewGuid().ToString();
            client.Connect(clientId);
            client.Publish("test", Encoding.UTF8.GetBytes("hello"));
        }
       
      

      
    }
}