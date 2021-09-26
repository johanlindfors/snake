using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Collections.Generic;

namespace Snake
{
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
            graphics.PreferredBackBufferHeight = Constants.SPRITE_SIZE * Constants.SCREEN_HEIGHT;
            graphics.PreferredBackBufferWidth = Constants.SPRITE_SIZE * Constants.SCREEN_WIDTH;
            graphics.ApplyChanges();

            apple = new Apple();
            snake = new Snake();

            rng = new Random();

            GenerateApple();

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
            if(snake.DX == 0) {
                if(state.IsKeyDown(Keys.Left)) {
                    snake.Face(-1, 0);
                }
                if(state.IsKeyDown(Keys.Right)) {
                    snake.Face(1, 0);
                }
            }
            if(snake.DY == 0 ) {
                if(state.IsKeyDown(Keys.Up)) {
                    snake.Face(0, -1);
                }
                if(state.IsKeyDown(Keys.Down)) {
                    snake.Face(0, 1);
                }
            }
        }

        void GenerateApple() {
            do {
                apple.Move(
                    rng.Next(Constants.SCREEN_WIDTH),
                    rng.Next(Constants.SCREEN_HEIGHT)
                );
            } while(snake.CheckCollision(apple.X, apple.Y));
        }

        protected override void Update(GameTime gameTime) {
            HandleInput();

            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            snake.Update();

            if(snake.CheckCollision(apple.X, apple.Y)) {
                snake.Eat();
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
