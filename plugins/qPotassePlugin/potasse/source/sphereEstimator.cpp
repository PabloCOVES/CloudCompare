#include <potasse/estimator/sphereEstimator.hpp>

#include <potasse/estimator/closestPointToNLines.hpp>

namespace potasse
{
  namespace estimator
  {
    typename SphereEstimator::Sphere
      SphereEstimator::estimate(VertexCloud const &vertexCloud,
          NormalCloud const &normalCloud,
          Indices const &inliers)
      {
        typedef ClosestPointToNLines ClosestPointToNLines;
        typedef typename ClosestPointToNLines::Line Line;
        typedef typename ClosestPointToNLines::Lines Lines;

        typedef typename Sphere::ConstMap ConstMap;

        Lines lines;
        for (auto const &index : inliers)
          lines.push_back(Line{vertexCloud[index], normalCloud[index]});

        Vertex center{ClosestPointToNLines::estimate(lines)};
        ConstMap centerMap{center.vertex()};

        SCALAR radius{0};
        for (auto const &index : inliers)
          radius += (ConstMap{vertexCloud[index].vertex()} - centerMap).norm();
        radius /= inliers.size();

        return Sphere{center, radius};
      }
  }
}
