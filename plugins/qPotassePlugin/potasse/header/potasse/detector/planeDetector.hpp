#ifndef _POTASSE_DETECTOR_PLANEDETECTOR_HPP_
#define _POTASSE_DETECTOR_PLANEDETECTOR_HPP_

#include <potasse/detector/shapeDetector.hpp>

namespace potasse
{
  namespace detector
  {
    template <typename PointCloud, typename NormalCloud>
    class PlaneDetector : public ShapeDetector<PointCloud, NormalCloud>
    {
     public:
      typedef typename ShapeDetector<PointCloud, NormalCloud>::Parameter Parameter;
      typedef typename ShapeDetector<PointCloud, NormalCloud>::TreeParameter TreeParameter;

      explicit PlaneDetector(Parameter const &parameter = Parameter(),
                             TreeParameter const &treeParameter = TreeParameter());

     protected:
     private:
      typedef typename ShapeDetector<PointCloud, NormalCloud>::Indices Indices;
      typedef typename ShapeDetector<PointCloud, NormalCloud>::Scalar Scalar;
      typedef typename ShapeDetector<PointCloud, NormalCloud>::ShapePtr ShapePtr;

      std::size_t minimum() const;

      ShapePtr create(PointCloud const &pointCloud,
                      NormalCloud const &normalCloud,
                      Indices const &support) const;

      ShapePtr create(PointCloud const &pointCloud,
                      Indices const &support) const;
    };
  }
}

#endif  //_POTASSE_DETECTOR_PLANEDETECTOR_HPP_
