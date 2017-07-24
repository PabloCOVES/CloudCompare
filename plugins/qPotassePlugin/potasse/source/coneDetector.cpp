#include <potasse/shapeDetector/coneDetector.hpp>

#include <potasse/estimator/coneEstimator.hpp>

namespace potasse
{
  namespace shapeDetector
  {
    ConeDetector::ConeDetector(Parameter const &parameter)
      : Base{parameter}
    {
    }

    typename ConeDetector::Size
      ConeDetector::minimum() const
      {
        return 3;
      }

    typename ConeDetector::ShapePtr
      ConeDetector::shapePtr(VertexCloud const &vertexCloud,
          NormalCloud const &normalCloud,
          Indices const &inliers) const
      {
        typedef typename estimator::ConeEstimator::Cone Cone;
        return std::make_shared<Cone>(estimator::ConeEstimator::estimate(vertexCloud, normalCloud, inliers));
      }
  }
}
