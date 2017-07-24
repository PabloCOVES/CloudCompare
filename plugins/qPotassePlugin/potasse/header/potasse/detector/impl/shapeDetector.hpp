#include <potasse/detector/shapeDetector.hpp>

#include <potasse/detector/tree.hpp>

#include <set>

namespace potasse
{
  namespace detector
  {
    namespace util
    {
      //////////////////////////////////////////////////////////////////////////
      // ShapeDetector
      //////////////////////////////////////////////////////////////////////////
      template <typename PointCloud, typename NormalCloud>
        ShapeDetector<PointCloud, NormalCloud>::ShapeDetector(
            Parameter const &parameter)
        : parameter_(parameter)
        {
        }

      template <typename PointCloud, typename NormalCloud>
        typename ShapeDetector<PointCloud, NormalCloud>::Parameter &
        ShapeDetector<PointCloud, NormalCloud>::parameter()
        {
          if (static_cast<bool>(parameter_)) return parameter_;
          throw std::domain_error("Invalid parameters.");
        }

      template <typename PointCloud, typename NormalCloud>
        typename ShapeDetector<PointCloud, NormalCloud>::Parameter const &
        ShapeDetector<PointCloud, NormalCloud>::parameter() const
        {
          if (static_cast<bool>(parameter_)) return parameter_;
          throw std::domain_error("Invalid parameters.");
        }

      template <typename PointCloud, typename NormalCloud>
        void
        ShapeDetector<PointCloud, NormalCloud>::parameter(
            Parameter const &parameter)
        {
          parameter_ = parameter;
        }

      template <typename PointCloud, typename NormalCloud>
        void
        ShapeDetector<PointCloud, NormalCloud>::compute(
            PointCloud const &pointCloud,
            NormalCloud const &normalCloud,
            VertexConstPtr const vertexConstPtr)
        {
          Parameter const &parameter(this->parameter());

          if (pointCloud.size() != normalCloud.size())
            throw std::invalid_argument("Clouds must be of same size.");

          typedef typename Graph<PointCloud>::Indices Indices;
          Indices const &indices(vertexConstPtr->indices());
          if (indices.size() < parameter.cardinal_)
            throw std::invalid_argument("Not enough points to perform detection.");
        }

      //////////////////////////////////////////////////////////////////////////
      // Parameter
      //////////////////////////////////////////////////////////////////////////

      template <typename PointCloud, typename NormalCloud>
        ShapeDetector<PointCloud, NormalCloud>::Parameter::Parameter(
            Scalar angle, Scalar distance, size_t cardinal, size_t tries)
        : angle_(angle),
        distance_(distance),
        cardinal_(cardinal),
        tries_(tries)
      {
      }

      template <typename PointCloud, typename NormalCloud>
        ShapeDetector<PointCloud, NormalCloud>::Parameter::operator bool() const
        {
          return angle_ > 0 && distance_ > 0 && cardinal_ > 0 && tries_ > 0;
        }
    }

    template <typename PointCloud, typename NormalCloud>
      ShapeDetector<PointCloud, NormalCloud>::ShapeDetector(
          PointCloudConstPtr pointCloudConstPtr,
          NormalCloudConstPtr normalCloudConstPtr,
          Parameter const &parameter)
      : util::ShapeDetector<PointCloud, NormalCloud>(parameter),
      pointCloudConstWPtr_(pointCloudConstPtr),
      normalCloudConstWPtr_(normalCloudConstPtr)
    {
    }

    template <typename PointCloud, typename NormalCloud>
      typename ShapeDetector<PointCloud, NormalCloud>::PointCloudConstPtr const
      ShapeDetector<PointCloud, NormalCloud>::pointCloud() const
      {
        PointCloudConstPtr output(pointCloudConstWPtr_.lock());
        if (static_cast<bool>(output)) return output;
        throw std::bad_weak_ptr();
      }

    template <typename PointCloud, typename NormalCloud>
      void
      ShapeDetector<PointCloud, NormalCloud>::pointCloud(
          PointCloudConstPtr pointCloudConstPtr)
      {
        pointCloudConstWPtr_ = pointCloudConstPtr;
      }

    template <typename PointCloud, typename NormalCloud>
      typename ShapeDetector<PointCloud, NormalCloud>::NormalCloudConstPtr const
      ShapeDetector<PointCloud, NormalCloud>::normalCloud() const
      {
        NormalCloudConstPtr output(normalCloudConstWPtr_.lock());
        if (static_cast<bool>(output)) return output;
        throw std::bad_weak_ptr();
      }

    template <typename PointCloud, typename NormalCloud>
      void
      ShapeDetector<PointCloud, NormalCloud>::normalCloud(
          NormalCloudConstPtr normalCloudConstPtr)
      {
        normalCloudConstWPtr_ = normalCloudConstPtr;
      }
  }
}
