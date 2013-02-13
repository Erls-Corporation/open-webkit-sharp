using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WebKit;
using WebKit.DOM;
using WebKit.Interop;

namespace OpenWebKitSharp_Test
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void webKitBrowser1_CloseWindowRequest(object sender, EventArgs e)
        {
            tabControl1.Controls.Remove((sender as WebKitBrowser).Parent);
        }

        private void webKitBrowser1_DangerousSiteDetected(object sender, EventArgs e)
        {
            MessageBox.Show("The site " + (sender as WebKitBrowser).Url.ToString() + " is considered dangerous and it is recommended that you leave.");
        }

        private void webKitBrowser1_FormSubmit(object sender, WebKit.FormDelegateFormEventArgs e)
        {
            //e.Listener.continueSubmit();  from 1.5 Beta 2 this is automatically called
        }

        private void webKitBrowser1_NewWindowCreated(object sender, WebKit.NewWindowCreatedEventArgs e)
        {
            AddTab(e.WebKitBrowser);
        }

        void wb_DocumentTitleChanged(object sender, EventArgs e)
        {
            ((Form)((WebKitBrowser)sender).Parent).Text = ((WebKitBrowser)sender).DocumentTitle;
        }

        void wb_FaviconAvaiable(object sender, FaviconAvailableEventArgs e)
        {
            ((Form)((WebKitBrowser)sender).Parent).Icon = e.Favicon;
        }

        private void webKitBrowser1_StatusTextChanged(object sender, WebKit.WebKitBrowserStatusChangedEventArgs e)
        {
            if (sender.Equals(webKitBrowser1))
            statusLabel.Text = e.StatusText; // link hovering status text is only supported in nightly builds
        }

        private void webKitBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            urlBar.AutoCompleteCustomSource.Add(e.Url.ToString());
            if (sender.Equals(webKitBrowser1))
                urlBar.Text = e.Url.ToString();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            webKitBrowser1.Navigate(urlBar.Text);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            webKitBrowser1.GoBack();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            webKitBrowser1.GoForward();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            webKitBrowser1.Reload();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            webKitBrowser1.Stop();
        }

        private void webKitBrowser1_ProgressChanged(object sender, WebKit.ProgressChangesEventArgs e)
        {
            toolStripProgressBar1.Value = e.Percent;
        }

        private void webKitBrowser1_DocumentTitleChanged(object sender, EventArgs e)
        {
            string ft;
            string dt = (sender as WebKitBrowser).DocumentTitle;
            if (dt.Length > 30)
                ft = dt.Substring(0, 30) + "...";
            else
                ft = dt;
            (sender as WebKitBrowser).Parent.Text = ft;
        }

        private void webKitBrowser1_FaviconAvaiable(object sender, WebKit.FaviconAvailableEventArgs e)
        {
            if (e.Favicon != null)
            {
                fav.Visible = true;
                fav.Image = e.Favicon.ToBitmap();
            }
            else
                fav.Visible = false;
        }

        private void webKitBrowser1_ShowJavaScriptPromptPanel(object sender, WebKit.ShowJavaScriptPromptPanelEventArgs e)
        {
            e.ReturnValue = Microsoft.VisualBasic.Interaction.InputBox(e.Message, "", e.DefaultValue);
        }

        private void webKitBrowser1_ShowJavaScriptAlertPanel(object sender, WebKit.ShowJavaScriptAlertPanelEventArgs e)
        {
            MessageBox.Show(e.Message);
        }

        private void webKitBrowser1_ShowJavaScriptConfirmPanel(object sender, WebKit.ShowJavaScriptConfirmPanelEventArgs e)
        {
            bool val = (MessageBox.Show(e.Message,"OpenWebKitSharp Example",MessageBoxButtons.OKCancel) == System.Windows.Forms.DialogResult.OK);
            e.ReturnValue = val;
        }

        private void openPageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog opn = new OpenFileDialog())
            {
                opn.Filter = "HTML Files (*.html; *.htm)|*.htm; *.htm";
                if (opn.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    webKitBrowser1.OpenDocument(opn.FileName);
                }
            }
        }

        private void savePageAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            webKitBrowser1.ShowSaveAsDialog();
        }

        private void showPageSetupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            webKitBrowser1.ShowPageSetupDialog();
        }

        private void showPrintPreviewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            webKitBrowser1.ShowPrintPreviewDialog();
        }

        private void showPrintDialogToolStripMenuItem_Click(object sender, EventArgs e)
        {
            webKitBrowser1.ShowPrintDialog();
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("This will test the downloading of GT Web Browser 3 installer.");
            webKitBrowser1.Navigate("http://gtwebbrowser.webs.com//Downloads/GT%20Web%20Browser%203%20Setup.msi");
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (webKitBrowser1 != null)
            {
                webKitBrowser1.AllowCookies = checkBox1.Checked;
            }
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (webKitBrowser1 != null)
            {
                webKitBrowser1.UseJavaScript = checkBox2.Checked;
            }
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (webKitBrowser1 != null)
            {
                webKitBrowser1.Preferences.AllowPlugins = checkBox3.Checked;
            }
        }


        private void toolStripMenuItem2_CheckStateChanged(object sender, EventArgs e)
        {
            webKitBrowser1.PrivateBrowsing = toolStripMenuItem2.Checked;
        }


        private void Form1_Load(object sender, EventArgs e)
        {
           AddTab("http://code.google.com/p/open-webkit-sharp/");
        }

        void ResourceIntercepter_ResourceFailedLoading(object sender, WebKitResourceErrorEventArgs e)
        {
            richTextBox1.Text = richTextBox1.Text + e.Resource.Url + " with the type " + e.Resource.MimeType + " failed to load because of this error: " + e.ErrorDescription + "\r\n";
        }

        void ResourceIntercepter_ResourcesSendRequest(object sender, WebKitResourceRequestEventArgs e)
        {
            // here you can handle Resources' Requests

            //Examples (Uncomment so that they take effect):

            //if (e.ResourceUrl.EndsWith(".js")) // Block all scripts
            //    e.SendRequest = false;

            //if (e.ResourceUrl.EndsWith(".css")) // Block all css
            //    e.SendRequest = false;

            //if (e.ResourceUrl.EndsWith(".swf")) // Block flash
            //    e.SendRequest = false;

            //if (e.ResourceUrl.EndsWith(".jpg")) // Block all jpg images
            //    e.SendRequest = false;
        }

        void ResourceIntercepter_ResourceStartedLoadingEvent(object sender, WebKitResourcesEventArgs e)
        {
            richTextBox1.Text = richTextBox1.Text + e.Resource.Url + " with the type " + e.Resource.MimeType + " has started loading." + "\r\n";
        }

        void ResourceIntercepter_ResourceFinishedLoadingEvent(object sender, WebKitResourcesEventArgs e)
        {
            richTextBox1.Text = richTextBox1.Text + e.Resource.Url + " has finished loading." + "\r\n";
        }

        void ResourceIntercepter_ResourceProgressChangedEvent(object sender, WebKitLoadingResourceEventArgs e)
        {
            if (e.Received > -1)
                richTextBox1.Text = richTextBox1.Text + e.Received + " bytes have been received" + "\r\n";
            else
                richTextBox1.Text = richTextBox1.Text + " the number of the bytes that have been received is not available" + "\r\n";
        }

        

        void CustomContextMenuManager_ShowContextMenu(object sender, ShowContextMenuEventArgs e)
        {
            if (!webKitBrowser1.UseDefaultContextMenu)
            {
                ContextMenu tst = new ContextMenu();
                MenuItem mn = new MenuItem("Test1");
                tst.MenuItems.Add(mn);
                MenuItem mn2 = new MenuItem("Test2");
                tst.MenuItems.Add(mn2);
                MenuItem mn3 = new MenuItem("-");
                tst.MenuItems.Add(mn3);
                MenuItem mn4 = new MenuItem("Test3");
                tst.MenuItems.Add(mn4);
                tst.Show(webKitBrowser1, e.Location);
            }
        }

        private void findToolStripMenuItem_Click(object sender, EventArgs e)
        {
            panel2.Visible = true;
        }

        private void button7_Click(object sender, EventArgs e)
        {
            panel2.Visible = false;
            webKitBrowser1.UnmarkTextMatches();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            uint totalmatches; // this is how many matches of the find method are found
            webKitBrowser1.Find(textBox1.Text, out totalmatches);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            panel1.Visible = false;
        }

        private void preferencesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            panel1.Visible = true;
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            if (webKitBrowser1 != null)
               webKitBrowser1.SetPageZoom((float)numericUpDown1.Value);
        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {
            if (webKitBrowser1 != null)
               webKitBrowser1.SetTextZoom((float)numericUpDown2.Value);
        }

        private void button9_Click(object sender, EventArgs e)
        {
            panel3.Visible = false;
        }

        private void zoomToolStripMenuItem_Click(object sender, EventArgs e)
        {
            panel3.Visible = true;
        }


        private void webKitBrowser1_ShowJavaScriptPromptBeforeUnload(object sender, ShowJavaScriptPromptBeforeUnloadEventArgs e)
        {
            if (MessageBox.Show(e.Message, "OpenWebKitSharp Example", MessageBoxButtons.YesNoCancel) == System.Windows.Forms.DialogResult.Yes)
            {
                e.ReturnValue = true;
            }
            else
            {
                e.ReturnValue = false;
            }
        }

        private void applyCSSFromLocalFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog opn = new OpenFileDialog())
            {
                opn.Filter = "CSS Files (*.css)|*.css";
                if (opn.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    webKitBrowser1.CSSManager.SetPageStyleSheetFromLocalFile(opn.FileName);
                }
            }
        }

        private void setDefaultCSSToolStripMenuItem_Click(object sender, EventArgs e)
        {
            webKitBrowser1.CSSManager.SetPageDefaultStyleSheet();
        }

        private void webKitBrowser1_CanGoBackChanged(object sender, CanGoBackChangedEventArgs e)
        {
            if (sender.Equals(webKitBrowser1))
            button1.Enabled = e.CanGoBack;
        }

        private void webKitBrowser1_CanGoForwardChanged(object sender, CanGoForwardChangedEventArgs e)
        {
            if (sender.Equals(webKitBrowser1))
            button2.Enabled = e.CanGoForward;
        }
        private void customMenuExampleToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void customMenuExampleToolStripMenuItem_CheckStateChanged(object sender, EventArgs e)
        {
            try
            {
                webKitBrowser1.UseDefaultContextMenu = !customMenuExampleToolStripMenuItem.Checked;
            }
            catch { }
        }

        private void webKitBrowser1_PopupCreated(object sender, NewWindowCreatedEventArgs e)
        {
            if (blockPopupsToolStripMenuItem.Checked)
                MessageBox.Show("A popup has been blocked");
            else
            {
                Form f = new Form();
                f.Show();
                WebKitBrowser wb = e.WebKitBrowser;
                wb.AllowDownloads = true;
                wb.Visible = true;
                wb.Name = "browser";
                wb.Dock = DockStyle.Fill;
                wb.DocumentTitleChanged += new EventHandler(wb_DocumentTitleChanged);
                wb.FaviconAvailable += new FaviconAvailable(wb_FaviconAvaiable);
                f.Controls.Add(wb);
            }
        }

        private void urlBar_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                webKitBrowser1.Navigate(urlBar.Text);
        }

        private void test1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void test1ToolStripMenuItem_CheckStateChanged(object sender, EventArgs e)
        {
            richTextBox1.Visible = test1ToolStripMenuItem.Checked;
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            richTextBox1.Text = "";
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void inToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog opn = new OpenFileDialog())
            {
                opn.Filter = "INI Files (*.ini)|*.ini";
                opn.InitialDirectory = Application.StartupPath + @"\LanguageLoader.resources";
                if (opn.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    WebKit.LanguageLoader.SetLanguageFromINIFile(opn.FileName);
                }
            }
        }

        private void richTextBox1_LinkClicked(object sender, LinkClickedEventArgs e)
        {
            webKitBrowser1.Navigate(e.LinkText);
        }

        public WebKitBrowser webKitBrowser1
        {
            get { return tabControl1.SelectedTab.Controls[0] as WebKitBrowser; }
        }

        private void addTabToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddTab("http://www.gt-web-software.webs.com/");
        }

        void AddEvents(WebKitBrowser browser)
        {
            browser.Navigating += new WebKitBrowserNavigatingEventHandler(webKitBrowser1_Navigating);
            browser.DocumentCompleted +=new WebBrowserDocumentCompletedEventHandler(webKitBrowser1_DocumentCompleted);
            browser.CanGoBackChanged +=new CanGoBackChanged(webKitBrowser1_CanGoBackChanged);
            browser.GeolocationPositionRequest += new GeolocationRequest(browser_GeolocationPositionRequest);
            browser.CanGoForwardChanged +=new CanGoForwardChanged(webKitBrowser1_CanGoForwardChanged);
            browser.CloseWindowRequest +=new EventHandler(webKitBrowser1_CloseWindowRequest);
            browser.DangerousSiteDetected +=new EventHandler(webKitBrowser1_DangerousSiteDetected);
            browser.DocumentTitleChanged +=new EventHandler(webKitBrowser1_DocumentTitleChanged);
            browser.FaviconAvailable +=new FaviconAvailable(webKitBrowser1_FaviconAvaiable);
            browser.HeadersAvailable += new HeadersAvailableEventHandler(browser_HeadersAvailable);
            browser.FormSubmit +=new WillSubmitForm(webKitBrowser1_FormSubmit);
            browser.NewWindowCreated +=new NewWindowCreatedEventHandler(webKitBrowser1_NewWindowCreated);
            browser.PopupCreated +=new NewWindowCreatedEventHandler(webKitBrowser1_PopupCreated);
            browser.ProgressChanged +=new WebKit.ProgressChangedEventHandler(webKitBrowser1_ProgressChanged);
            browser.ShowJavaScriptAlertPanel +=new ShowJavaScriptAlertPanelEventHandler(webKitBrowser1_ShowJavaScriptAlertPanel);
            browser.ShowJavaScriptConfirmPanel +=new ShowJavaScriptConfirmPanelEventHandler(webKitBrowser1_ShowJavaScriptConfirmPanel);
            browser.ShowJavaScriptPromptBeforeUnload +=new ShowJavaScriptPromptBeforeUnloadEventHandler(webKitBrowser1_ShowJavaScriptPromptBeforeUnload);
            browser.ShowJavaScriptPromptPanel +=new ShowJavaScriptPromptPanelEventHandler(webKitBrowser1_ShowJavaScriptPromptPanel);
            browser.StatusTextChanged +=new StatusTextChanged(webKitBrowser1_StatusTextChanged);
            browser.CustomContextMenuManager.ShowContextMenu += new ShowContextMenu(CustomContextMenuManager_ShowContextMenu);
            browser.ResourceIntercepter.ResourceSizeAvailableEvent += new ResourceSizeAvailable(ResourceIntercepter_ResourceProgressChangedEvent);
            browser.ResourceIntercepter.ResourceFinishedLoadingEvent += new ResourceFinishedLoadingHandler(ResourceIntercepter_ResourceFinishedLoadingEvent);
            browser.ResourceIntercepter.ResourceStartedLoadingEvent += new ResourceStartedLoadingHandler(ResourceIntercepter_ResourceStartedLoadingEvent);
            browser.ResourceIntercepter.ResourcesSendRequest += new ResourceSendRequestEventHandler(ResourceIntercepter_ResourcesSendRequest);
            browser.ResourceIntercepter.ResourceFailedLoading += new ResourceFailedHandler(ResourceIntercepter_ResourceFailedLoading);
        
        }

        void browser_GeolocationPositionRequest(object sender, GeolocationRequestEventArgs e)
        {
            e.Allow = true;
        }

        void browser_HeadersAvailable(object sender, HeadersAvailableEventArgs e)
        {
            // here you can interfere with headers

            // uncomment the following to see how a message box will show
            // all headers with their fields and values
            
            //string tomes = "";
            //foreach (Header h in e.Headers)
            //{
            //    tomes = tomes + h.Field + ":" + h.Value + "\r\n";
            //}
            //MessageBox.Show(tomes);
        }


        void webKitBrowser1_Navigating(object sender, WebKitBrowserNavigatingEventArgs e)
        {
            // here you can interfere with the WebKitBrowser object before it attempts to load a web page
            // You can cancel the navigation progress and get the Url and TargetFrameName
        }

        void AddTab(string url)
        {
            WebKitBrowser browser = new WebKitBrowser();
            TabPage tab = new TabPage();
            tab.Controls.Add(browser);
            browser.Name = "WebKitBrowser";
            browser.Dock = DockStyle.Fill;
            tabControl1.Controls.Add(tab);
            tabControl1.SelectedTab = tab;
            AddEvents(browser);
            browser.Navigate(url);
        }
        void AddTab(WebKitBrowser browser)
        {
            TabPage tab = new TabPage();
            tab.Controls.Add(browser);
            browser.Name = "WebKitBrowser";
            browser.Dock = DockStyle.Fill;
            tabControl1.Controls.Add(tab);
            tabControl1.SelectedTab = tab;
            AddEvents(browser);
        }

        private void tabControl1_Selected(object sender, TabControlEventArgs e)
        {
            statusLabel.Text = webKitBrowser1.StatusText;
            toolStripProgressBar1.Value = (int)webKitBrowser1.WebView.estimatedProgress() * 100;
            button1.Enabled = webKitBrowser1.CanGoBack;
            button2.Enabled = webKitBrowser1.CanGoForward;
            if (webKitBrowser1.Url != null)
                urlBar.Text = webKitBrowser1.Url.ToString();
            else
                urlBar.Text = "about:blank";
        }

        private void removeTabToolStripMenuItem_Click(object sender, EventArgs e)
        {
            tabControl1.Controls.Remove(tabControl1.SelectedTab);
        }

        private void toolStripMenuItem4_Click(object sender, EventArgs e)
        {
            
        }
    }
}
