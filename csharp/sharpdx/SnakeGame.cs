using System;
using System.Diagnostics;
using System.Collections.Generic;
using SharpDX;
using SharpDX.Direct2D1;
using SharpDX.Direct3D;
using SharpDX.Direct3D11;
using SharpDX.DirectInput;
using SharpDX.DXGI;
using SharpDX.Windows;

using AlphaMode = SharpDX.Direct2D1.AlphaMode;
using Device = SharpDX.Direct3D11.Device;
using Factory = SharpDX.DXGI.Factory;

namespace Snake
{
    public static class Constants {
        public const int SPRITE_SIZE = 20;
        public const int SCREEN_SIZE = 20;
        public const int FRAMES_PER_SECOND = 15;
        public const int INITIAL_TAIL = 5;
    }

    public struct Point {
        public int X;
        public int Y;
    }

    public abstract class Drawable {
        protected SolidColorBrush brush;
        public int x;
        public int y;
        public int width = Constants.SPRITE_SIZE - 1;
        public int height = Constants.SPRITE_SIZE - 1;
        
        public void Initialize(RenderTarget renderTarget, Color color) {
            brush = new SolidColorBrush(renderTarget, color);
        }
    }

    public class Apple : Drawable {
        public void Draw(RenderTarget renderTarget, SharpDX.Direct2D1.Factory factory) {
            var x = this.x * Constants.SPRITE_SIZE + 1;
            var y = this.y * Constants.SPRITE_SIZE + 1;
            var rectangleGeometry = new RectangleGeometry(factory, new RectangleF(x, y, width, height));
            renderTarget.FillGeometry(rectangleGeometry, brush, null);
        }
    }

    public class Snake : Drawable {
        public Queue<Point> trail = new Queue<Point>();
        public int tail = Constants.INITIAL_TAIL;
        public int dx;
        public int dy;

        public bool CheckCollision(int x, int y) {
            foreach (var element in trail) {
                if(element.X == x && element.Y == y)
                    return true;
            }
            return false;
        }

        public void Update() {
            x = (x + dx + Constants.SCREEN_SIZE) % Constants.SCREEN_SIZE;
            y = (y + dy + Constants.SCREEN_SIZE) % Constants.SCREEN_SIZE;

            if(CheckCollision(x,y)){
                x = y = 10;
                dx = dy = 0;
                tail = Constants.INITIAL_TAIL;
            }

            trail.Enqueue(new Point { X = x, Y = y });
            while(trail.Count > tail){
                trail.Dequeue();
            }
        }

        public void Draw(RenderTarget renderTarget, SharpDX.Direct2D1.Factory factory) {
            foreach (var element in trail)
            {
                var x = element.X * Constants.SPRITE_SIZE + 1;
                var y = element.Y * Constants.SPRITE_SIZE + 1;
                var rectangleGeometry = new RectangleGeometry(factory, new RectangleF(x, y, width, height));
                renderTarget.FillGeometry(rectangleGeometry, brush, null);
            }
        }
    }

    public class SnakeGame : IDisposable
    {
        RenderForm form;
        RenderTarget d2dRenderTarget;
        SwapChain swapChain;
        Device device;
        Factory factory;
        Texture2D backBuffer;
        RenderTargetView renderView;
        SharpDX.Direct2D1.Factory d2dFactory;

        Random rng;
        Stopwatch stopwatch;

        Apple apple;
        Snake snake;
        bool sleeping = false;
        
        Keyboard keyboard;

        public SnakeGame()
        {
            form = new RenderForm("Snake");
            form.ClientSize = new System.Drawing.Size(Constants.SCREEN_SIZE * Constants.SPRITE_SIZE, Constants.SCREEN_SIZE * Constants.SPRITE_SIZE);            
            form.AllowUserResizing = false;
        }

        public void Dispose() {
            renderView.Dispose();
            backBuffer.Dispose();
            device.ImmediateContext.ClearState();
            device.ImmediateContext.Flush();
            device.Dispose();
            swapChain.Dispose();
            factory.Dispose();

            form.Dispose();
        }

        public void Run() {
            Initialize();
            RenderLoop.Run(form, RenderCallback);
        }

