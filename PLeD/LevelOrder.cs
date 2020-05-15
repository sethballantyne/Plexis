// Copyright(c) 2018 Seth Ballantyne <seth.ballantyne@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

namespace PLeD
{
    public partial class LevelOrder : Form
    {
        string filename;
        string levelsPath;
        string[] rotationLevels;

        public LevelOrder()
        {
            InitializeComponent();
        }

        private void listbox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(rotationListListBox.SelectedIndex >= 0)
            {
                rotationListDeleteButton.Enabled = rotationListMoveDownButton.Enabled = rotationListMoveUpButton.Enabled = true;
            }
            else
            {
                rotationListDeleteButton.Enabled = rotationListMoveDownButton.Enabled = rotationListMoveUpButton.Enabled = false;
            }
        }

        private void LevelOrder_FormClosing(object sender, FormClosingEventArgs e)
        {
            okayButton.Enabled = false;
        }
        
        /// <summary>
        /// Gets or sets the levels that populate the rotation list.
        /// </summary>
        public string[] RotationLevels
        {
            get 
            {
                List<string> levels = new List<string>();
                foreach (string s in rotationListListBox.Items)
                {
                    levels.Add(s);
                }

                return levels.ToArray();
            }
            set
            {
                rotationLevels = value; 
            }
        }

        private void moveUpButton_Click(object sender, EventArgs e)
        {
            if(rotationListListBox.SelectedIndex > 0)
            {
                Swap(rotationListListBox.SelectedIndex, rotationListListBox.SelectedIndex - 1);
                okayButton.Enabled = true;
            }
        }

        private void moveDownButton_Click(object sender, EventArgs e)
        {
            if (rotationListListBox.SelectedIndex != rotationListListBox.Items.Count - 1)
            {
                Swap(rotationListListBox.SelectedIndex, rotationListListBox.SelectedIndex + 1);
                okayButton.Enabled = true;
            }
        }

        void Swap(int selectedItem, int destination)
        {
            string item = rotationListListBox.Items[selectedItem] as string;
            string item2 = rotationListListBox.Items[destination] as string;

            rotationListListBox.Items[selectedItem] = item2;
            rotationListListBox.Items[destination] = item;

            if(selectedItem < destination)
            {
                rotationListListBox.SelectedIndex++;
            }
            else
            {
                rotationListListBox.SelectedIndex--;
            }
        }

        public string FileName
        {
            get { return filename; }
            set { filename = value; }
        }

        public string LevelsPath
        {
            get { return LevelsPath; }
            set { levelsPath = value; }
        }

        private void okayButton_VisibleChanged(object sender, EventArgs e)
        {
            if(Visible)
            {
                if (rotationListListBox.Items.Count > 0 && filename != null)
                {
                    for (int i = 0; i < rotationListListBox.Items.Count; i++)
                    {
                        string item = rotationListListBox.Items[i] as string;
                        if (item == filename)
                        {
                            rotationListListBox.SelectedIndex = i;
                        }
                    }
                }
            }
        }

        private void deleteButton_Click(object sender, EventArgs e)
        {
            int index = rotationListListBox.SelectedIndex;

            if(index >= 0)
            {
                availableLevelsListBox.Items.Add(rotationListListBox.Items[index]);
                rotationListListBox.Items.RemoveAt(index);
                okayButton.Enabled = true;
            }
        }

        private string TrimExtension(string file)
        {
            StringBuilder sb = new StringBuilder();

            for(int i = 0; i < file.Length && file[i] != '.'; i++)
            {
                sb.Append(file[i]);
            }

            return sb.ToString();
        }

        private string[] EnumerateLevels(string path)
        {
            string levelsPath = AppDomain.CurrentDomain.BaseDirectory + path;
            List<string> levels = new List<string>(Directory.GetFiles(levelsPath));
        
            for(int i = levels.Count - 1; i >= 0; i--)
            {
                FileInfo fi = new FileInfo(levels[i]);
                
                if(fi.Extension.ToLower() == ".xml")
                {
                    levels[i] = TrimExtension(fi.Name);
                }
                else
                {
                    levels.RemoveAt(i);
                }
            }

            return levels.ToArray();
        }

        private void LevelOrder_Shown(object sender, EventArgs e)
        {
            string[] allLevels = EnumerateLevels(levelsPath);

            rotationListListBox.Items.Clear();
            availableLevelsListBox.Items.Clear();

            // filter levels out of allLevels if they're in the rotation list
            for(int i = 0; i < allLevels.Length; i++)
            {
                for(int j = 0; j < rotationLevels.Length; j++)
                {
                    if(allLevels[i] == rotationLevels[j])
                    {
                        allLevels[i] = null;
                    }
                }
            }

            PopulateListBox(rotationListListBox, rotationLevels);
            PopulateListBox(availableLevelsListBox, allLevels);
        }

        private void PopulateListBox(ListBox listBox, string[] levels)
        {
            foreach (string s in levels)
            {
                if(s != null)
                {
                    listBox.Items.Add(s);
                }
            }
        }

        private void availableLevelsListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (availableLevelsListBox.SelectedIndex >= 0)
            {
                availableLevelsAddButton.Enabled = true;
            }
            else
            {
                availableLevelsAddButton.Enabled = false;
            }
        }

        private void availableLevelsAddButton_Click(object sender, EventArgs e)
        {
            int index = availableLevelsListBox.SelectedIndex;

            if (index >= 0)
            {
                rotationListListBox.Items.Add(availableLevelsListBox.Items[index]);
                availableLevelsListBox.Items.RemoveAt(index);
                okayButton.Enabled = true;
            }
        }
    }
}
