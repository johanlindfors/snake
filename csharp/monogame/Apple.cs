using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Snake 
{
    public class Apple {
        private int x;
        private int y;

        public int X => x;
        public int Y => y;

        public void Move(int x, int y) {
            this.x = x;
            this.y = y;
        }

        public void Draw(SpriteBatch spriteBatch, Texture2D texture) {
            var pos = new Vector2(
                x * Constants.SPRITE_SIZE,
                y * Constants.SPRITE_SIZE);
            spriteBatch.Draw(texture, pos, Color.Red);
        }
    }
}
