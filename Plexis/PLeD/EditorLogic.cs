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
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Reflection;
using System.Xml;
using System.Diagnostics;
using System.Drawing;

namespace PLeD
{
    /// <summary>
    /// Handles all the logic behind the GUI.
    /// </summary>
    static class EditorLogic
    {
        #region private
        // all the bricks loaded from entities.xml.
        static Brick[] bricks;
        static Paths paths;

        static Grid editorGrid = new Grid();
        static Level currentLevel;

        static MainForm mainForm;
        static ListView listView;
        static Control renderControl;
        static ImageList imageList;

        static SaveFileDialog saveFileDialog = new SaveFileDialog();
        static OpenFileDialog openFileDialog = new OpenFileDialog();
        static ColorDialog colourDialog = new ColorDialog();

        static Bitmap bufferBitmap;
        static Graphics bufferBitmapGraphics;

        static SolidBrush blackBrush = new SolidBrush(Color.Black);

        static Stack<Point> undoStack = new Stack<Point>();
        static Stack<Point> redoStack = new Stack<Point>();

        static string gameDirectory;
        static string currentLevelFilename;

        static bool workSaved = true;

        /// <summary>
        /// Loads and adds the brick images to the ListView so we can bricks.
        /// </summary>
        /// <remarks>This function operates from the assumption that the configuration information
        ///  has already been read in.</remarks>
        /// <exception cref="System.UnauthorizedException">The program doesn't have the required permissions to access a directory contained within <i>path</i>.</exception>
        /// <exception cref="System.IO.DirectoryNotFoundException">A directory containd within <i>path</i> couldn't be found.</exception>
        private static void AddBricksToListView()
        {
            // This function operates from the assumption that the configuration information
            // has already been read in. 

            Debug.Assert(EditorLogic.imageList != null);
            Debug.Assert(EditorLogic.listView != null);

            try
            {
                for (int i = 0; i < bricks.Length; i++)
                {
                    foreach (Path path in paths.BitmapPaths)
                    {
                        string location = FindBrick(path, bricks[i].ImageName);
                        if (location != null)
                        {
                            imageList.Images.Add(bricks[i].Name, new Bitmap(location));

                            // first arg: item text, second: the images key in the list view.
                            ListViewItem newItem = new ListViewItem(bricks[i].Name, bricks[i].Name);
                            listView.Items.Add(newItem);
                        }
                    }
                }
            }
            catch
            {
                throw;
            }
           
        }

        /// <summary>
        /// Searches the specified paths for a specific image.
        /// </summary>
        /// <param name="path">Path object that specifies the directories containing Bitmap images used by the game.</param>
        /// <param name="brickFileName">the filename to search for, not including the .bmp extension.</param>
        /// <returns>the absolute path to the brick image, otherwise <b>null</b>.</returns>
        /// <exception cref="System.UnauthorizedException">The program doesn't have the required permissions to access a directory contained within <i>path</i>.</exception>
        /// <exception cref="System.IO.DirectoryNotFoundException">A directory containd within <i>path</i> couldn't be found.</exception>
        static string FindBrick(Path path, string brickFileName)
        {
            Debug.Assert(brickFileName != null && brickFileName != String.Empty);

            string searchPattern = String.Format("{0}.bmp", brickFileName);
            string searchPath = String.Format("{0}{1}", gameDirectory, path.ResourcePath);
            string[] files;

            if(path.IncludeSubDirectories)
            {
                files = Directory.GetFiles(searchPath, searchPattern, SearchOption.AllDirectories);
            }
            else
            {
                files = Directory.GetFiles(searchPath, searchPattern, SearchOption.AllDirectories);
            }

            if(files == null || files.Length == 0)
            {
                return null;
            }

            return files[0];
        }

