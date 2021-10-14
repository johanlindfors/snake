package objects

import (
	"image/color"
	"snake/constants"

	"github.com/hajimehoshi/ebiten/v2"
	"github.com/hajimehoshi/ebiten/v2/ebitenutil"
)

type Snake struct {
	position Point
	width    float64
	height   float64
	color    color.Color
	tail     int
	dx       int
	dy       int
	trail    []Point
}

func (s *Snake) CheckCollision(position Point) bool {
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
	s.position.X = (s.position.X + s.dx + constants.ScreenSize) % constants.ScreenSize
	s.position.Y = (s.position.Y + s.dy + constants.ScreenSize) % constants.ScreenSize

	if s.CheckCollision(s.position) {
		s.position.X = constants.ScreenSize / 2
		s.position.Y = constants.ScreenSize / 2
		s.dx = 0
		s.dy = 0
		s.tail = constants.InitialTail
	}

	s.trail = append(s.trail, s.position)
	for ok := len(s.trail) > s.tail; ok; ok = len(s.trail) > s.tail {
		ret := make([]Point, 0)
		s.trail = append(ret, s.trail[1:]...)
	}
}
