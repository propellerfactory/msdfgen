package msdfgen

import (
	"image"
	"image/color"
	"image/png"
	"os"
	"testing"
)

func TestMSDFGen(t *testing.T) {
	width := 64
	height := 64
	pxRange := 10.0
	shapeDefinition := "{ 169, 0; 169, 1456; 645, 1456; (882, 1456); 1001.5, 1358; (1121, 1260); 1121, 1068; (1121, 966); 1063, 887.5; (1005, 809); 905, 766; (1023, 733); 1091.5, 640.5; (1160, 548); 1160, 420; (1160, 224); 1033, 112; (906, 0); 674, 0; # } { 361, 681; 361, 157; 678, 157; (812, 157); 889.5, 226.5; (967, 296); 967, 418; (967, 681); 681, 681; # } { 361, 835; 651, 835; (777, 835); 852.5, 898; (928, 961); 928, 1069; (928, 1189); 858, 1243.5; (788, 1298); 645, 1298; 361, 1298; # }"
	msdf, err := GenerateMSDFA(shapeDefinition, width, height, pxRange)
	if err != nil {
		t.Fatal(err)
	}
	t.Logf("bounds: %f,%f,%f,%f", msdf.Left, msdf.Top, msdf.Right, msdf.Bottom)
	t.Logf("translate: %f,%f", msdf.X, msdf.Y)
	t.Logf("scale: %f", msdf.Scale)
	i := image.NewRGBA(image.Rect(0, 0, 64, 64))
	for y := 0; y < height; y++ {
		for x := 0; x < width; x++ {
			i.SetRGBA(x, y, color.RGBA{
				R: msdf.Pixels[(y*width+x)*4+0],
				G: msdf.Pixels[(y*width+x)*4+1],
				B: msdf.Pixels[(y*width+x)*4+2],
				A: msdf.Pixels[(y*width+x)*4+3],
			})
		}
	}
	out, err := os.Create("./test.png")
	if err != nil {
		t.Fatal(err)
	}
	if err := png.Encode(out, i); err != nil {
		t.Fatal(err)
	}
}
