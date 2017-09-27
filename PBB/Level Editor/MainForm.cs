using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Level_Editor
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
            listView.ShowItemToolTips = true;
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            BrickData[] parsedXMLData = XMLParser.Load("assets.xml");

            foreach (BrickData brick in parsedXMLData)
            {
                Image brickImage = Image.FromFile(brick.Filename);
                imageList.Images.Add(brick.ID, brickImage);

                ListViewItem item = new ListViewItem(brick.Filename, brick.ID);
                StringBuilder stringBuilder = new StringBuilder();
                foreach(string s in brick.GetAttributes())
                {
                    stringBuilder.AppendLine(s);
                }
                item.ToolTipText = String.Format("{0}\n{1}", stringBuilder.ToString(), brick.Description);
                listView.Items.Add(item);
                
            }
        }
    }
}
