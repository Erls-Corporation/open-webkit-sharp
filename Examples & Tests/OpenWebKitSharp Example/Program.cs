using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace OpenWebKitSharp_Test
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            WebKit.GlobalPreferences.DownloadsFolder = "C:\\Test Download";
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
