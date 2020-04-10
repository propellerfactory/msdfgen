#include "../msdfgen.h"
#include "../msdfgen-cgo.h"

#define LARGE_VALUE 1e240

using namespace msdfgen;

int bridge_generateMSDFA(const char* shapeDescription, int width, int height, double pxRange, 
    void* bitmap, struct Metrics *metrics) {
    Vector2 frame(width, height);
    double edgeThreshold = 1.001;
    double angleThreshold = 3.0;
    Vector2 scale = 1;
    Vector2 translate;
    double range = 1;
    double avgScale = .5*(scale.x+scale.y);
    bool overlapSupport = true;
    struct {
        double l, b, r, t;
    } bounds = {
        LARGE_VALUE, LARGE_VALUE, -LARGE_VALUE, -LARGE_VALUE
    };
    Shape shape;
    if( !readShapeDescription(shapeDescription, shape) ) {
        return 1;
    }
    shape.normalize();
    edgeColoringSimple(shape, angleThreshold);
    shape.bound(bounds.l, bounds.b, bounds.r, bounds.t);
    // Auto-frame
    double l = bounds.l, b = bounds.b, r = bounds.r, t = bounds.t;
    frame -= pxRange;
    if (l >= r || b >= t)
        l = 0, b = 0, r = 1, t = 1;
    //if (frame.x <= 0 || frame.y <= 0)
    //    ABORT("Cannot fit the specified pixel range.");
    Vector2 dims(r-l, t-b);
    if (dims.x*frame.y < dims.y*frame.x) {
        translate.set(.5*(frame.x/frame.y*dims.y-dims.x)-l, -b);
        scale = avgScale = frame.y/dims.y;
    } else {
        translate.set(-l, .5*(frame.y/frame.x*dims.x-dims.y)-b);
        scale = avgScale = frame.x/dims.x;
    }
    translate += .5*pxRange/scale;
    range = pxRange/min(scale.x, scale.y);

    metrics->left = bounds.l;
    metrics->top = bounds.t;
    metrics->right = bounds.r;
    metrics->bottom = bounds.b;
    metrics->scale = avgScale;
    metrics->x = translate.x;
    metrics->y = translate.y;

    // Compute output
    Bitmap<float, 1> sdf;
    sdf = Bitmap<float, 1>(width, height);
    generateSDF(sdf, shape, range, scale, translate, overlapSupport);

    Bitmap<float, 3> msdf;
    msdf = Bitmap<float, 3>(width, height);
    generateMSDF(msdf, shape, range, scale, translate, 0, overlapSupport);
    distanceSignCorrection(msdf, shape, scale, translate, FILL_NONZERO);
    msdfErrorCorrection(msdf, edgeThreshold/(scale*range));

    for (int y = 0; y < msdf.height(); ++y) {
        for (int x = 0; x < msdf.width(); ++x) {
            int offset = ((msdf.height() - y - 1) * msdf.width() + x) * 4;
            ((u_int8_t*)bitmap)[offset + 0] = (u_int8_t)pixelFloatToByte(msdf(x, y)[0]);
            ((u_int8_t*)bitmap)[offset + 1] = (u_int8_t)pixelFloatToByte(msdf(x, y)[1]);
            ((u_int8_t*)bitmap)[offset + 2] = (u_int8_t)pixelFloatToByte(msdf(x, y)[2]);
            ((u_int8_t*)bitmap)[offset + 3] = (u_int8_t)pixelFloatToByte(sdf(x, y)[0]);
        }
    }

    return 0;
}