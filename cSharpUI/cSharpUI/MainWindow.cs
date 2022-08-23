using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace cSharpUI
{
    /// <summary>
    /// UI for the C++ application.
    /// </summary>
    public partial class MainWindow : Form
    {
        /// <summary>
        /// The connector to the C++ application.
        /// </summary>
        private CppConnector connector = null;

        /// <summary>
        /// Constructor for the window.
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();
            connector = new CppConnector(20, 64, 36);
        }

        // GUI methods
        private void pictureBox_Click(object sender, EventArgs e)
        {
            var mouseArgs = e as MouseEventArgs;
            connector.AddParticle(mouseArgs.Location);
            //toolStripStatusLabel.Text = "";
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            connector.Simulate();
            pictureBox.Image = connector.SimulationResult;
        }

        private void MainWindow_FormClosing(object sender, FormClosingEventArgs e)
        {
            connector.Close();
        }
    }
}
