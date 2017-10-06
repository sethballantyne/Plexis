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
    public struct TileCoordID
    {
        public ushort x;
        public ushort y;
        public int index;

        public TileCoordID(ushort x, ushort y, int index)
        {
            this.x = x;
            this.y = y;
            this.index = index;
        }
    }

    public partial class MainForm : Form
    {
        bool isPainting = false;
        Graphics bufferBMPGraphicsHandle;
        Bitmap bufferBitmap = new Bitmap(1024, 768);
       // short[,] map = new short[25, 23];
        short selectedBrick = -1;
        bool workSaved = true;
        string filename = null;

        Stack<TileCoordID[]> undoStack = new Stack<TileCoordID[]>();
        List<TileCoordID> paintBuffer = new List<TileCoordID>();

        Stack<TileCoordID[]> redoStack = new Stack<TileCoordID[]>();

        Level level = new Level(25, 23);

        About about = new About();

        public MainForm()
        {
            InitializeComponent();
            listView.ShowItemToolTips = true;

            bufferBMPGraphicsHandle = Graphics.FromImage(bufferBitmap);
            pictureBox.Image = bufferBitmap;
            //for (int i = 0; i < 25; i++)
            //{
            //    for (int j = 0; j < 23; j++)
            //    {
            //        map[i, j] = -1;
            //    }
            //}
        }
        private void ResetState()
        {
            bufferBitmap.Dispose();
            bufferBitmap = new Bitmap(1024, 768);
            bufferBMPGraphicsHandle = Graphics.FromImage(bufferBitmap);
            pictureBox.Image = bufferBitmap;
            this.Text = "Level Editor";
            workSaved = true;
            filename = null;

            level.Clear();
            undoStack.Clear();
            redoStack.Clear();

            // don't allow the saving of blank levels
            saveStripButton.Enabled = saveToolStripMenuItem.Enabled = saveAsToolStripMenuItem.Enabled = 
                undoStripButton.Enabled = undoToolStripMenuItem.Enabled = redoStripButton.Enabled =
                redoToolStripMenuItem.Enabled = false;
            
        }

        private DialogResult PromptToSave()
        {
            return MessageBox.Show("Do you want to save the changes?",
                "Confirmation needed",
                MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
        }

        private void NewLevel()
        {
            if (!workSaved)
            {
                switch (PromptToSave())
                {
                    case DialogResult.Yes:
                        // check to see if the user clicked Cancel on the save file dialog.
                        // if they did, abort creating a new level. 
                        if(SaveLevel() == System.Windows.Forms.DialogResult.Cancel)
                            return;
                        break;
                    case DialogResult.Cancel:
                        return;
                }
            }

            ResetState();
            pictureBox.Refresh();
        }

        private void OpenLevel()
        {
            if (!workSaved)
            {
                // work hasn't been saved; prompt to see if the user wishes to save
                // before opening a level.
                switch (PromptToSave())
                {
                    case DialogResult.Yes:
                        if (saveFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                        {
                            level.Save(saveFileDialog.FileName);
                            Text = "Map Editor - " + filename;
                        }
                        else
                        {
                            // user has clicked Cancel on the save file dialog
                            return;
                        }
                        break;
                    case DialogResult.Cancel:
                        return;
                }
            }

            if (openFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                ResetState();
                level.Read(openFileDialog.FileName);

                for (uint i = 0; i < 25; i++)
                {
                    for (uint j = 0; j < 23; j++)
                    {
                        int value = level.GetElement((ushort)i, (ushort)j);
                        PaintBrick(i * 41, j * 20, value, false);
                    }
                }

                pictureBox.Refresh();

                this.Text = "Level Editor - " + openFileDialog.FileName;
                filename = openFileDialog.FileName;

                saveToolStripMenuItem.Enabled = saveAsToolStripMenuItem.Enabled =
                    saveStripButton.Enabled = true;

            }
        }

        private DialogResult SaveLevel()
        {
            DialogResult dr = System.Windows.Forms.DialogResult.Cancel;

            if (filename == null)
            {
                dr = saveFileDialog.ShowDialog();
                if (dr == System.Windows.Forms.DialogResult.OK)
                {
                    if (filename != saveFileDialog.FileName)
                    {
                        filename = saveFileDialog.FileName;
                    }
                }
                else
                {
                    return dr;
                }
            }

            level.Save(filename);
            workSaved = true;

            Text = "Map Editor - " + filename;

            return dr;
        }

        private void Undo()
        {
            TileCoordID[] bricks = undoStack.Pop();
            List<TileCoordID> temp = new List<TileCoordID>();

            foreach (TileCoordID brick in bricks)
            {
                ushort tileCoordX = (ushort)(brick.x / 41);
                ushort tileCoordY = (ushort)(brick.y / 20);

                // take each brick we're about the paint over and stick them in a buffer 
                // which will be passed to the redo stack once painting is finished. 
                //short brickIndex = map[tileCoordX, tileCoordY];
                int brickIndex = level.GetElement(tileCoordX, tileCoordY);
                temp.Add(new TileCoordID(brick.x, brick.y, brickIndex));

                // brick.x and brick.y contain screen coordinates. If you don't
                // convert to tile coordinates, you'll get a buffer overflow (and they'll be wrong regardless).
                //map[tileCoordX, tileCoordY] = brick.index;
                level.SetElement(tileCoordX, tileCoordY, brick.index);
                // disabling the refresh because otherwise each brick is drawn and then displayed.
                // It results in a domino-like effect; we want to see all the bricks appear at once. 
                PaintBrick((ushort)brick.x, (ushort)brick.y, brick.index, false);
            }

            redoStack.Push(temp.ToArray());
            pictureBox.Refresh();

            if (undoStack.Count == 0)
            {
                undoToolStripMenuItem.Enabled = undoStripButton.Enabled = false;
            }

            redoToolStripMenuItem.Enabled = redoStripButton.Enabled = true;
        }

        private void Redo()
        {
            TileCoordID[] bricks = redoStack.Pop();
            List<TileCoordID> temp = new List<TileCoordID>();

            foreach (TileCoordID brick in bricks)
            {
                ushort tileCoordX = (ushort)(brick.x / 41);
                ushort tileCoordY = (ushort)(brick.y / 20);

                // take each brick we're about the paint over and stick them in a buffer 
                // which will be passed to the redo stack once painting is finished. 
                //short brickIndex = map[tileCoordX, tileCoordY];
                int brickIndex = level.GetElement(tileCoordX, tileCoordY);
                temp.Add(new TileCoordID(brick.x, brick.y, brickIndex));

                // brick.x and brick.y contain screen coordinates. If you don't
                // convert to tile coordinates, you'll get a buffer overflow (and they'll be wrong regardless).
                //map[tileCoordX, tileCoordY] = brick.index;
                level.SetElement(tileCoordX, tileCoordY, brick.index);

                // disabling the refresh because otherwise each brick is drawn and then displayed.
                // It results in a domino-like effect; we want to see all the bricks appear at once. 
                PaintBrick((ushort)brick.x, (ushort)brick.y, brick.index, false);
            }

            undoStack.Push(temp.ToArray());
            pictureBox.Refresh();

            if (redoStack.Count == 0)
            {
                redoToolStripMenuItem.Enabled = redoStripButton.Enabled = false;
            }

            undoToolStripMenuItem.Enabled = undoStripButton.Enabled = true;
        }

        void PaintBrick(uint x, uint y, int index, bool refresh)
        {
            if (index == -1)
            {
                using (SolidBrush brush = new SolidBrush(Color.Black))
                {
                    //pictureBoxGraphicsHandle.DrawRectangle(p, x, y, 41, 20);
                    bufferBMPGraphicsHandle.FillRectangle(brush, x, y, 41, 20);
                }
            }
            else
            {
                bufferBMPGraphicsHandle.DrawImage(imageList.Images[index], x, y);
            }

            if (refresh)
            {
                pictureBox.Refresh();
            }
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

                if (tileCoordX < 25 && tileCoordY < 23)
                {
                    //if (map[tileCoordX, tileCoordY] != selectedBrick)
                    if(level.GetElement(tileCoordX, tileCoordY) != selectedBrick)
                    {
                        redoStack.Clear();
                        redoStripButton.Enabled = redoToolStripMenuItem.Enabled = false;

                        ushort screenX = (ushort)(tileCoordX * 41);
                        ushort screenY = (ushort)(tileCoordY * 20);

                        //short oldBrick = map[tileCoordX, tileCoordY];
                        int oldBrick = level.GetElement(tileCoordX, tileCoordY);
                        paintBuffer.Add(new TileCoordID(screenX, screenY, oldBrick));
                    
                        //map[tileCoordX, tileCoordY] = selectedBrick;
                        level.SetElement(tileCoordX, tileCoordY, selectedBrick);
                        PaintBrick(screenX, screenY, selectedBrick, true);

                        if (workSaved)
                        {
                            workSaved = false;
                            this.Text += "*";
                        }
                    }
                }
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
                    //if (map[tileCoordX, tileCoordY] != selectedBrick)
                    if(level.GetElement(tileCoordX, tileCoordY) != selectedBrick)
                    {
                        ushort screenX = (ushort)(tileCoordX * 41);
                        ushort screenY = (ushort)(tileCoordY * 20);

                        //short oldBrick = map[tileCoordX, tileCoordY];
                        int oldBrick = level.GetElement(tileCoordX, tileCoordY);
                        //paintBuffer.AddCoordinate(mouseX, mouseY);
                        paintBuffer.Add(new TileCoordID(screenX, screenY, oldBrick));
                        //map[tileCoordX, tileCoordY] = selectedBrick;
                        level.SetElement(tileCoordX, tileCoordY, selectedBrick);
                        PaintBrick(screenX, screenY, selectedBrick, true);
                    }
                }
            }
        }

        private void pictureBox_MouseUp(object sender, MouseEventArgs e)
        {
            isPainting = false;
            if (paintBuffer.Count > 0)
            {
                undoStack.Push(paintBuffer.ToArray());
                paintBuffer.Clear();
                undoToolStripMenuItem.Enabled = undoStripButton.Enabled = true;
            }

            saveAsToolStripMenuItem.Enabled = saveToolStripMenuItem.Enabled = saveStripButton.Enabled = true;
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Undo();
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Redo();
        }

        private void redoStripButton_Click(object sender, EventArgs e)
        {
            Redo();
        }

        private void undoStripButton_Click(object sender, EventArgs e)
        {
            Undo();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveLevel();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (saveFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if (filename != saveFileDialog.FileName)
                {
                    filename = saveFileDialog.FileName;
                }
                
                level.Save(filename);
                workSaved = true;

                Text = "Map Editor - " + filename;
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenLevel();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!workSaved)
            {
                DialogResult dr = MessageBox.Show("Do you want to save the changes?", 
                    "Confirmation needed",
                    MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);

                if (dr == System.Windows.Forms.DialogResult.Yes)
                {
                    // save-as code.
                    if (saveFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                    {
                        if (filename != saveFileDialog.FileName)
                        {
                            filename = saveFileDialog.FileName;
                        }

                        level.Save(filename);
                        workSaved = true;

                        Text = "Map Editor - " + filename;
                    }
                    else
                    {
                        // user has pressed cancel on the save file dialog.
                        e.Cancel = true;
                    }
                }
                else if (dr == System.Windows.Forms.DialogResult.Cancel)
                {
                    e.Cancel = true;
                }
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewLevel();
        }

        private void newStripButton_Click(object sender, EventArgs e)
        {
            NewLevel();
        }

        private void openStripButton_Click(object sender, EventArgs e)
        {
            OpenLevel();
        }

        private void saveStripButton_Click(object sender, EventArgs e)
        {
            SaveLevel();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            about.Show();
        }
    }
}
