using System;
using System.Diagnostics;
using SharpDX;
using SharpDX.Direct2D1;
using SharpDX.Direct3D;
using SharpDX.Direct3D11;
using SharpDX.DXGI;
using SharpDX.Windows;

using AlphaMode = SharpDX.Direct2D1.AlphaMode;
using Device = SharpDX.Direct3D11.Device;
using Factory = SharpDX.DXGI.Factory;

namespace Snake
{
    internal static class Program
    {
        [STAThread]
        private static void Main()
        {
            using(var game = new SnakeGame()){
                game.Run();
            }
        }
    }
}