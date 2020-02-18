package msdfgen

// #cgo linux LDFLAGS: -L. -lmsdfgen_core
// #cgo darwin LDFLAGS: -L. -lmsdfgen_core_darwin
// #include <stdlib.h>
// #include "msdfgen-cgo.h"
import "C"
import (
	"errors"
	"unsafe"
)

// MSDF .
type MSDF struct {
	Pixels  []byte
	Width   int
	Height  int
	PxRange float64
	Scale   float64
	Left    float64
	Right   float64
	Top     float64
	Bottom  float64
	X       float64
	Y       float64
}

// GenerateMSDFA .
func GenerateMSDFA(shapeDescription string, width int, height int, pxRange float64) (*MSDF, error) {
	strShapeDescription := C.CString(shapeDescription)
	pBitmap := C.malloc(C.ulong(width * height * 4))
	metrics := C.struct_Metrics{}
	if C.bridge_generateMSDFA(strShapeDescription, C.int(width), C.int(height), C.double(pxRange), pBitmap, &metrics) != 0 {
		C.free(unsafe.Pointer(strShapeDescription))
		return nil, errors.New("failed to generate MSDFA")
	}
	bitmap := C.GoBytes(pBitmap, C.int(width*height*4))
	C.free(unsafe.Pointer(strShapeDescription))
	C.free(unsafe.Pointer(pBitmap))
	return &MSDF{
		Pixels:  bitmap,
		Width:   width,
		Height:  height,
		PxRange: pxRange,
		Scale:   float64(metrics.scale),
		Left:    float64(metrics.left),
		Top:     float64(metrics.top),
		Right:   float64(metrics.right),
		Bottom:  float64(metrics.bottom),
		X:       float64(metrics.x),
		Y:       float64(metrics.y),
	}, nil
}
