#include <chrono>
#include <random>

namespace shapeDetector
{
  static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

  template <std::size_t S>
  ShapeDetector<S>::Parameter::operator bool() const
  {
    return 0 <= distance_ &&
           0 <= angle_ &&
           0 < tries_ &&
           0 < cardinal_;
  }

  template <std::size_t S>
  ShapeDetector<S>::ShapeDetector(Parameter const &parameter)
      : parameter_{parameter}
  {
  }

  template <std::size_t S>
  typename ShapeDetector<S>::DetectedShape
  ShapeDetector<S>::detect(VertexCloud const &vertexCloud,
                              NormalCloud const &normalCloud,
                              Indices const &indices) const
  {
    DetectedShape detectedShape;

    if (!static_cast<bool>(parameter_) ||
        indices.size() < parameter_.cardinal_)
      return detectedShape;

    std::uniform_int_distribution<Size> distribution{0, indices.size() - 1};

    for (Size tries{0}; tries < parameter_.tries_; ++tries)
    {
      Indices support;
      do
      {
        auto it{indices.begin()};
        std::advance(it, distribution(generator));
        support.push_back(*it);
        support.sort();
        support.unique();
      } while (support.size() != ShapeDetector::minimum());

      ShapePtr shapePtr{ShapeDetector::shapePtr(vertexCloud, normalCloud, support)};

      Indices inliers;
      Indices outliers;
      for (auto const &index : indices)
      {
        Vertex const &vertex{vertexCloud[index]};
        Normal const &normal{normalCloud[index]};

        if (shapePtr->distance(vertex) < parameter_.distance_ &&
            shapePtr->angle(vertex, normal) < parameter_.angle_)
          inliers.push_back(index);
        else
          outliers.push_back(index);
      }

      if (inliers.size() < parameter_.cardinal_)
        continue;

      if (!static_cast<bool>(detectedShape.shapePtr_) ||
          detectedShape.inliers_.size() < inliers.size())
        detectedShape = DetectedShape{shapePtr, inliers, outliers};
    }

    return detectedShape;
  }
}
