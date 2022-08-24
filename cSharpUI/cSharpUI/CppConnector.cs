using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace cSharpUI
{
    /// <summary>
    /// Connects to the C++ application for the simulation.
    /// </summary>
    internal class CppConnector
    {
        /// <summary>
        /// Returns the first port on the system that is free to use.
        /// </summary>
        /// <returns>The first port on the system that is free to use.</returns>
        static private int FindFreePort()
        {
            TcpListener tcpListener = new TcpListener(IPAddress.Loopback, 0);
            tcpListener.Start();
            var port = ((IPEndPoint)tcpListener.LocalEndpoint).Port;
            tcpListener.Stop();
            return port;
        }

        /// <summary>
        /// Returns the path of the application to call.
        /// </summary>
        /// <returns>The path of the application to call, null if nothing was found.</returns>
        static private string FindApplication()
        {
            var buildDirPath = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), "..", "..", "..", "..", "out", "build"));
            var release = Path.Combine(buildDirPath, "x64-release", "cSharpConnector", "cSharpConnector.exe");
            var debug = Path.Combine(buildDirPath, "x64-debug", "cSharpConnector", "cSharpConnector.exe");
            if (File.Exists(release))
            {
                return release;
            }
            if (File.Exists(debug))
            {
                return debug;
            }
            return null;
        }

        /// <summary>
        /// The image resulting from the simulation.
        /// </summary>
        public Image SimulationResult { get; private set; }

        /// <summary>
        /// The simulation process running the C++ application.
        /// </summary>
        private Process simulationProcess = null;

        /// <summary>
        /// The socket used for the communication.
        /// </summary>
        private Socket socket = null;

        /// <summary>
        /// Thread to receiver data from the auto gamer.
        /// </summary>
        private Thread receiverThread = null;

        /// <summary>
        /// Buffer to assemble larger structures of received data.
        /// </summary>
        private byte[] assemblyBuffer = null;

        /// <summary>
        /// Initializes a new CppConnector using the parameters for the simulation.
        /// </summary>
        /// <param name="cellSize">The size of the cells in the simulation.</param>
        /// <param name="columns">The amount of columns in the simulation.</param>
        /// <param name="rows">The amount of columns in the simulation.</param>
        public CppConnector(int cellSize, int columns, int rows) {
            assemblyBuffer = new byte[128 + (cellSize * cellSize * columns * rows * 4)];
            var port = FindFreePort();
            var path = FindApplication();
            if(path != null)
            {
                var info = new ProcessStartInfo(path);
                info.CreateNoWindow = true;
                info.Arguments = "" + port + " " + cellSize + " " + columns + " " + rows;
                info.UseShellExecute = false;
                simulationProcess = Process.Start(info);
                Thread.Sleep(500);

                try
                {
                    IPHostEntry hostEntry = Dns.GetHostEntry("localhost");
                    foreach (IPAddress address in hostEntry.AddressList)
                    {
                        if (address.AddressFamily == AddressFamily.InterNetworkV6)
                        {
                            continue;
                        }
                        IPEndPoint ipe = new IPEndPoint(address, port);
                        Socket tempSocket = new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
                        tempSocket.Connect(ipe);
                        if (tempSocket.Connected)
                        {
                            socket = tempSocket;
                            break;
                        }
                    }
                }
                catch (SocketException socketException)
                {
                    throw new IOException(socketException.Message);
                }

                if (socket == null)
                {
                    throw new IOException("Es konnte keine Verbindung geöffnet werden.");
                }

                var threadStart = new ThreadStart(this.ThreadMethod);
                receiverThread = new Thread(threadStart);
                receiverThread.Start();
            }
        }

        /// <summary>
        /// Adds a new Particle to the simulation.
        /// </summary>
        /// <param name="position">The position of the particle.</param>
        public void AddParticle(Point position)
        {
            lock (this)
            {
                socket?.Send(Encoding.UTF8.GetBytes("add " + position.X + " " + position.Y + "\n\0"));
            }
        }

        /// <summary>
        /// Executes the next step in the simulation.
        /// </summary>
        public void Simulate()
        { 
            lock (this)
            {
                socket?.Send(Encoding.UTF8.GetBytes("draw\n\0"));
            }
        }

        /// <summary>
        /// Closes the connection to the application, shuting it down in the process.
        /// </summary>
        public void Close()
        {
            lock (this) 
            { 
                socket?.Send(Encoding.UTF8.GetBytes("close\n\0"));
                socket?.Close();
                socket = null;
            }
        }

        /// <summary>
        /// Method of the receiver thread.
        /// </summary>
        public void ThreadMethod()
        {
            socket.ReceiveTimeout = 1000;
            while (socket != null && socket.Connected)
            {
                try
                {
                    var length = socket.Receive(assemblyBuffer);
                    // Convert received data to image
                    using (var stream = new MemoryStream(assemblyBuffer, 0, length))
                    {
                        SimulationResult = new Bitmap(stream);
                    }
                }
                catch (SocketException e)
                {
                    Console.WriteLine(e);
                }
                catch(Exception e)
                {
                    Console.WriteLine(e);
                    Close();
                }
            }
        }
    }
}
