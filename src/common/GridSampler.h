#ifndef __GRID_SAMPLER_H__
#define __GRID_SAMPLER_H__

#include <common/Counted.h>
#include <common/BitMatrix.h>
#include <common/PerspectiveTransform.h>

namespace qcv {
class GridSampler {
private:
  static GridSampler gridSampler;
  GridSampler();

public:
  Ref<BitMatrix> sampleGrid(Ref<BitMatrix> image, int dimension, Ref<PerspectiveTransform> transform);
  Ref<BitMatrix> sampleGrid(Ref<BitMatrix> image, int dimensionX, int dimensionY, Ref<PerspectiveTransform> transform);

  Ref<BitMatrix> sampleGrid(Ref<BitMatrix> image, int dimension, float p1ToX, float p1ToY, float p2ToX, float p2ToY,
                            float p3ToX, float p3ToY, float p4ToX, float p4ToY, float p1FromX, float p1FromY, float p2FromX,
                            float p2FromY, float p3FromX, float p3FromY, float p4FromX, float p4FromY);
  static void checkAndNudgePoints(Ref<BitMatrix> image, std::vector<float> &points);
  static GridSampler &getInstance();
};
}

#endif // __GRID_SAMPLER_H__
