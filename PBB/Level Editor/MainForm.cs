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
        bool isPainting = false;
        Graphics pictureBoxGraphicsHandle;
        Bitmap bufferBitmap = new Bitmap(1024, 768);
        short[,] map = new short[25, 23];
        short selectedBrick = -1;

        public MainForm()
        {
            InitializeComponent();
            listView.ShowItemToolTips = true;

            pictureBoxGraphicsHandle = Graphics.FromImage(bufferBitmap);
            pictureBox.Image = bufferBitmap;
            for (int i = 0; i < 25; i++)
            {
                for (int j = 0; j < 23; j++)
                {
                    map[i, j] = -1;
                }
            }
        }

        void PaintBrick(ushort x, ushort y, short index)
        {
            pictureBoxGraphicsHandle.DrawImage(imageList.Images[index], x, y);
            pictureBox.Refresh();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            BrickData[] parsedXMLData = XMLParser.Load("assets.xml");

            foreach (BrickData brick in parsedXMLData)
            {
                Image brickImage = Image.FromFile(brick.Filename);
                imageList.Images.Add(brick.ID, brickImage);

                ListViewItem item = new ListViewItem(brick.Label, brick.ID);
                StringBuilder stringBuilder = new StringBuilder();
                foreach(string s in brick.GetAttributes())
                {
                    stringBuilder.AppendLine(s);
                }

                item.ToolTipText = String.Format("{0}\n{1}", stringBuilder.ToString(), brick.Description);
                listView.Items.Add(item);
                
            }
        }

        private void pictureBox_MouseDown(object sender, MouseEventArgs e)
        {
            if (listView.SelectedItems.Count == 1)
            {
                isPainting = true;
                ushort tileCoordX = (ushort)(e.X / 41);
                ushort tileCoordY = (ushort)(e.Y / 20);

                ushort mouseX = (ushort)(tileCoordX * 41);
                ushort mouseY = (ushort)(tileCoordY * 20);

                map[tileCoordX, tileCoordY] = selectedBrick;

                PaintBrick(mouseX, mouseY, selectedBrick);
            }
        }

        private void listView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listView.SelectedItems.Count > 0)
            {
                selectedBrick = (short)listView.Items.IndexOf(listView.SelectedItems[0]);
            }
        }

        private void pictureBox_MouseMove(object sender, MouseEventArgs e)
        {
            if(isPainting)
            {
                ushort tileCoordX = (ushort)(e.X / 41);
                ushort tileCoordY = (ushort)(e.Y / 20);

                if (tileCoordX < 25 && tileCoordY < 23)
                {
                    if (map[tileCoordX, tileCoordY] != selectedBrick)
                    {
                        ushort mouseX = (ushort)(tileCoordX * 41);
                        ushort mouseY = (ushort)(tileCoordY * 20);

                        map[tileCoordX, tileCoordY] = selectedBrick;

                        PaintBrick(mouseX, mouseY, selectedBrick);
                    }
                }
            }
        }

        private void pictureBox_MouseUp(object sender, MouseEventArgs e)
        {
            isPainting = false;
        }
    }
}
