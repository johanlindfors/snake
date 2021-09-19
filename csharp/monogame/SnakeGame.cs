using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Collections.Generic;

namespace Snake
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
            var pos = new Vector2(
                x * Constants.SPRITE_SIZE,
                y * Constants.SPRITE_SIZE);
            spriteBatch.Draw(texture, pos, Color.Red);
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
            x = (x + dx + Constants.SCREEN_SIZE) % Constants.SCREEN_SIZE;
            y = (y + dy + Constants.SCREEN_SIZE) % Constants.SCREEN_SIZE;

            if(CheckCollision(x,y)) {
                x = y = 10;
                dx = dy = 0;
                tail = Constants.INITIAL_TAIL;
            }

            trail.Enqueue(new Point(x, y));
            while(trail.Count > tail)
                trail.Dequeue();
        }

        public void Draw(SpriteBatch spriteBatch, Texture2D texture) {
            foreach (var element in trail) {
                var pos = new Vector2(
                    element.X * Constants.SPRITE_SIZE,
                    element.Y * Constants.SPRITE_SIZE);
                spriteBatch.Draw(texture, pos, Color.Green);
            }
        }
    }

    public class SnakeGame : Game {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Texture2D rectTexture;
        Random rng;

        Apple apple;
        Snake snake;

        public SnakeGame() {
            graphics = new GraphicsDeviceManager(this);

            TargetElapsedTime = TimeSpan.FromMilliseconds(1000/Constants.FPS);
        }

        protected override void Initialize() {
            graphics.PreferredBackBufferHeight = Constants.SPRITE_SIZE * Constants.SCREEN_SIZE;
            graphics.PreferredBackBufferWidth = Constants.SPRITE_SIZE * Constants.SCREEN_SIZE;
            graphics.ApplyChanges();

            apple = new Apple();
            snake = new Snake();

            rng = new Random();

            base.Initialize();
        }

        private Texture2D GenerateTexture(int width, int height) {
            var data = new Color[width*height];
            for(int i=0; i<width*height;i++)
                data[i] = Color.White;

            var texture = new Texture2D(GraphicsDevice, width, height);
            texture.SetData(data);
            return texture;
        }

        protected override void LoadContent() {
            spriteBatch = new SpriteBatch(GraphicsDevice);
            rectTexture = GenerateTexture(Constants.SPRITE_SIZE - 1, Constants.SPRITE_SIZE - 1);
        }

        private void HandleInput() {
            var state = Keyboard.GetState();
            if(snake.dx == 0) {
                if(state.IsKeyDown(Keys.Left)) {
                    snake.dx = -1;
                    snake.dy = 0;
                }
                if(state.IsKeyDown(Keys.Right)) {
                    snake.dx = 1;
                    snake.dy = 0;
                }
            }
            if(snake.dy == 0 ) {
                if(state.IsKeyDown(Keys.Up)) {
                    snake.dx = 0;
                    snake.dy = -1;
                }
                if(state.IsKeyDown(Keys.Down)) {
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

        protected override void Update(GameTime gameTime) {
            HandleInput();

            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            snake.Update();

            if(snake.CheckCollision(apple.x, apple.y)) {
                snake.tail++;
                GenerateApple();
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime) {
            GraphicsDevice.Clear(Color.Black);

            var position = new Vector2(100,100);

            spriteBatch.Begin();
            apple.Draw(spriteBatch, rectTexture);
            snake.Draw(spriteBatch, rectTexture);
            spriteBatch.End();

            base.Draw(gameTime);
        }

        static void Main() {
            using (var game = new SnakeGame())
                game.Run();
        }
    }
}
