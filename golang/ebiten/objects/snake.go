package objects

import (
	"image/color"
	"os"
	"snake/constants"

	"github.com/hajimehoshi/ebiten/v2"
	"github.com/hajimehoshi/ebiten/v2/ebitenutil"
	"github.com/hajimehoshi/ebiten/v2/inpututil"
)

type Snake struct {
	position Vector
	velocity Vector
	width    float64
	height   float64
	color    color.Color
	tail     int
	trail    []Vector
	keys     []ebiten.Key
}

func (s *Snake) CheckCollision(position Vector) bool {
	for i := 0; i < len(s.trail); i++ {
		if s.trail[i].X == position.X && s.trail[i].Y == position.Y {
			return true
		}
	}
	return false
}

func (s *Snake) HandleInput() {
	s.keys = inpututil.AppendPressedKeys(s.keys[:0])
	for _, p := range s.keys {
		if p == ebiten.KeyArrowLeft && s.velocity.X == 0 {
			s.velocity.X = -1
			s.velocity.Y = 0
		}
		if p == ebiten.KeyArrowRight && s.velocity.X == 0 {
			s.velocity.X = 1
			s.velocity.Y = 0
		}
		if p == ebiten.KeyArrowDown && s.velocity.Y == 0 {
			s.velocity.X = 0
			s.velocity.Y = 1
		}
		if p == ebiten.KeyArrowUp && s.velocity.Y == 0 {
			s.velocity.X = 0
			s.velocity.Y = -1
		}
		if p == ebiten.KeyEscape {
			os.Exit(0)
		}
	}
}

func (s *Snake) Draw(screen *ebiten.Image) {
	for i := 0; i < len(s.trail); i++ {
		ebitenutil.DrawRect(
			screen,
			float64(s.trail[i].X*constants.SpriteSize),
			float64(s.trail[i].Y*constants.SpriteSize),
			s.width,
			s.height,
			s.color)
	}
}

func (s *Snake) move() {
	s.position.X = (s.position.X + s.velocity.X + constants.ScreenSize) % constants.ScreenSize
	s.position.Y = (s.position.Y + s.velocity.Y + constants.ScreenSize) % constants.ScreenSize
}

func (s *Snake) reset() {
	s.position = Vector{constants.ScreenSize / 2, constants.ScreenSize / 2}
	s.velocity = Vector{0, 0}
	s.tail = constants.InitialTail
}

func (s *Snake) trimTrail() {
	s.trail = append(s.trail, s.position)
	if len(s.trail) > s.tail {
		ret := make([]Vector, 0)
		s.trail = append(ret, s.trail[1:]...)
	}
}

func (s *Snake) Update() {
	s.move()
	if s.CheckCollision(s.position) {
		s.reset()
	}
	s.trimTrail()
}
