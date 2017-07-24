#include <potasse/estimator/cylinderEstimator.hpp>

#include <potasse/estimator/mostOrthogonalVectorToNVectors.hpp>
#include <potasse/estimator/closestLineToNLinesGivenItsDirection.hpp>

namespace potasse
{
  namespace estimator
  {
    typename CylinderEstimator::Cylinder
      CylinderEstimator::estimate(VertexCloud const &vertexCloud,
          NormalCloud const &normalCloud,
          Indices const &inliers)
      {
        typedef typename ClosestLineToNLinesGivenItsDirection::Line Line;
        typedef typename ClosestLineToNLinesGivenItsDirection::Lines Lines;

        Normal direction{MostOrthogonalVectorToNVectors::estimate(normalCloud, inliers)};

        Lines lines{};
        for (auto const &index : inliers)
          lines.push_back(Line{vertexCloud[index], normalCloud[index]});
        Line line{ClosestLineToNLinesGivenItsDirection::estimate(lines, direction)};

        SCALAR radius{0};
        for (auto const &index : inliers)
          radius += line.distance(vertexCloud[index]);
        radius /= inliers.size();

        return Cylinder{line.vertex(), direction, radius};
      }
  }
}
