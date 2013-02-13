using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WebKit.JSCore;

namespace JavaScriptExample
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            webKitBrowser1.Navigate("http://www.google.com/");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            webKitBrowser1.Navigate(textBox1.Text);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            JSValue val = webKitBrowser1.GetScriptManager.EvaluateScript(textBox2.Text);

            if (val != null)
            {
                JSObject result = val.ToObject();
                
                MessageBox.Show(val.ToString());
            }
            else
                MessageBox.Show("Script failed");
        }

        private void loadJSTestPageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            webKitBrowser1.OpenDocument(Application.StartupPath + @"\JSTest\page.html");
        }

        private void executeSaySunToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
            webKitBrowser1.GetScriptManager.CallFunction("saySun", new object[] { });
        }
        catch
        {
            MessageBox.Show("Please load the test file firstly");
        }
        }

        private void executeSayHellostringStrToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                webKitBrowser1.GetScriptManager.CallFunction("sayHello", new object[] { "Hi! Random number:" + new Random().Next(20).ToString() });
            }
            catch
            {
                MessageBox.Show("Please load the test file firstly");
            }
        }
    }
}