        /// <summary>
        /// Loads the required information from paths.xml and entities.xml.
        /// </summary>
        /// <exception cref="System.FileNotFoundException">one of the required XML files couldn't be found.</exception>
        /// <exception cref="System.UnauthorizedAccessException">The editor doesn't have the correct permissions to access a directory containing bitmaps.</exception>
        /// <exception cref="System.IO.DirectoryNotFound">One of the directories containing bitmaps.</exception>
        /// <exception cref="System.XmlException">A required XML file is missing data or is incorrectly formatted.</exception>
        /// <exception cref="System.FormatException">A required XML attribute was supposed to contain a numeric value but doesn't.</exception>
        static void LoadXMLData()
        {
            // grab the absolute path of the editor and trim the leading "file://" from the
            // the result. 
            string gamePath = Assembly.GetExecutingAssembly().CodeBase;
            gamePath = new Uri(gamePath).LocalPath;

            // grab the path minus the editors filename.
            gameDirectory = System.IO.Path.GetDirectoryName(gamePath);

            try
            {
                //-------------------------------------------------------------------
		        // First step: attempt load paths.xml.
		        // this file contains the locations of the resources used by the game.
                //--------------------------------------------------------------------
                paths = XML.LoadPaths("paths.xml");

                if(paths.XMLPaths.Length == 0)
                {
                    throw new XmlException("paths.xml doesn't contain any of the required information, so I can't load any resources.");
                }

                // enumerate all the locations we'll be searching for bitmaps.
                // it's possible to set an attribute in the XML we've just parsed that specifies that the
                // provided path and all its subdirectories should be included when
                // searching for resources, so we have to factor this in.
                List<string> directoryList = new List<string>();

                foreach(Path directory in paths.BitmapPaths)
                {
                    // NOTE: it's possible to add multiple entries of the same directory, fix if there's time.
                    directoryList.Add(directory.ResourcePath);

                    if(directory.IncludeSubDirectories)
                    {
                        string tempPath = String.Format("{0}/{1}", gameDirectory, directory.ResourcePath);
                        string[] directories = Directory.GetDirectories(tempPath);

                        if(directories != null)
                        {
                            directoryList.AddRange(directories);
                        }
                    }
                }

                foreach(Path path in paths.XMLPaths)
                {
                    try
                    {
                        string tempPath = String.Format("{0}{1}{2}", gameDirectory, path.ResourcePath, "/entities.xml");
                        bricks = XML.ReadBrickData(tempPath);
                        break;
                    }
                    catch(FileNotFoundException)
                    {
                        // looking in the wrong place, continue on to the next directory. 
                    }
                    catch
                    {
                        throw;
                    }
                }

                if(bricks == null)
                {
                    throw new FileNotFoundException("Unable to locate entities.xml.");
                }

                editorGrid.Colour = Properties.Settings.Default.GridColour;
                editorGrid.Visible = Properties.Settings.Default.GridIsVisible;
            }
            catch
            {
                throw;
            }
            
        }

        /// <summary>
        /// Paints a brick at the specified coordinates
        /// </summary>
        /// <param name="x">the bricks x coordinate on the control being used to render the level.</param>
        /// <param name="y">the bricks y coordinate on the control being used to render the level.</param>
        /// <param name="index">the index of the desired brick in the image list. Use -1 if painting a blank
        /// space (removing a brick).</param>
        /// <param name="refresh"><b>true</b> if the control being used to render the level
        /// should be refreshed as soon as the brick is drawn, otherwise <b>false</b>.</param>
        /// <remarks>Coordinates are screen coordinates, not tile coordinates.</remarks>
        private static void PaintBrick(int x, int y, int index, bool refresh)
        {
            Debug.Assert(index == -1 || index < imageList.Images.Count);
            Debug.Assert(x >= 0 && y >= 0);

            if(index != -1)
            {
                EditorLogic.bufferBitmapGraphics.DrawImage(EditorLogic.imageList.Images[index], x, y);
            }
            else
            {
                EditorLogic.bufferBitmapGraphics.FillRectangle(
                    EditorLogic.blackBrush, x, y, EditorLogic.bricks[0].FrameWidth, 
                    EditorLogic.bricks[0].FrameHeight);
            }

            if(refresh)
            {
                EditorLogic.editorGrid.Render(EditorLogic.bufferBitmapGraphics);
                EditorLogic.renderControl.Refresh();
            }
        }

