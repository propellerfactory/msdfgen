#ifndef __msdfgen_cgo_
#define __msdfgen_cgo_

#ifdef __cplusplus
extern "C" {
#endif

struct Metrics {
    double left;
    double top;
    double right;
    double bottom;
    double scale;
    double x;
    double y;
};

int bridge_generateMSDFA(const char* shapeDescription, int width, int height, 
    double pxRange, void *bitmap, struct Metrics *metrics);

#ifdef __cplusplus
}
#endif

#endif