#ifndef _CELL_H_
#define _CELL_H_

#include <memory>
#include <vector>

#include <Eigen/Dense>

namespace CCLib {
class ReferenceCloud;
}

class ccPlane;
class ccCylinder;

class Cell {
   public:
    typedef std::shared_ptr<ccPlane> PlanePtr;
    typedef std::shared_ptr<ccPlane const> PlaneConstPtr;

    typedef std::shared_ptr<ccCylinder> CylinderPtr;
    typedef std::shared_ptr<ccCylinder const> CylinderConstPtr;

   public:
    explicit Cell(unsigned int index, unsigned int level_, float maxDist,
                  float maxAngle);

    void operator()(CCLib::ReferenceCloud const &referenceCloud,
                    std::vector<Eigen::Vector3f> const &vertices,
                    std::vector<Eigen::Vector3f> const &normals, bool plane,
                    bool cylinder);

    PlanePtr planePtr() { return planePtr_; }
    PlaneConstPtr planePtr() const { return planePtr_; }

    CylinderPtr cylinderPtr() { return cylinderPtr_; }
    CylinderConstPtr cylinderPtr() const { return cylinderPtr_; }

   protected:
   private:
    unsigned int index_;
    unsigned int level_;

    float maxDist_;
    float maxAngle_;

    PlanePtr planePtr_;
    CylinderPtr cylinderPtr_;
};

#endif  //_CELL_H_
