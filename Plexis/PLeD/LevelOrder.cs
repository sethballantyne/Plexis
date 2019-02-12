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

namespace PLeD
{
    public partial class LevelOrder : Form
    {
        string filename;

        public LevelOrder()
        {
            InitializeComponent();
        }

        private void listbox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(listbox.SelectedIndex >= 0)
            {
                deleteButton.Enabled = moveDownButton.Enabled = moveUpButton.Enabled = true;
            }
            else
            {
                deleteButton.Enabled = moveDownButton.Enabled = moveUpButton.Enabled = false;
            }
        }

        private void LevelOrder_FormClosing(object sender, FormClosingEventArgs e)
        {
            okayButton.Enabled = false;
        }
        
        /// <summary>
        /// Gets the levels 
        /// </summary>
        public string[] Levels
        {
            get 
            {
                List<string> levels = new List<string>();
                foreach(string s in listbox.Items)
                {
                    levels.Add(s);
                }

                return levels.ToArray();
            }
            set
            {
                listbox.Items.Clear();
                foreach (string s in value)
                {
                    listbox.Items.Add(s);
                }

            }
        }

        private void moveUpButton_Click(object sender, EventArgs e)
        {
            if(listbox.SelectedIndex > 0)
            {
                Swap(listbox.SelectedIndex, listbox.SelectedIndex - 1);
                okayButton.Enabled = true;
            }
        }

        private void moveDownButton_Click(object sender, EventArgs e)
        {
            if (listbox.SelectedIndex != listbox.Items.Count - 1)
            {
                Swap(listbox.SelectedIndex, listbox.SelectedIndex + 1);
                okayButton.Enabled = true;
            }
        }

        void Swap(int selectedItem, int destination)
        {
            string item = listbox.Items[selectedItem] as string;
            string item2 = listbox.Items[destination] as string;

            listbox.Items[selectedItem] = item2;
            listbox.Items[destination] = item;

            if(selectedItem < destination)
            {
                listbox.SelectedIndex++;
            }
            else
            {
                listbox.SelectedIndex--;
            }
        }

        public string FileName
        {
            get { return filename; }
            set { filename = value; }
        }

        private void okayButton_VisibleChanged(object sender, EventArgs e)
        {
            if(Visible)
            {
                if (listbox.Items.Count > 0 && filename != null)
                {
                    for (int i = 0; i < listbox.Items.Count; i++)
                    {
                        string item = listbox.Items[i] as string;
                        if (item == filename)
                        {
                            listbox.SelectedIndex = i;
                        }
                    }
                }
            }
        }

        private void deleteButton_Click(object sender, EventArgs e)
        {
            if(listbox.SelectedIndex >= 0)
            {
                DialogResult dr = MessageBox.Show("Are you sure you want to delete this level from the list? Note that this only removes the level from the list, it doesn't delete the file from your hard disk.",
                    "Conformation Needed", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if(dr == DialogResult.Yes)
                {
                    listbox.Items.RemoveAt(listbox.SelectedIndex);
                    okayButton.Enabled = true;
                }
            }
        }
    }
}
