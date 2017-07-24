#ifndef _POTASSE_DETECTOR_SHAPEDETECTOR_HPP_
#define _POTASSE_DETECTOR_SHAPEDETECTOR_HPP_

#include <potasse/detector/graph.hpp>
#include <potasse/detector/tree.hpp>

#include <potasse/shape/shape.hpp>

#include <limits>
#include <memory>

namespace potasse
{
  namespace detector
  {
    template <typename PointCloud, typename NormalCloud>
    class ShapeDetector
    {
     public:
      class Parameter;
      class Detected;

      typedef typename PointCloud::value_type::value_type Scalar;
      typedef typename Graph<PointCloud>::Vertex Vertex;
      typedef typename Vertex::Indices Indices;
      typedef typename Vertex::Box Box;

      typedef shape::Shape<Scalar, std::tuple_size<typename PointCloud::value_type>::value> Shape;
      typedef std::shared_ptr<Shape> ShapePtr;

      typedef Tree<PointCloud> PointCloudTree;
      typedef typename PointCloudTree::Parameter TreeParameter;

      explicit ShapeDetector(Parameter const &parameter = Parameter(), TreeParameter const &treeParameter = TreeParameter());

      explicit operator bool() const;

      Detected compute(PointCloud const &pointCloud,
                       NormalCloud const &normalCloud,
                       Vertex const &vertex) const;

      Parameter &parameter();
      Parameter const &parameter() const;
      void parameter(Parameter const &parameter);

      PointCloudTree &pointCloudTree();
      PointCloudTree const &pointCloudTree() const;
      void pointCloudTree(PointCloudTree const &pointCloudTree);

     protected:
     private:
      Parameter parameter_;
      PointCloudTree pointCloudTree_;

      virtual std::size_t minimum() const = 0;

      virtual ShapePtr create(PointCloud const &pointCloud,
                              NormalCloud const &normalCloud,
                              Indices const &support) const = 0;

      Scalar error(PointCloud const &pointCloud,
                   ShapePtr const shapePtr,
                   Indices const &inliers,
                   Box const &box) const;

      Scalar error(PointCloud const &pointCloud,
                   ShapePtr const shapePtr,
                   Indices const &inliers) const;

      Scalar error(PointCloud const &pointCloud,
                   Indices const &inliers,
                   Box const &box) const;

     public:
      class Parameter
      {
       public:
        explicit Parameter(Scalar distance = 0,
                           Scalar angle = 0,
                           std::size_t tries = 0,
                           std::size_t cardinal_ = 0);

        explicit operator bool() const;

        Scalar &distance();
        Scalar const &distance() const;
        void distance(Scalar const &distance);

        Scalar &angle();
        Scalar const &angle() const;
        void angle(Scalar const &angle);

        std::size_t &tries();
        std::size_t const &tries() const;
        void tries(std::size_t const &distance);

        std::size_t &cardinal();
        std::size_t const &cardinal() const;
        void cardinal(std::size_t const &distance);

       protected:
       private:
        Scalar distance_;
        Scalar angle_;
        std::size_t tries_;
        std::size_t cardinal_;
      };

      class Detected
      {
       public:
        explicit Detected(ShapePtr shapePtr = nullptr,
                          Indices const &inliers = Indices(),
                          Indices const &outliers = Indices(),
                          Scalar const &error_ = std::numeric_limits<Scalar>::max());

        ShapePtr shapePtr();
        ShapePtr const shapePtr() const;
        void shapePtr(ShapePtr const shapePtr);

        Indices &inliers();
        Indices const &inliers() const;
        void inliers(Indices const &inliers);

        Indices &outliers();
        Indices const &outliers() const;
        void outliers(Indices const &outliers);

        Scalar &error();
        Scalar const &error() const;
        void error(Scalar const &scalar);

       protected:
       private:
        ShapePtr shapePtr_;
        Indices inliers_;
        Indices outliers_;
        Scalar error_;
      };
    };
  }
}

#endif  //_POTASSE_DETECTOR_SHAPEDETECTOR_HPP_
