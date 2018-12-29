using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PLeD
{
    /// <summary>
    /// 
    /// </summary>
    struct CoordsAndIndex
    {
        public int X;
        public int Y;
        public int Index;

        public CoordsAndIndex(int x, int y, int index)
        {
            this.X = x;
            this.Y = y;
            this.Index = index;
        }
    }
}
