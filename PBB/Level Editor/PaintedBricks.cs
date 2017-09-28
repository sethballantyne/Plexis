using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace Level_Editor
{
    class PaintedBricks
    {
        List<Point> brickLocations = new List<Point>();

        // index of the brick in the palette listview. 
        ushort brickIndex;

        public PaintedBricks(Point[] brickLocations, ushort index)
        {
            if (brickLocations == null)
            {
                throw new ArgumentNullException("brickLocations");
            }

            this.brickLocations.AddRange(brickLocations);
            brickIndex = index;
        }

        public PaintedBricks(ushort x, ushort y, ushort index)
        {
            Point p = new Point(x, y);

            brickLocations.Add(p);
            brickIndex = index;
        }

        public Point[] GetPoints()
        {
            return brickLocations.ToArray();
        }

        public void AddCoordinate(ushort x, ushort y)
        {
            Point p = new Point(x, y);

            brickLocations.Add(p);
        }
    }
}
