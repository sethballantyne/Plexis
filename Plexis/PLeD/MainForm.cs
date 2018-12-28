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
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PLeD
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();

            SetGUIState(GUIState.Default);
        }

        private void DisplayErrorMessage(string message)
        {
            string errorMessage = String.Format("An error occured while initialising the editor: {0}\n\nAdios!", message);
                MessageBox.Show(errorMessage, "ERRAWR!", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.Initialise(this, pictureBox, brickImageList, brickListView);
            }
            catch (Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        internal void SetGUIState(GUIState guiState)
        {
            switch(guiState)
            {
                case GUIState.Default:

                    // hide the brush panel and level renderer.
                    splitContainer.Visible = false;

                    //-----------------------------------------
				    // FILE MENU
				    //-----------------------------------------
				    saveLevelToolStripButton.Enabled = false;
				    saveToolStripMenuItem.Enabled = false;
				    saveAsToolStripMenuItem.Enabled = false;

				    previewLevelToolStripButton.Enabled = false;
				    previewToolStripMenuItem.Enabled = false;

				    //------------------------------------------
				    // EDIT MENU
				    //------------------------------------------
				    undoToolStripMenuItem.Enabled = false;
				    undoToolStripButton.Enabled = false;
				    redoToolStripMenuItem.Enabled = false;
				    redoToolStripButton.Enabled = false;

				    //-------------------------------------------
				    // VIEW MENU
				    //-------------------------------------------
				    toggleGridToolStripButton.Enabled = false;
				    toggleGridToolStripMenuItem.Enabled = false;

				    //-------------------------------------------
				    // TOOLS MENU
				    //-------------------------------------------
				    eraserToolStripMenuItem.Enabled = false;
				    eraserToolStripButton.Enabled = false;

				    brushToolStripMenuItem.Enabled = false;
                    brushToolStripButton.Enabled = false;
                    break;

                case GUIState.Editing:
                    splitContainer.Visible = true;

				    //-----------------------------------------
				    // FILE MENU
				    //---------------------------------------
				    saveLevelToolStripButton.Enabled = true;
				    saveToolStripMenuItem.Enabled = true;
				    saveAsToolStripMenuItem.Enabled = true;

				    previewLevelToolStripButton.Enabled = true;
				    previewToolStripMenuItem.Enabled = true;

				    //-------------------------------------------
				    // VIEW MENU
				    //-------------------------------------------
				    toggleGridToolStripButton.Enabled = true;
				    toggleGridToolStripMenuItem.Enabled = true;

				    //-------------------------------------------
				    // TOOLS MENU
				    //-------------------------------------------
				    eraserToolStripMenuItem.Enabled = true;
				    eraserToolStripButton.Enabled = true;

				    brushToolStripMenuItem.Enabled = true;
				    brushToolStripButton.Enabled = true;
                    break;
                default:
                    break;
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.NewLevel();
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                EditorLogic.CleanUp();
            }
            catch(Exception ex)
            {
                // more for debug purposes in this instance.
                DisplayErrorMessage(ex.Message);
            }
        }

        private void newLevelToolStripButton_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.NewLevel();
            }
            catch (Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.OpenLevel();
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void openLevelToolStripButton_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.OpenLevel();
            }
            catch (Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.SaveLevel();
            }
            catch
            {
                throw;
            }
            
        }

        private void saveLevelToolStripButton_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.SaveLevel();
            }
            catch
            {
                throw;
            }
        }
    }
}
