using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;

namespace Level_Editor
{
    public partial class About : Form
    {
        public About()
        {
            InitializeComponent();
            string programNameVersion = String.Format(
                "LevelED v{0}.{1}.{2}",
                Assembly.GetExecutingAssembly().GetName().Version.Major,
                Assembly.GetExecutingAssembly().GetName().Version.Minor,
                Assembly.GetExecutingAssembly().GetName().Version.Build
                );

            titleLabel.Text = programNameVersion;
        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
