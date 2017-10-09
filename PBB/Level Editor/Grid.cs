using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace Level_Editor
{
    class Grid
    {
        bool visible = false;
        Color gridColor = Color.Green;

        public bool Visible
        {
            get { return visible; }
            set { visible = value; }
        }

        public Color Colour
        {
            get { return gridColor; }
            set { gridColor = value; }
        }

        public void Draw(Graphics renderTo)
        {
            if (!visible)
            {
                return;
            }

            using(Pen pen = new Pen(gridColor))
            {
                for (int i = 0; i < 480; i += 20)
                {
                    // draw horizontal lines
                    renderTo.DrawLine(pen, 0, i, 1024, i);
                }

                for (int i = 0; i < 1024; i += 41)
                {
                    // vertical lines
                    renderTo.DrawLine(pen, i, 0, i, 460);
                }

                // 1024/41 doesn't divide evenly, the right most cells 
                // are 40 pixels wide. It's not really noticeable in the game
                // but it also means the grid doesn't quite render as desired.
                // This ensures the right side of the cells are drawn. 
                renderTo.DrawLine(pen, 1023, 0, 1023, 460); 
            }
        }
    }
}
