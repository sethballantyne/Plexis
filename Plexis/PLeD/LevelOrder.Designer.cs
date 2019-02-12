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
            this.deleteButton = new System.Windows.Forms.Button();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.listbox = new System.Windows.Forms.ListBox();
            this.moveUpButton = new System.Windows.Forms.Button();
            this.moveDownButton = new System.Windows.Forms.Button();
            this.levelOrderGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(192, 227);
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
            this.okayButton.Location = new System.Drawing.Point(111, 227);
            this.okayButton.Name = "okayButton";
            this.okayButton.Size = new System.Drawing.Size(75, 23);
            this.okayButton.TabIndex = 1;
            this.okayButton.Text = "&OK";
            this.okayButton.UseVisualStyleBackColor = true;
            this.okayButton.VisibleChanged += new System.EventHandler(this.okayButton_VisibleChanged);
            // 
            // levelOrderGroupBox
            // 
            this.levelOrderGroupBox.Controls.Add(this.deleteButton);
            this.levelOrderGroupBox.Controls.Add(this.listbox);
            this.levelOrderGroupBox.Controls.Add(this.moveUpButton);
            this.levelOrderGroupBox.Controls.Add(this.moveDownButton);
            this.levelOrderGroupBox.Location = new System.Drawing.Point(15, 12);
            this.levelOrderGroupBox.Name = "levelOrderGroupBox";
            this.levelOrderGroupBox.Size = new System.Drawing.Size(252, 209);
            this.levelOrderGroupBox.TabIndex = 4;
            this.levelOrderGroupBox.TabStop = false;
            this.levelOrderGroupBox.Text = "Levels";
            // 
            // deleteButton
            // 
            this.deleteButton.Enabled = false;
            this.deleteButton.ImageKey = "Delete.bmp";
            this.deleteButton.ImageList = this.imageList;
            this.deleteButton.Location = new System.Drawing.Point(221, 106);
            this.deleteButton.Name = "deleteButton";
            this.deleteButton.Size = new System.Drawing.Size(25, 23);
            this.deleteButton.TabIndex = 5;
            this.deleteButton.UseVisualStyleBackColor = true;
            this.deleteButton.Click += new System.EventHandler(this.deleteButton_Click);
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.Fuchsia;
            this.imageList.Images.SetKeyName(0, "BuilderDialog_movedown.bmp");
            this.imageList.Images.SetKeyName(1, "BuilderDialog_moveup.bmp");
            this.imageList.Images.SetKeyName(2, "Delete.bmp");
            // 
            // listbox
            // 
            this.listbox.FormattingEnabled = true;
            this.listbox.Location = new System.Drawing.Point(6, 19);
            this.listbox.Name = "listbox";
            this.listbox.Size = new System.Drawing.Size(209, 186);
            this.listbox.TabIndex = 4;
            this.listbox.SelectedIndexChanged += new System.EventHandler(this.listbox_SelectedIndexChanged);
            // 
            // moveUpButton
            // 
            this.moveUpButton.Enabled = false;
            this.moveUpButton.ImageIndex = 1;
            this.moveUpButton.ImageList = this.imageList;
            this.moveUpButton.Location = new System.Drawing.Point(221, 19);
            this.moveUpButton.Name = "moveUpButton";
            this.moveUpButton.Size = new System.Drawing.Size(25, 23);
            this.moveUpButton.TabIndex = 3;
            this.moveUpButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.moveUpButton.UseVisualStyleBackColor = true;
            this.moveUpButton.Click += new System.EventHandler(this.moveUpButton_Click);
            // 
            // moveDownButton
            // 
            this.moveDownButton.Enabled = false;
            this.moveDownButton.ImageIndex = 0;
            this.moveDownButton.ImageList = this.imageList;
            this.moveDownButton.Location = new System.Drawing.Point(221, 48);
            this.moveDownButton.Name = "moveDownButton";
            this.moveDownButton.Size = new System.Drawing.Size(25, 23);
            this.moveDownButton.TabIndex = 2;
            this.moveDownButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.moveDownButton.UseVisualStyleBackColor = true;
            this.moveDownButton.Click += new System.EventHandler(this.moveDownButton_Click);
            // 
            // LevelOrder
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(279, 262);
            this.ControlBox = false;
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
            this.levelOrderGroupBox.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button okayButton;
        private System.Windows.Forms.GroupBox levelOrderGroupBox;
        private System.Windows.Forms.ListBox listbox;
        private System.Windows.Forms.Button moveUpButton;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.Button moveDownButton;
        private System.Windows.Forms.Button deleteButton;

    }
}