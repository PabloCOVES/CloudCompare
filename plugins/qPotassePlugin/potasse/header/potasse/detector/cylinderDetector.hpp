#ifndef _POTASSE_DETECTOR_CYLINDERDETECTOR_HPP_
#define _POTASSE_DETECTOR_CYLINDERDETECTOR_HPP_

#include <potasse/detector/shapeDetector.hpp>

namespace potasse
{
  namespace detector
  {
    template <typename PointCloud, typename NormalCloud>
    class CylinderDetector : public ShapeDetector<PointCloud, NormalCloud>
    {
     public:
      typedef typename ShapeDetector<PointCloud, NormalCloud>::Parameter Parameter;
      typedef typename ShapeDetector<PointCloud, NormalCloud>::TreeParameter TreeParameter;

      explicit CylinderDetector(Parameter const &parameter = Parameter(),
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
    };
  }
}

#endif  //_POTASSE_DETECTOR_CYLINDERDETECTOR_HPP_
