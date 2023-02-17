using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.IO;
using System.ServiceProcess;


namespace ConsoleApplication1
{
    class Program
    {
      
        static void Main(string[] args)
        {
            
            
                string text;
                var fileStream = new FileStream(@"c:\file.txt", FileMode.Open, FileAccess.Read);
                using (var streamReader = new StreamReader(fileStream, Encoding.UTF8))
                {
                    text = streamReader.ReadToEnd();
                }


                int fCount = Directory.GetFiles(@"C:\backup\backup\", "*", SearchOption.TopDirectoryOnly).Length;


                Process p = new Process();
                ProcessStartInfo startInfo = new ProcessStartInfo();
                startInfo.FileName = "expdp";
                startInfo.Arguments = "backup/Samsung123#$  schemas=" + text + " directory=backup dumpfile=" + fCount + "_no_" + DateTime.Now.ToString("dd_MMM_yy_hh_mm_ss_tt_") + ".dmp   logfile=" + fCount + "_no_" + DateTime.Now.ToString("dd_MMM_yy_hh_mm_ss_tt");
                p.StartInfo = startInfo;
                p.Start();

               /* Console.WriteLine("expdp " + "backup/Samsung123#$  schemas=" + text + "  dumpfile=" + fCount + "_no_" + DateTime.Now.ToString("dd_MMM_yy_hh_mm_ss_tt_") + ".dmp directory=backup  logfile=" + fCount + "_no_" + DateTime.Now.ToString("dd_MMM_yy_hh_mm_ss_tt"));
                Console.ReadLine();*/
           
            
            
        }
    }
}