        /// <summary>
        /// Displays a message box asking the user if they want to save any changes made.
        /// </summary>
        /// <returns>DialogResult.Yes if the user clicked Yes, DialogResult.No if the user clicked
        /// No, or DialogResult.Cancel if the user clicked Cancel.</returns>
        static DialogResult PromptToSave()
        {
            return MessageBox.Show("Do you want to save the changes?", 
                "Confirmatio Needed", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
        }

        /// <summary>
        /// Renders the specified Level to the control being used for rendering.
        /// </summary>
        static void RenderLevel()
        {
            // TODO: this could be refactored into the level class.
            // Clear buffer
            // currentLevel.RenderLevel(...)
            // Draw grid
            // Refresh control.
            EditorLogic.bufferBitmapGraphics.Clear(Color.Black);

            for(int i = 0; i < EditorLogic.currentLevel.Width; i++)
            {
                for(int j = 0; j < EditorLogic.currentLevel.Height; j++)
                {
                    int brick = currentLevel[i, j];
                    if(brick != -1) // -1 means there's no brick here
                    {
                        PaintBrick(i * bricks[0].FrameWidth,
                            j * bricks[0].FrameHeight, brick, false);
                    }
                }
            }

            EditorLogic.editorGrid.Render(EditorLogic.bufferBitmapGraphics);
            EditorLogic.renderControl.Refresh();
        }

        
        /// <summary>
        /// 
        /// </summary>
        static void ResetState()
        {
            // erase the level from the screen
            EditorLogic.bufferBitmapGraphics.Clear(Color.Black);

            // make sure controls needed for editing for enabled.
            EditorLogic.mainForm.SetGUIState(GUIState.Editing);

            // remove the filename from the titlebar 
            EditorLogic.mainForm.Text = Application.ProductName;

            // don't prompt to save if the user quits the program|opens|creates a level.
            EditorLogic.workSaved = true;
            EditorLogic.currentLevelFilename = null;

            // it'll be null if no other levels have been created/opened
            // during this instance. 
            if(EditorLogic.currentLevel != null)
            {
                EditorLogic.currentLevel.Clear();
            }

            EditorLogic.undoStack.Clear();
            EditorLogic.redoStack.Clear();
        }

        /// <summary>
        /// Writes the current level to disk and updates the GUI to show its been saved.
        /// </summary>
        static void SaveAndUpdateGUI()
        {
            try
            {
                XML.WriteLevel(EditorLogic.currentLevelFilename, EditorLogic.currentLevel, EditorLogic.bricks);
                
                EditorLogic.workSaved = true;
                mainForm.Text = String.Format("{0} - {1}", Application.ProductName, EditorLogic.currentLevelFilename);
            }
            catch
            {
                throw;
            }
        }

        /// <summary>
        /// Prompts the user to save any work, displaying the Save file dialog box if neccessary.
        /// </summary>
        /// <returns>DialogResult.OK if the user saved the file, DialogResult.No if the user chose
        /// No on the prompt, DialogResult.Cancel if the user cancelled the operation.</returns>
        private static DialogResult PromptAndSaveBeforeShutdown()
        {
            DialogResult dr = DialogResult.OK;

            try
            {
                if (!workSaved)
                {
                    dr = EditorLogic.PromptToSave();

                    if (dr == System.Windows.Forms.DialogResult.Yes)
                    {
                        dr = EditorLogic.SaveAs();
                    }
                }

                return dr;
            }
            catch
            {
                throw;
            }
            
        }

        #endregion
        #region internal

        /// <summary>
        /// Disposes unmanaged resources used by the editor.
        /// </summary>
        internal static void CleanUp()
        {
            if (EditorLogic.editorGrid != null)
            {
                EditorLogic.editorGrid.Dispose();
            }

            if(EditorLogic.bufferBitmapGraphics != null)
            {
                EditorLogic.bufferBitmapGraphics.Dispose();
            }

            if(EditorLogic.bufferBitmap != null)
            {
                EditorLogic.bufferBitmap.Dispose();
            }
        }

        /// <summary>
        /// Loads the required XML and puts the editor into its default state.
        /// </summary>
        /// <param name="renderTarget">the control levels will be rendered to.</param>
        /// <param name="brickImageList">the image list that will be used to store brick images, which will be copied to the ListView.</param>
        /// <param name="listView">the listview being used as a palette.</param>
        internal static void Initialise(MainForm mainForm, Control renderTarget, ImageList brickImageList, ListView listView)
        {
            //mainForm.SetGUIState(GUIState.Default);

            EditorLogic.mainForm = mainForm;
            EditorLogic.listView = listView;
            EditorLogic.renderControl = renderTarget;
            EditorLogic.imageList = brickImageList;
            EditorLogic.bufferBitmap = new Bitmap(renderTarget.Width, renderTarget.Height);
            EditorLogic.bufferBitmapGraphics = Graphics.FromImage(bufferBitmap);

            EditorLogic.renderControl.BackgroundImage = bufferBitmap;

            EditorLogic.editorGrid.Width = renderControl.Width;
            EditorLogic.editorGrid.Height = renderControl.Height;

            try
            {
                LoadXMLData();

                // All the bricks are the same size.
                EditorLogic.imageList.ImageSize = new Size(bricks[0].FrameWidth, bricks[0].FrameHeight);

                EditorLogic.editorGrid.CellHeight = bricks[0].FrameHeight;
                EditorLogic.editorGrid.CellWidth = bricks[0].FrameWidth;

                AddBricksToListView();
            }
            catch
            {
                throw;
            }


        }

        

        /// <summary>
        /// Prompts to save unsaved work and performs clean up when the application shuts down.
        /// </summary>
        internal static bool InitiateShutdownSequence()
        {
            try
            {
                DialogResult dr = EditorLogic.PromptAndSaveBeforeShutdown();
                if(dr == DialogResult.Cancel)
                {
                    // abort the operation
                    return true;
                }

                // we're termingating; clean up and exit.
                EditorLogic.CleanUp();
                return false;
            }
            catch
            {
                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        internal static void NewLevel()
        {
            if(!workSaved)
            {
                // work hasn't been saved; ask the user if they want to save first.
                DialogResult dialogResult = PromptToSave();
                switch(dialogResult)
                {
                        // why yes, yes I would like to save.
                    case DialogResult.Yes:
                        // display the save dialog!
                        DialogResult saveDialogResult = SaveLevel();
                        if(saveDialogResult == DialogResult.Cancel)
                        {
                            // user clicked cancel on the save dialog, so abort.
                            return;
                        }
                        break;

                    case DialogResult.Cancel:
                        // user clicked cancel, nothing else to do.
                        return;
                }
            }

            ResetState();

            EditorLogic.currentLevel = new Level(41, 20);

            editorGrid.Render(EditorLogic.bufferBitmapGraphics);
            renderControl.Refresh();
        }

        /// <summary>
        /// Implements File->Open functionality.
        /// </summary>
        /// <exception cref="System.IO.FileNotFoundException">The specified file couldn't be found.</exception>
        /// <exception cref="System.FormatException">A required attribute in the XML was supposed to contain a numeric value but doesn't.</exception>
        /// <exception cref="System.Xml.XmlException">The specified XML file is missing data or is incorrectly formatted.</exception>
        internal static void OpenLevel()
        {
            try
            {
                if (!workSaved)
                {
                    // the user hasn't saved their changes; prompt to see if they
                    // want to save their work before opening a level.
                    DialogResult userResponse = PromptToSave();
                    switch (userResponse)
                    {
                        case DialogResult.Yes:
                            // Why yes, I would like to save.
                            DialogResult saveAsDialogResponse = SaveLevel();
                            if (saveAsDialogResponse == DialogResult.Cancel)
                            {
                                // user smacked 'Cancel' on the save as dialog, so cancel opening the level
                                return;
                            }
                            break;
                        case DialogResult.Cancel:
                            return;
                    }
                }

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    ResetState();

                    EditorLogic.currentLevel = XML.ReadLevel(EditorLogic.openFileDialog.FileName, EditorLogic.bricks);
                    EditorLogic.mainForm.Text = String.Format(
                    "{0} - {1}", Application.ProductName, EditorLogic.openFileDialog.FileName);
                    EditorLogic.currentLevelFilename = EditorLogic.openFileDialog.FileName;

                    EditorLogic.RenderLevel();
                }
            }
            catch
            {
                throw;
            }
            
        }

        /// <summary>
        /// File->Save As... menu item functionality.
        /// </summary>
        internal static DialogResult SaveAs()
        {
            string temp = EditorLogic.currentLevelFilename;
            EditorLogic.currentLevelFilename = null;

            try
            {
                DialogResult dr = EditorLogic.SaveLevel();
                if (dr == DialogResult.Cancel)
                {
                    EditorLogic.currentLevelFilename = temp;
                }

                return dr;
                // no need to update currentLevelFilename here because that's
                // handled by SaveLevel() on a successfull save.
            }
            catch
            {
                throw;
            }
            
        }

        /// <summary>
        /// Saves the level to disk, displaying the Save dialog box if the level
        /// hasn't been saved previously.
        /// </summary>
        /// <returns><c>DialogResult.Cancel</c> if the operation was cancelled by the user, otherwise <c>DialogResult.OK</c>.</returns>
        internal static DialogResult SaveLevel()
        {
            DialogResult dialogResult = DialogResult.Cancel;

            try
            {
                // determine whether the save file dialog needs to be displayed
                // before we write the level to disk.
                if (EditorLogic.currentLevelFilename == null)
                {
                    // null = level has never been saved before.
                    dialogResult = EditorLogic.saveFileDialog.ShowDialog();
                    if (dialogResult == DialogResult.OK)
                    {
                        EditorLogic.currentLevelFilename = EditorLogic.saveFileDialog.FileName;
                        SaveAndUpdateGUI();
                    }
                }
            }
            catch
            {
                throw;
            }

            return dialogResult;
            
        }

        #endregion

        
    }
}
