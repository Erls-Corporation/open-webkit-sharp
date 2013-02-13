using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WebKit;

namespace CustomMenuImplementation
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            webKitBrowser1.Navigate(urlBar.Text);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // after we have set Browser.UseDefaultContextMenu then we
            // will use the ShowContextMenu event from the CustomContextMenuManager
            // object. Then, according to the suggested menu type we will show
            // our custom menu.
            webKitBrowser1.CustomContextMenuManager.ShowContextMenu += new WebKit.ShowContextMenu(CustomContextMenuManager_ShowContextMenu);
        }

        void CustomContextMenuManager_ShowContextMenu(object sender, WebKit.ShowContextMenuEventArgs e)
        {
            // Here we handle our custom menu implementation.
            // Remember that firstly we have set Browser.UseDefaultContextMenu = false
            // Otherwise this event will not be fired and the default webkit context
            // menu will be shown.
            ContextMenu Menu;
            if (e.MenuType == WebKit.ContextMenuType.Image)
                Menu = imageContextMenu;
            else if (e.MenuType == WebKit.ContextMenuType.ImageAndLink)
                Menu = imageAndLinkContextMenu;
            else if (e.MenuType == WebKit.ContextMenuType.Link)
                Menu = linkContextMenu;
            else if (e.MenuType == WebKit.ContextMenuType.Input)
                Menu = inputContextMenu;
            else if (e.MenuType == WebKit.ContextMenuType.Form)
                Menu = formContextMenu;
            else Menu = normalContextMenu;
            Menu.Show((sender as WebKitBrowser), e.Location);
        }

        private void webKitBrowser1_Navigating(object sender, WebKitBrowserNavigatingEventArgs e)
        {
            urlBar.Text = e.Url.ToString();
        }
    }
}