        private void RenderCallback() {
            if(sleeping) return;
            stopwatch = Stopwatch.StartNew();
            Update();
            Draw();
            stopwatch.Stop();
            var sleepTime = (int)(1000f/Constants.FRAMES_PER_SECOND - stopwatch.ElapsedMilliseconds);
            if(sleepTime > 0){
                sleeping = true;
                System.Threading.Thread.Sleep(sleepTime);
                sleeping = false;
            }
        }

        protected void Initialize()
        {
            apple = new Apple() { x = 3, y = 3 };
            snake = new Snake() { x = 10, y = 10, dy = 1 };
            
            rng = new Random();

            // SwapChain description
            var desc = new SwapChainDescription()
                           {
                               BufferCount = 1,
                               ModeDescription = 
                                   new ModeDescription(form.ClientSize.Width, form.ClientSize.Height,
                                                       new Rational(Constants.FRAMES_PER_SECOND, 1), Format.R8G8B8A8_UNorm),
                               IsWindowed = true,
                               OutputHandle = form.Handle,
                               SampleDescription = new SampleDescription(1, 0),
                               SwapEffect = SwapEffect.Discard,
                               Usage = Usage.RenderTargetOutput
                           };

            // Create Device and SwapChain
            Device.CreateWithSwapChain(DriverType.Hardware, DeviceCreationFlags.BgraSupport, new SharpDX.Direct3D.FeatureLevel[] { SharpDX.Direct3D.FeatureLevel.Level_10_0 }, desc, out device, out swapChain);

            d2dFactory = new SharpDX.Direct2D1.Factory();

            int width = form.ClientSize.Width;
            int height = form.ClientSize.Height;

            // Ignore all windows events
            factory = swapChain.GetParent<Factory>();
            factory.MakeWindowAssociation(form.Handle, WindowAssociationFlags.IgnoreAll);

            // New RenderTargetView from the backbuffer
            backBuffer = Texture2D.FromSwapChain<Texture2D>(swapChain, 0);
            renderView = new RenderTargetView(device, backBuffer);

            Surface surface = backBuffer.QueryInterface<Surface>();

            d2dRenderTarget = new RenderTarget(d2dFactory, surface, new RenderTargetProperties(new PixelFormat(Format.Unknown, AlphaMode.Premultiplied)));

            var rectangleGeometry = new RoundedRectangleGeometry(d2dFactory, new RoundedRectangle() { RadiusX = 32, RadiusY = 32, Rect = new RectangleF(128, 128, width - 128 * 2, height-128 * 2) });
            var solidColorBrush = new SolidColorBrush(d2dRenderTarget, Color.White);

            var directInput = new DirectInput();
            keyboard = new Keyboard(directInput);
            keyboard.Acquire();

            apple.Initialize(d2dRenderTarget, Color.Red);
            snake.Initialize(d2dRenderTarget, Color.Green);
        }

        private bool HandleInput() {
            var state = keyboard.GetCurrentState();
            if(snake.dx == 0){
                if(state.IsPressed(Key.Left)){
                    snake.dx = -1;
                    snake.dy = 0;
                } else if(state.IsPressed(Key.Right)){
                    snake.dx = 1;
                    snake.dy = 0;
                }
            }
            if(snake.dy == 0 ){
                if(state.IsPressed(Key.Up)){
                    snake.dx = 0;
                    snake.dy = -1;
                }
                if(state.IsPressed(Key.Down)){
                    snake.dx = 0;
                    snake.dy = 1;
                }
            }
            if(state.IsPressed(Key.Escape)){
                return true;
            } else {
                return false;
            }
        }

        void GenerateApple() {
            do {
                apple.x = rng.Next(Constants.SCREEN_SIZE);
                apple.y = rng.Next(Constants.SCREEN_SIZE);                
            } while(snake.CheckCollision(apple.x, apple.y));
        }

        protected void Update()
        {
            if(HandleInput()) {
                System.Diagnostics.Process.GetCurrentProcess().Kill();
            }

            snake.Update();
            
            if(snake.CheckCollision(apple.x, apple.y)){
                snake.tail++;
                GenerateApple();
            }
        }

        protected void Draw()
        {
            d2dRenderTarget.BeginDraw();
            d2dRenderTarget.Clear(Color.Black);
            apple.Draw(d2dRenderTarget, d2dFactory);
            snake.Draw(d2dRenderTarget, d2dFactory);
            d2dRenderTarget.EndDraw();
            swapChain.Present(0, PresentFlags.None);
        }
    }
}
