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

        // paths for XML and bitmap data required by the editor.
        static Paths paths;

        static Grid editorGrid;

        // the current level being edited.
        static Level currentLevel;

        const uint levelWidth = 25;
        const uint levelHeight = 23;

        // the editors GUI.
        static MainForm mainForm;

        // the list view containing the bricks.
        static ListView listView;

        // the control used to render the map when it's displayed.
        static Control renderControl;

        // contains all the bricks loaded from the directories contained in
        // EditorLogic.paths. Required so the List View control can display the bricks.
        static ImageList imageList;

        static SaveFileDialog saveFileDialog = new SaveFileDialog();
        static OpenFileDialog openFileDialog = new OpenFileDialog();
        static ColorDialog colourDialog = new ColorDialog();

        // off screen surface that the levels are rendered to, then it's passed
        // to EditorLogic.renderControl to display the level on the screen.
        static Bitmap bufferBitmap;

        // bufferBitmap's drawing surface. 
        static Graphics bufferBitmapGraphics;

        // used when the user erases a brick 
        static SolidBrush blackBrush = new SolidBrush(Color.Black);

        // buffers to hold painted bricks so the user can undo/redo as needed.
        static Stack<CoordsAndIndex[]> undoStack = new Stack<CoordsAndIndex[]>();
        static Stack<CoordsAndIndex[]> redoStack = new Stack<CoordsAndIndex[]>();

        // buffer used to hold bricks as they're being painted. It's copied to undoStack and cleared
        // when the user unpresses the mouse button.
        static List<CoordsAndIndex> paintBuffer = new List<CoordsAndIndex>();

        // the absolute path for the games directory.
        static string gameDirectory;

        // the absolute path of the level that's currently being edited.
        static string currentLevelFilename;

        // specifies whether the user is currently using the eraser, the brush or neither.
        static EditMode editMode = EditMode.None;

        // true if the latest edits to the level has been saved, otherwise false.
        static bool workSaved = true;

        // the ListView's selected index of the currently selected brick.
        // applied to the image list so bricks can actually be painted.
        private static int selectedBrick;

        // true if the user has the mouse button down and is currently painting bricks on EditorLogic.renderControl.
        private static bool isPainting;

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
                //List<string> directoryList = new List<string>();

                //foreach(Path directory in paths.BitmapPaths)
                //{
                //    if (!directoryList.Contains(directory.ResourcePath))
                //    {
                //        directoryList.Add(directory.ResourcePath);

                //        if (directory.IncludeSubDirectories)
                //        {
                //            string tempPath = String.Format("{0}/{1}", gameDirectory, directory.ResourcePath);
                //            string[] directories = Directory.GetDirectories(tempPath);

                //            if (directories != null)
                //            {
                //                directoryList.AddRange(directories);
                //            }
                //        }
                //    }
                //}

                // find which path contains entities.xml
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
        /// Puts the Editor into a GUIState.Editing state with a blank level visible.
        /// Effectively the same state as selecting File->New.
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
        /// Draws or erases the selected brick and puts the editor into a "painting" state.
        /// </summary>
        /// <param name="mousePosition">pixel coordinates of the mouse pointer.</param>
        /// <remarks>The coordinates passed via <c>mousePosition</c> don't reflect where the brick
        /// will be drawn. Bricks have to tile, so the coordinates will most likely be modified
        /// during the methods execution so the brick "snaps" into place at the correct coordinate. 
        /// In other words, they'll be converted from screen coordinates to brick (tile) coordinates.</remarks>
        internal static void BeginPainting(Point mousePosition)
        {
            try
            {
                if(EditorLogic.editMode == EditMode.Brush || 
                   EditorLogic.editMode == EditMode.Eraser)
                {
                    EditorLogic.isPainting = true;
                    
                    int tileCoordX = mousePosition.X / EditorLogic.bricks[0].FrameWidth;
                    int tileCoordY = mousePosition.Y / EditorLogic.bricks[0].FrameHeight;

                    // don't allow drawing out of bounds.
                    if (tileCoordX < EditorLogic.currentLevel.Width && tileCoordY < EditorLogic.currentLevel.Height)
                    {
                        // no point in painting a brick if it's already at these coordinates
                        if(EditorLogic.currentLevel[tileCoordX, tileCoordY] != EditorLogic.selectedBrick)
                        {
                            EditorLogic.redoStack.Clear();
                            EditorLogic.mainForm.SetRedo(false);

                            // they might appear to be the same as the mouse coordinates but
                            // they're not; these cause the brick to "snap" into place, so everything
                            // is nicely tiled.
                            int screenX = tileCoordX * EditorLogic.bricks[0].FrameWidth;
                            int screenY = tileCoordY * EditorLogic.bricks[0].FrameHeight;

                            int oldBrick = EditorLogic.currentLevel[tileCoordX, tileCoordY];
                            EditorLogic.paintBuffer.Add(new CoordsAndIndex(screenX, screenY, oldBrick));

                            // selectedBrick is set when a brick is selected in the list view.
                            EditorLogic.currentLevel[tileCoordX, tileCoordY] = EditorLogic.selectedBrick;

                            EditorLogic.PaintBrick(screenX, screenY, selectedBrick, true);
                            if(workSaved)
                            {
                                // changes have been made that need to be saved, indicate
                                // this to the user.
                                EditorLogic.workSaved = false;
                                EditorLogic.mainForm.Text += "*";
                            }
                        }
                    }
                }
            }
            catch
            {
                // fix for #73. Don't allow the user to keep painting if an error
                // is thrown; prevents a trail of bricks from being drawn after the user
                // has released the mouse button to dismiss the message box caused by the exception.
                EditorLogic.isPainting = false;

                throw;
            }
        }

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
        /// Puts the editor into the specified edit mode.
        /// </summary>
        /// <param name="editMode">the mode the user has selected.</param>
        internal static void EditToolSelected(EditMode editMode)
        {
            // TODO: this needs to be refactored/rewritten. The switch is now redundant as its 
            // only ever calld by the eraser button and menu item.

            try
            {
                switch (editMode)
                {
                    case EditMode.Eraser:
                        if (EditorLogic.editMode == EditMode.Eraser)
                        {
                            // user has unchecked the eraser button or menu item.
                            EditorLogic.editMode = EditMode.None;
                            EditorLogic.mainForm.CheckEraserControls(false);
                        }
                        else
                        {
                            //EditorLogic.mainForm.Cursor = mainForm.EraserCursor();
                            EditorLogic.editMode = editMode;
                            EditorLogic.selectedBrick = -1;
                            EditorLogic.mainForm.ClearListViewSelection();
                            EditorLogic.mainForm.CheckEraserControls(true);
                        }

                        break;

                }
            }
            catch
            {
                throw;
            }
        }

        /// <summary>
        /// Finalizes the painting operation and updates the GUI.
        /// </summary>
        internal static void EndPainting()
        {
            try
            {
                EditorLogic.isPainting = false;
                if (EditorLogic.paintBuffer.Count > 0)
                {
                    // we've painted some bricks, so add the old ones that we painted over
                    // to the undo stack. When we click Undo, they'll magically appear
                    // and peace will be throughout the land.
                    EditorLogic.undoStack.Push(EditorLogic.paintBuffer.ToArray());
                    EditorLogic.paintBuffer.Clear();

                    EditorLogic.mainForm.SetUndo(true);
                }

                EditorLogic.mainForm.SetSaveItems(true);
            }
            catch
            {
                throw;
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
            openFileDialog.Filter = "XML files|*.xml";
            saveFileDialog.Filter = openFileDialog.Filter;

            EditorLogic.mainForm = mainForm;
            EditorLogic.listView = listView;
            EditorLogic.renderControl = renderTarget;
            EditorLogic.imageList = brickImageList;
            EditorLogic.bufferBitmap = new Bitmap(renderTarget.Width, renderTarget.Height);
            EditorLogic.bufferBitmapGraphics = Graphics.FromImage(bufferBitmap);

            EditorLogic.renderControl.BackgroundImage = bufferBitmap;

            EditorLogic.editorGrid = new Grid(EditorLogic.renderControl.Size.Width, 
                EditorLogic.renderControl.Size.Height,
                Properties.Settings.Default.GridColour, 
                Properties.Settings.Default.GridIsVisible);

            EditorLogic.mainForm.CheckGridControls(EditorLogic.editorGrid.Visible);

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
        /// <returns><b>false</b> if the user cancelled the operation, otherwise <b>true</b>.</returns>
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
        /// Updates the editing state based on what's been selected (deselected) in the listview.
        /// </summary>
        internal static void ListViewSelectionChanged()
        {
            // an item has been selected in the listview, so we're now in paint mode.
            if (EditorLogic.listView.SelectedItems.Count == 1)
            {
                EditorLogic.editMode = EditMode.Brush;
                EditorLogic.selectedBrick = listView.Items.IndexOf(listView.SelectedItems[0]);
                EditorLogic.mainForm.CheckEraserControls(false);
            }
            else
            {
                // a brick was unselected; how we handle it depends on which edit mode we're in.
                if (EditorLogic.editMode == EditMode.Brush)
                {
                    EditorLogic.editMode = EditMode.None;
                }
                else
                {
                    // user has selected the eraser, which was unselected the item.
                    //EditorLogic.editMode = EditMode.
                }
            }
        }

        /// <summary>
        /// Paints a brick at the specified mouse coordinates. This method is designed to be called as 
        /// the mouse moves.
        /// </summary>
        /// <param name="mousePosition">The pixel coordinates of the mouse.</param>
        internal static void Painting(Point mousePosition)
        {
            try
            {
                if (EditorLogic.isPainting)
                {
                    int tileCoordX = mousePosition.X / EditorLogic.bricks[0].FrameWidth;
                    int tileCoordY = mousePosition.Y / EditorLogic.bricks[0].FrameHeight;

                    if (tileCoordX < EditorLogic.currentLevel.Width && tileCoordY < EditorLogic.currentLevel.Height)
                    {
                        if (EditorLogic.currentLevel[tileCoordX, tileCoordY] != EditorLogic.selectedBrick)
                        {
                            // they might appear to be the same as the mouse coordinates but
                            // they're not; these cause the brick to "snap" into place, so everything
                            // is nicely tiled.
                            int screenX = tileCoordX * EditorLogic.bricks[0].FrameWidth;
                            int screenY = tileCoordY * EditorLogic.bricks[0].FrameHeight;

                            int oldBrick = EditorLogic.currentLevel[tileCoordX, tileCoordY];
                            EditorLogic.paintBuffer.Add(new CoordsAndIndex(screenX, screenY, oldBrick));

                            EditorLogic.currentLevel[tileCoordX, tileCoordY] = selectedBrick;
                            EditorLogic.PaintBrick(screenX, screenY, selectedBrick, true);
                        }
                    }
                }
            }
            catch
            {
                // fix for #73. Don't allow the user to keep painting if an error
                // is thrown; prevents a trail of bricks from being drawn after the user
                // has released the mouse button to dismiss the message box caused by the exception.
                EditorLogic.isPainting = false;

                throw;
            }
        }

        /// <summary>
        /// Implements the File->New functionality.
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

            EditorLogic.currentLevel = new Level(EditorLogic.levelWidth, EditorLogic.levelHeight);

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

                    // bug fix: don't show the filename of the level that was just opened the next time
                    // the editor displays the dialog.
                    EditorLogic.openFileDialog.FileName = String.Empty;

                    EditorLogic.RenderLevel();
                }
            }
            catch
            {
                throw;
            }
            
        }

        /// <summary>
        /// Implements Edit->Redo functionality.
        /// </summary>
        internal static void Redo()
        {
            try
            {
                CoordsAndIndex[] bricks = redoStack.Pop();
                List<CoordsAndIndex> temp = new List<CoordsAndIndex>();

                foreach (CoordsAndIndex brick in bricks)
                {
                    int tileCoordX = brick.X / EditorLogic.bricks[0].FrameWidth;
                    int tileCoordY = brick.Y / EditorLogic.bricks[0].FrameHeight;

                    int brickIndex = EditorLogic.currentLevel[tileCoordX, tileCoordY];
                    temp.Add(new CoordsAndIndex(brick.X, brick.Y, brickIndex));

                    EditorLogic.currentLevel[tileCoordX, tileCoordY] = brick.Index;

                    EditorLogic.PaintBrick(brick.X, brick.Y, brick.Index, false);
                }

                EditorLogic.undoStack.Push(temp.ToArray());
                EditorLogic.editorGrid.Render(EditorLogic.bufferBitmapGraphics);
                EditorLogic.renderControl.Refresh();

                if (EditorLogic.redoStack.Count == 0)
                {
                    EditorLogic.mainForm.SetRedo(false);
                }

                EditorLogic.mainForm.SetUndo(true);
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

                        // clearing the filename in the textbox to prevent the names
                        // of previous levels appearing in the dialog when a new level
                        // is created and saved for the first time. (bug fix for #82)
                        EditorLogic.saveFileDialog.FileName = String.Empty;
                    }
                    else
                    {
                        return dialogResult;
                    }
                }

                SaveAndUpdateGUI();
            }
            catch
            {
                throw;
            }

            return dialogResult;
            
        }

        /// <summary>
        /// Displays the colour dialog so the user can change the colour of the grid.
        /// </summary>
        internal static void ShowColourDialog()
        {
            try
            {
                if (EditorLogic.colourDialog.ShowDialog() == DialogResult.OK)
                {
                    EditorLogic.editorGrid.Colour = EditorLogic.colourDialog.Color;
                    RenderLevel();
                }
            }
            catch
            {
                throw;
            }
            
        }

        #endregion

        /// <summary>
        /// Functionality for the grid menu item and toolstrip button.
        /// </summary>
        internal static void ToggleGrid()
        {
            try
            {
                // this has to be before calling RenderLevel() because the method 
                // checks to see if the grid needs to be rendered.
                EditorLogic.editorGrid.Visible = !EditorLogic.editorGrid.Visible;
                EditorLogic.mainForm.CheckGridControls(EditorLogic.editorGrid.Visible);

                EditorLogic.RenderLevel();

                Properties.Settings.Default.GridIsVisible = EditorLogic.editorGrid.Visible;
                Properties.Settings.Default.Save();
            }
            catch
            {
                throw;
            }
        }

        /// <summary>
        /// Edit->Undo functionality.
        /// </summary>
        internal static void Undo()
        {

            try
            {
                CoordsAndIndex[] poppedBricks = undoStack.Pop();
                List<CoordsAndIndex> temp = new List<CoordsAndIndex>();

                foreach (CoordsAndIndex brick in poppedBricks)
                {
                    // brick.x and brick.y contain screen coordinates. If you don't
                    // convert to tile coordinates, you'll get a buffer overflow (and they'll be wrong regardless).
                    int tileCoordX = brick.X / EditorLogic.bricks[0].FrameWidth;
                    int tileCoordY = brick.Y / EditorLogic.bricks[0].FrameHeight;

                    // take each brick we're about the paint over and stick them in a buffer.
                    // the buffer will be passed to the redo stack when we're done.
                    int brickIndex = EditorLogic.currentLevel[tileCoordX, tileCoordY];
                    temp.Add(new CoordsAndIndex(brick.X, brick.Y, brickIndex));

                    EditorLogic.currentLevel[tileCoordX, tileCoordY] = brick.Index;

                    // disabling the refresh because otherwise each brick is drawn and then displayed.
                    // It results in a domino-like effect; we want to see all the bricks appear at once.
                    EditorLogic.PaintBrick(brick.X, brick.Y, brick.Index, false);
                }

                EditorLogic.redoStack.Push(temp.ToArray());
                EditorLogic.editorGrid.Render(EditorLogic.bufferBitmapGraphics);
                EditorLogic.renderControl.Refresh();

                if (undoStack.Count == 0)
                {
                    EditorLogic.mainForm.SetUndo(false);
                }

                EditorLogic.mainForm.SetRedo(true);
            }
            catch
            {
                throw;
            }
        }
    }
}
