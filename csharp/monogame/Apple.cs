using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Snake 
{
    public class Apple {
        public int x;
        public int y;

        public void Draw(SpriteBatch spriteBatch, Texture2D texture) {
            var pos = new Vector2(
                x * Constants.SPRITE_SIZE,
                y * Constants.SPRITE_SIZE);
            spriteBatch.Draw(texture, pos, Color.Red);
        }
    }
}
