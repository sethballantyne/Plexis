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
        LevelOrder levelOrder = new LevelOrder();

        public MainForm()
        {
            InitializeComponent();

            SetGUIState(GUIState.Default);
        }

        /// <summary>
        /// Displays an error message to the user based on an exception thrown by EditorLogic.
        /// </summary>
        /// <param name="message">the exceptions Message property.</param>
        private void DisplayErrorMessage(string message)
        {
            string errorMessage = String.Format("Exception caught with message: {0}\n\n", message);
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

        /// <summary>
        /// Enables or disables controls based on the state the Editor is being put in.
        /// </summary>
        /// <param name="guiState">The state of the editor</param>
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
                e.Cancel = EditorLogic.InitiateShutdownSequence();
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
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
            
        }

        private void saveLevelToolStripButton_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.SaveLevel();
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.SaveAs();
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.Undo();
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.Redo();
            }
            catch (Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        /// <summary>
        /// enables or disables the undo menu item and toolstrip button.
        /// </summary>
        /// <param name="enabled"><b>true</b> to enable the GUI items, <b>false</b> to disable.</param>
        internal void SetUndo(bool enabled)
        {
            undoToolStripButton.Enabled = undoToolStripMenuItem.Enabled = enabled;
        }

        /// <summary>
        /// enables or disables the redo menu item and toolstrip button.
        /// </summary>
        /// <param name="enabled"><b>true</b> to enable the GUI items, <b>false</b> to disable.</param>
        internal void SetRedo(bool enabled)
        {
            redoToolStripButton.Enabled = redoToolStripMenuItem.Enabled = enabled;
        }

        private void toggleGridToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.ToggleGrid();
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void toggleGridToolStripButton_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.ToggleGrid();
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void undoToolStripButton_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.Undo();
            }
            catch (Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void redoToolStripButton_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.Redo();
            }
            catch (Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void pictureBox_MouseDown(object sender, MouseEventArgs e)
        {
            try
            {
                EditorLogic.BeginPainting(e.Location);
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        /// <summary>
        /// Checks or unchecks the grid menu item and toolstrip button.
        /// </summary>
        /// <param name="enabled"><b>true</b> if the items should be checked, otherwise <b>false</b>.</param>
        internal void CheckGridControls(bool enabled)
        {
            toggleGridToolStripButton.Checked = toggleGridToolStripMenuItem.Checked = enabled;
        }

        private void gridColourToolStripMenuItem_Click(object sender, EventArgs e)
        {
            EditorLogic.ShowColourDialog();
        }

        private void brickListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.ListViewSelectionChanged();
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void pictureBox_MouseMove(object sender, MouseEventArgs e)
        {
            try
            {
                EditorLogic.Painting(e.Location);
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void pictureBox_MouseUp(object sender, MouseEventArgs e)
        {
            try
            {
                EditorLogic.EndPainting();
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        /// <summary>
        /// Enables or disables the save menu items and toolstrip button.
        /// </summary>
        /// <param name="enabled"><b>true</b> if the GUI items should be enabled, otherwise <b>false</b>.</param>
        internal void SetSaveItems(bool enabled)
        {
            saveAsToolStripMenuItem.Enabled = saveToolStripMenuItem.Enabled = saveLevelToolStripButton.Enabled = enabled;
        }

        private void eraserToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.EditToolSelected(EditMode.Eraser);
            }
            catch (Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void eraserToolStripButton_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.EditToolSelected(EditMode.Eraser);
            }
            catch(Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        /// <summary>
        /// Unselects all bricks in the list view.
        /// </summary>
        /// <remarks>
        /// This is a good example of why encapsulation can be a load of shit.
        /// </remarks>
        internal void ClearListViewSelection()
        {
            brickListView.SelectedItems.Clear();
        }

        /// <summary>
        /// Checks or unchecks the eraser toolstrip button and menu item.
        /// </summary>
        /// <param name="isChecked"><b>true</b> if the gui items should be put into a checked state, otherwise <b>false</b>.</param>
        internal void CheckEraserControls(bool isChecked)
        {
            eraserToolStripButton.Checked = eraserToolStripMenuItem.Checked = isChecked;
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ((new AboutBox())).Show();
        }

        private void previewLevelToolStripButton_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.PreviewLevel();
            }
            catch (Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void previewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                EditorLogic.PreviewLevel();
            }
            catch (Exception ex)
            {
                DisplayErrorMessage(ex.Message);
            }
        }

        private void chaneLevelOrderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                string[] test = { "1", "2", "3", "4" };
                EditorLogic.ShowLevelOrderDialog();
            }
            catch
            {
                throw;
            }
            //levelOrder.Show(test, "test");
        }
    }
}
