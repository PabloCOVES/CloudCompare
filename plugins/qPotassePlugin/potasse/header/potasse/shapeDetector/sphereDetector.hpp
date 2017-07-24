#ifndef _SHAPEDETECTOR_SPHEREDETECTOR_HPP_
#define _SHAPEDETECTOR_SPHEREDETECTOR_HPP_

#include <shapeDetector/shapeDetector.hpp>

#include <estimator/sphereEstimator.hpp>

namespace shapeDetector
{
  class SphereDetector
      : ShapeDetector<3>
  {
   public:
    typedef ShapeDetector<3> Base;

    typedef typename Base::Vertex Vertex;
    typedef typename Base::Normal Normal;

    typedef typename Base::VertexCloud VertexCloud;
    typedef typename Base::NormalCloud NormalCloud;

    typedef typename Base::Size Size;

    typedef typename Base::Indices Indices;

    typedef typename Base::Parameter Parameter;

    explicit SphereDetector(Parameter const &parameter);

   protected:
    typedef typename Base::ShapePtr ShapePtr;

    Size minimum() const;
    ShapePtr shapePtr(VertexCloud const &vertexCloud,
                      NormalCloud const &normalCloud,
                      Indices const &inliers) const;

   private:
  };
}

#endif  //_SHAPEDETECTOR_SPHEREDETECTOR_HPP_
