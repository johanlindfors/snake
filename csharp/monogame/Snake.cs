using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Snake
{
    public class Snake {
        private int x;
        private int y;
        private int tail;
        private Queue<Point> trail = new Queue<Point>();

        private int dx;
        private int dy;

        public int DX => dx;
        public int DY => dy;

        public Snake() {
            Reset();
        }

        public bool CheckCollision(int x, int y) {
            foreach (var element in trail) {
                if(element.X == x && element.Y == y)
                    return true;
            }
            return false;
        }

        public void Eat() {
            tail++;
        }

        public void Face(int dx, int dy) {
            this.dx = dx;
            this.dy = dy;
        }

        public void Update() {
            x = (x + dx + Constants.SCREEN_SIZE) % Constants.SCREEN_SIZE;
            y = (y + dy + Constants.SCREEN_SIZE) % Constants.SCREEN_SIZE;

            if(CheckCollision(x,y)) {
                Reset();
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

        private void Reset() {
            x = y = Constants.SCREEN_SIZE/2;
            dx = dy = 0;
            tail = Constants.INITIAL_TAIL;
        }
    }
}
