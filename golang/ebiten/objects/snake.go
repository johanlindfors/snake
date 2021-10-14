package objects

import (
	"image/color"
	"snake/constants"

	"github.com/hajimehoshi/ebiten/v2"
	"github.com/hajimehoshi/ebiten/v2/ebitenutil"
)

type Snake struct {
	position Vector
	velocity Vector
	width    float64
	height   float64
	color    color.Color
	tail     int
	trail    []Vector
}

func (s *Snake) CheckCollision(position Vector) bool {
	for i := 0; i < len(s.trail); i++ {
		if s.trail[i].X == position.X && s.trail[i].Y == position.Y {
			return true
		}
	}
	return false
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

func (s *Snake) Update() {
	s.position.X = (s.position.X + s.velocity.X + constants.ScreenSize) % constants.ScreenSize
	s.position.Y = (s.position.Y + s.velocity.Y + constants.ScreenSize) % constants.ScreenSize

	if s.CheckCollision(s.position) {
		s.position = Vector{constants.ScreenSize / 2, constants.ScreenSize / 2}
		s.velocity = Vector{0, 0}
		s.tail = constants.InitialTail
	}

	s.trail = append(s.trail, s.position)
	for ok := len(s.trail) > s.tail; ok; ok = len(s.trail) > s.tail {
		ret := make([]Vector, 0)
		s.trail = append(ret, s.trail[1:]...)
	}
}
