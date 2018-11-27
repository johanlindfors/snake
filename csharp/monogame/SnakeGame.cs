using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Collections.Generic;

namespace WinFormTest
{
    public static class Constants {
        public const int SPRITE_SIZE = 20;
        public const int SCREEN_SIZE = 20;
        public const int FPS = 15;
        public const int INITIAL_TAIL = 5;
    }

    public class Apple {
        public int x = 3;
        public int y = 3;

        public void Draw(SpriteBatch spriteBatch, Texture2D texture) {
            var rect = new Rectangle(
                x * Constants.SPRITE_SIZE + 1,
                y * Constants.SPRITE_SIZE + 1,
                Constants.SPRITE_SIZE - 1,
                Constants.SPRITE_SIZE - 1);
            spriteBatch.Draw(texture, rect, Color.Red);
        }
    }

    public class Snake {
        public Queue<Point> trail = new Queue<Point>();
        public int tail = Constants.INITIAL_TAIL;
        public int x = 10;
        public int y = 10;
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
            x += dx;
            y += dy;

            x = x > Constants.SCREEN_SIZE -1 ? 0 : x < 0 ? Constants.SCREEN_SIZE - 1 : x;
            y = y > Constants.SCREEN_SIZE -1 ? 0 : y < 0 ? Constants.SCREEN_SIZE - 1 : y;

            if(CheckCollision(x,y)){
                x = y = 10;
                dx = dy = 0;
                tail = Constants.INITIAL_TAIL;
            }

            trail.Enqueue(new Point(x, y));
            while(trail.Count > tail){
                trail.Dequeue();
            }
        }

        public void Draw(SpriteBatch spriteBatch, Texture2D texture) {
            foreach (var element in trail)
            {
                var rect = new Rectangle(
                    element.X * Constants.SPRITE_SIZE + 1,
                    element.Y * Constants.SPRITE_SIZE + 1,
                    Constants.SPRITE_SIZE - 1,
                    Constants.SPRITE_SIZE - 1);
                spriteBatch.Draw(texture, rect, Color.Green);
            }
        }
    }

    public class SnakeGame : Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Texture2D rectTexture;
        Random rng;

        Apple apple;
        Snake snake;
        
        public SnakeGame()
        {
            graphics = new GraphicsDeviceManager(this);
            graphics.PreferredBackBufferHeight = Constants.SPRITE_SIZE * Constants.SCREEN_SIZE;
            graphics.PreferredBackBufferWidth = Constants.SPRITE_SIZE * Constants.SCREEN_SIZE;
            graphics.ApplyChanges();

            TargetElapsedTime = TimeSpan.FromMilliseconds(1000/Constants.FPS);
        }

        protected override void Initialize()
        {
            apple = new Apple();
            snake = new Snake();
            
            rng = new Random();

            base.Initialize();
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);

            var data = new Color[1];
            data[0] = Color.White;

            rectTexture = new Texture2D(GraphicsDevice, 1, 1);
            rectTexture.SetData(data);
        }

        private void HandleInput() {
            var state = Keyboard.GetState();
            if(snake.dx == 0){
                if(state.IsKeyDown(Keys.Left)){
                    snake.dx = -1;
                    snake.dy = 0;
                }
                if(state.IsKeyDown(Keys.Right)){
                    snake.dx = 1;
                    snake.dy = 0;
                }
            }
            if(snake.dy == 0 ){
                if(state.IsKeyDown(Keys.Up)){
                    snake.dx = 0;
                    snake.dy = -1;
                }
                if(state.IsKeyDown(Keys.Down)){
                    snake.dx = 0;
                    snake.dy = 1;
                }
            }
        }
        void GenerateApple() {
            do {
                apple.x = rng.Next(Constants.SCREEN_SIZE);
                apple.y = rng.Next(Constants.SCREEN_SIZE);                
            } while(snake.CheckCollision(apple.x, apple.y));
        }

        protected override void Update(GameTime gameTime)
        {
            HandleInput();

            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();                    

            snake.Update();
            
            if(snake.CheckCollision(apple.x, apple.y)){
                snake.tail++;
                GenerateApple();
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Black);

            var position = new Vector2(100,100);

            spriteBatch.Begin();
            apple.Draw(spriteBatch, rectTexture);
            snake.Draw(spriteBatch, rectTexture);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
