#ifndef _SHAPEDETECTOR_PLANEDETECTOR_HPP_
#define _SHAPEDETECTOR_PLANEDETECTOR_HPP_

#include <potasse/shapeDetector/shapeDetector.hpp>

#include <potasse/estimator/planeEstimator.hpp>

namespace potasse
{
  namespace shapeDetector
  {
    class PlaneDetector
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

        explicit PlaneDetector(Parameter const &parameter);

      protected:
        typedef typename Base::ShapePtr ShapePtr;

        Size minimum() const;
        ShapePtr shapePtr(VertexCloud const &vertexCloud,
            NormalCloud const &normalCloud,
            Indices const &inliers) const;

      private:
    };
  }
}

#endif  //_SHAPEDETECTOR_PLANEDETECTOR_HPP_
