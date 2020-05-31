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

namespace PLeD
{
    partial class LevelOrder
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LevelOrder));
            this.cancelButton = new System.Windows.Forms.Button();
            this.okayButton = new System.Windows.Forms.Button();
            this.levelOrderGroupBox = new System.Windows.Forms.GroupBox();
            this.rotationListDeleteButton = new System.Windows.Forms.Button();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.rotationListListBox = new System.Windows.Forms.ListBox();
            this.rotationListMoveUpButton = new System.Windows.Forms.Button();
            this.rotationListMoveDownButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.availableLevelsAddButton = new System.Windows.Forms.Button();
            this.availableLevelsListBox = new System.Windows.Forms.ListBox();
            this.levelOrderGroupBox.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(479, 227);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 0;
            this.cancelButton.Text = "&Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // okayButton
            // 
            this.okayButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okayButton.Enabled = false;
            this.okayButton.Location = new System.Drawing.Point(398, 227);
            this.okayButton.Name = "okayButton";
            this.okayButton.Size = new System.Drawing.Size(75, 23);
            this.okayButton.TabIndex = 1;
            this.okayButton.Text = "&OK";
            this.okayButton.UseVisualStyleBackColor = true;
            this.okayButton.VisibleChanged += new System.EventHandler(this.okayButton_VisibleChanged);
            // 
            // levelOrderGroupBox
            // 
            this.levelOrderGroupBox.Controls.Add(this.rotationListDeleteButton);
            this.levelOrderGroupBox.Controls.Add(this.rotationListListBox);
            this.levelOrderGroupBox.Controls.Add(this.rotationListMoveUpButton);
            this.levelOrderGroupBox.Controls.Add(this.rotationListMoveDownButton);
            this.levelOrderGroupBox.Location = new System.Drawing.Point(270, 12);
            this.levelOrderGroupBox.Name = "levelOrderGroupBox";
            this.levelOrderGroupBox.Size = new System.Drawing.Size(282, 209);
            this.levelOrderGroupBox.TabIndex = 4;
            this.levelOrderGroupBox.TabStop = false;
            this.levelOrderGroupBox.Text = "Level Rotation List";
            // 
            // rotationListDeleteButton
            // 
            this.rotationListDeleteButton.Enabled = false;
            this.rotationListDeleteButton.ImageKey = "left.png";
            this.rotationListDeleteButton.ImageList = this.imageList;
            this.rotationListDeleteButton.Location = new System.Drawing.Point(6, 19);
            this.rotationListDeleteButton.Name = "rotationListDeleteButton";
            this.rotationListDeleteButton.Size = new System.Drawing.Size(25, 23);
            this.rotationListDeleteButton.TabIndex = 5;
            this.rotationListDeleteButton.UseVisualStyleBackColor = true;
            this.rotationListDeleteButton.Click += new System.EventHandler(this.deleteButton_Click);
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.Fuchsia;
            this.imageList.Images.SetKeyName(0, "BuilderDialog_movedown.bmp");
            this.imageList.Images.SetKeyName(1, "BuilderDialog_moveup.bmp");
            this.imageList.Images.SetKeyName(2, "Delete.bmp");
            this.imageList.Images.SetKeyName(3, "right.png");
            this.imageList.Images.SetKeyName(4, "left.png");
            // 
            // rotationListListBox
            // 
            this.rotationListListBox.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.rotationListListBox.FormattingEnabled = true;
            this.rotationListListBox.Location = new System.Drawing.Point(37, 19);
            this.rotationListListBox.Name = "rotationListListBox";
            this.rotationListListBox.Size = new System.Drawing.Size(208, 186);
            this.rotationListListBox.TabIndex = 4;
            this.rotationListListBox.SelectedIndexChanged += new System.EventHandler(this.listbox_SelectedIndexChanged);
            this.rotationListListBox.DoubleClick += new System.EventHandler(this.rotationListListBox_DoubleClick);
            // 
            // rotationListMoveUpButton
            // 
            this.rotationListMoveUpButton.Enabled = false;
            this.rotationListMoveUpButton.ImageIndex = 1;
            this.rotationListMoveUpButton.ImageList = this.imageList;
            this.rotationListMoveUpButton.Location = new System.Drawing.Point(251, 19);
            this.rotationListMoveUpButton.Name = "rotationListMoveUpButton";
            this.rotationListMoveUpButton.Size = new System.Drawing.Size(25, 23);
            this.rotationListMoveUpButton.TabIndex = 3;
            this.rotationListMoveUpButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.rotationListMoveUpButton.UseVisualStyleBackColor = true;
            this.rotationListMoveUpButton.Click += new System.EventHandler(this.moveUpButton_Click);
            // 
            // rotationListMoveDownButton
            // 
            this.rotationListMoveDownButton.Enabled = false;
            this.rotationListMoveDownButton.ImageIndex = 0;
            this.rotationListMoveDownButton.ImageList = this.imageList;
            this.rotationListMoveDownButton.Location = new System.Drawing.Point(251, 48);
            this.rotationListMoveDownButton.Name = "rotationListMoveDownButton";
            this.rotationListMoveDownButton.Size = new System.Drawing.Size(25, 23);
            this.rotationListMoveDownButton.TabIndex = 2;
            this.rotationListMoveDownButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.rotationListMoveDownButton.UseVisualStyleBackColor = true;
            this.rotationListMoveDownButton.Click += new System.EventHandler(this.moveDownButton_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.availableLevelsAddButton);
            this.groupBox1.Controls.Add(this.availableLevelsListBox);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(252, 209);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Available Levels";
            // 
            // availableLevelsAddButton
            // 
            this.availableLevelsAddButton.Enabled = false;
            this.availableLevelsAddButton.ImageKey = "right.png";
            this.availableLevelsAddButton.ImageList = this.imageList;
            this.availableLevelsAddButton.Location = new System.Drawing.Point(221, 19);
            this.availableLevelsAddButton.Name = "availableLevelsAddButton";
            this.availableLevelsAddButton.Size = new System.Drawing.Size(25, 23);
            this.availableLevelsAddButton.TabIndex = 5;
            this.availableLevelsAddButton.UseVisualStyleBackColor = true;
            this.availableLevelsAddButton.Click += new System.EventHandler(this.availableLevelsAddButton_Click);
            // 
            // availableLevelsListBox
            // 
            this.availableLevelsListBox.FormattingEnabled = true;
            this.availableLevelsListBox.Location = new System.Drawing.Point(6, 19);
            this.availableLevelsListBox.Name = "availableLevelsListBox";
            this.availableLevelsListBox.Size = new System.Drawing.Size(209, 186);
            this.availableLevelsListBox.TabIndex = 4;
            this.availableLevelsListBox.SelectedIndexChanged += new System.EventHandler(this.availableLevelsListBox_SelectedIndexChanged);
            this.availableLevelsListBox.DoubleClick += new System.EventHandler(this.availableLevelsListBox_DoubleClick);
            // 
            // LevelOrder
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(566, 262);
            this.ControlBox = false;
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.levelOrderGroupBox);
            this.Controls.Add(this.okayButton);
            this.Controls.Add(this.cancelButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "LevelOrder";
            this.Text = "Change Level Order";
            this.TopMost = true;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.LevelOrder_FormClosing);
            this.Shown += new System.EventHandler(this.LevelOrder_Shown);
            this.levelOrderGroupBox.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button okayButton;
        private System.Windows.Forms.GroupBox levelOrderGroupBox;
        private System.Windows.Forms.ListBox rotationListListBox;
        private System.Windows.Forms.Button rotationListMoveUpButton;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.Button rotationListMoveDownButton;
        private System.Windows.Forms.Button rotationListDeleteButton;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button availableLevelsAddButton;
        private System.Windows.Forms.ListBox availableLevelsListBox;

    }
}