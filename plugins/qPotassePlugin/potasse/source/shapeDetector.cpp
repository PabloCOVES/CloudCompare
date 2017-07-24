#include <potasse/detector/shapeDetector.hpp>

#include <potasse/detector/exception.hpp>

#include <chrono>
#include <random>

namespace potasse
{
  namespace detector
  {
    // ShapeDetector
    template <typename PointCloud, typename NormalCloud>
    ShapeDetector<PointCloud, NormalCloud>::ShapeDetector(Parameter const &parameter, TreeParameter const &treeParameter)
        : parameter_(parameter), pointCloudTree_(treeParameter)
    {
    }

    template <typename PointCloud, typename NormalCloud>
    ShapeDetector<PointCloud, NormalCloud>::operator bool() const
    {
      return static_cast<bool>(parameter_) && static_cast<bool>(pointCloudTree_);
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Detected
    ShapeDetector<PointCloud, NormalCloud>::compute(PointCloud const &pointCloud,
                                                    NormalCloud const &normalCloud,
                                                    Vertex const &vertex) const
    {
      Indices const &indices(vertex.indices());

      if (indices.size() < parameter_.cardinal())
        throw Exception("Not enough available points.", __FILE__, __LINE__);

      std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
      std::uniform_int_distribution<size_t> distribution(0, indices.size() - 1);

      Detected output;
      for (std::size_t tries(0); tries < parameter_.tries(); ++tries)
      {
        Indices support;
        while (support.size() < minimum())
        {
          typename Indices::const_iterator it(indices.begin());
          std::size_t id(distribution(generator));
          while (id--) ++it;
          if (std::find(support.begin(), support.end(), *it) == support.end())
            support.push_back(*it);
        }

        Detected detected(create(pointCloud, normalCloud, support));
        for (size_t const &indice : indices)
        {
          Scalar distance(detected.shapePtr()->distance(pointCloud[indice].point()));
          Scalar angle(detected.shapePtr()->angle(pointCloud[indice].point(), normalCloud[indice].normal()));

          if (distance < parameter_.distance() && angle < parameter_.angle())
            detected.inliers().push_back(indice);
          else
            detected.outliers().push_back(indice);
        }

        if (detected.inliers().size() < parameter_.cardinal())
          continue;

        detected.shapePtr(create(pointCloud, normalCloud, detected.inliers()));
        detected.error(error(pointCloud, detected.shapePtr(), detected.inliers(), vertex.box()));

        if (!static_cast<bool>(output.shapePtr()) || detected.error() < output.error())
          std::swap(output, detected);

      }
      if (!static_cast<bool>(output.shapePtr()))
        throw Exception("Invalid detected shape.", __FILE__, __LINE__);

      return output;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Scalar
    ShapeDetector<PointCloud, NormalCloud>::error(PointCloud const &pointCloud,
                                                  ShapePtr const shapePtr,
                                                  Indices const &inliers,
                                                  Box const &box) const
    {
      return error(pointCloud, shapePtr, inliers) / error(pointCloud, inliers, box);
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Scalar
    ShapeDetector<PointCloud, NormalCloud>::error(PointCloud const &pointCloud,
                                                  ShapePtr const shapePtr,
                                                  Indices const &inliers) const
    {
      Scalar output(0);
      for (typename Indices::value_type const &inlier : inliers)
        output += std::pow(shapePtr->distance(pointCloud[inlier].point()), 2);
      return output / inliers.size();
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Scalar
    ShapeDetector<PointCloud, NormalCloud>::error(PointCloud const &pointCloud,
                                                  Indices const &inliers,
                                                  Box const &box) const
    {
      PointCloudTree tree(pointCloudTree_.parameter());
      tree.compute(pointCloud, inliers, box);
      return tree.volume() / box.volume();
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Parameter &
    ShapeDetector<PointCloud, NormalCloud>::parameter()
    {
      return parameter_;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Parameter const &
    ShapeDetector<PointCloud, NormalCloud>::parameter() const
    {
      return parameter_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    ShapeDetector<PointCloud, NormalCloud>::parameter(Parameter const &parameter)
    {
      parameter_ = parameter;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::PointCloudTree &
    ShapeDetector<PointCloud, NormalCloud>::pointCloudTree()
    {
      return pointCloudTree_;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::PointCloudTree const &
    ShapeDetector<PointCloud, NormalCloud>::pointCloudTree() const
    {
      return pointCloudTree_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    ShapeDetector<PointCloud, NormalCloud>::pointCloudTree(PointCloudTree const &pointCloudTree)
    {
      pointCloudTree_ = pointCloudTree;
    }

    // ShapeDetector Parameter
    template <typename PointCloud, typename NormalCloud>
    ShapeDetector<PointCloud, NormalCloud>::Parameter::Parameter(Scalar distance,
                                                                 Scalar angle,
                                                                 std::size_t tries,
                                                                 std::size_t cardinal)
        : distance_(distance), angle_(angle), tries_(tries), cardinal_(cardinal)
    {
    }

    template <typename PointCloud, typename NormalCloud>
    ShapeDetector<PointCloud, NormalCloud>::Parameter::operator bool() const
    {
      return distance_ > 0 && angle_ > 0 && tries_ && cardinal_;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Scalar &
    ShapeDetector<PointCloud, NormalCloud>::Parameter::distance()
    {
      return distance_;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Scalar const &
    ShapeDetector<PointCloud, NormalCloud>::Parameter::distance() const
    {
      return distance_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    ShapeDetector<PointCloud, NormalCloud>::Parameter::distance(Scalar const &distance)
    {
      distance_ = distance;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Scalar &
    ShapeDetector<PointCloud, NormalCloud>::Parameter::angle()
    {
      return angle_;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Scalar const &
    ShapeDetector<PointCloud, NormalCloud>::Parameter::angle() const
    {
      return angle_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    ShapeDetector<PointCloud, NormalCloud>::Parameter::angle(Scalar const &angle)
    {
      angle_ = angle;
    }

    template <typename PointCloud, typename NormalCloud>
    std::size_t &
    ShapeDetector<PointCloud, NormalCloud>::Parameter::tries()
    {
      return tries_;
    }

    template <typename PointCloud, typename NormalCloud>
    std::size_t const &
    ShapeDetector<PointCloud, NormalCloud>::Parameter::tries() const
    {
      return tries_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    ShapeDetector<PointCloud, NormalCloud>::Parameter::tries(std::size_t const &tries)
    {
      tries_ = tries;
    }

    template <typename PointCloud, typename NormalCloud>
    std::size_t &
    ShapeDetector<PointCloud, NormalCloud>::Parameter::cardinal()
    {
      return cardinal_;
    }

    template <typename PointCloud, typename NormalCloud>
    std::size_t const &
    ShapeDetector<PointCloud, NormalCloud>::Parameter::cardinal() const
    {
      return cardinal_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    ShapeDetector<PointCloud, NormalCloud>::Parameter::cardinal(std::size_t const &cardinal)
    {
      cardinal_ = cardinal;
    }

    // ShapeDetector Detected
    template <typename PointCloud, typename NormalCloud>
    ShapeDetector<PointCloud, NormalCloud>::Detected::Detected(ShapePtr shapePtr,
                                                               Indices const &inliers,
                                                               Indices const &outliers,
                                                               Scalar const &error)
        : shapePtr_(shapePtr), inliers_(inliers), outliers_(outliers), error_(error)
    {
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::ShapePtr
    ShapeDetector<PointCloud, NormalCloud>::Detected::shapePtr()
    {
      return shapePtr_;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::ShapePtr const
    ShapeDetector<PointCloud, NormalCloud>::Detected::shapePtr() const
    {
      return shapePtr_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    ShapeDetector<PointCloud, NormalCloud>::Detected::shapePtr(ShapePtr const shapePtr)
    {
      shapePtr_ = shapePtr;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Indices &
    ShapeDetector<PointCloud, NormalCloud>::Detected::inliers()
    {
      return inliers_;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Indices const &
    ShapeDetector<PointCloud, NormalCloud>::Detected::inliers() const
    {
      return inliers_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    ShapeDetector<PointCloud, NormalCloud>::Detected::inliers(Indices const &inliers)
    {
      inliers_ = inliers;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Indices &
    ShapeDetector<PointCloud, NormalCloud>::Detected::outliers()
    {
      return outliers_;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Indices const &
    ShapeDetector<PointCloud, NormalCloud>::Detected::outliers() const
    {
      return outliers_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    ShapeDetector<PointCloud, NormalCloud>::Detected::outliers(Indices const &outliers)
    {
      outliers_ = outliers;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Scalar &
    ShapeDetector<PointCloud, NormalCloud>::Detected::error()
    {
      return error_;
    }

    template <typename PointCloud, typename NormalCloud>
    typename ShapeDetector<PointCloud, NormalCloud>::Scalar const &
    ShapeDetector<PointCloud, NormalCloud>::Detected::error() const
    {
      return error_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    ShapeDetector<PointCloud, NormalCloud>::Detected::error(Scalar const &error)
    {
      error_ = error;
    }
  }
}

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/point.hpp>

typedef potasse::cloud::Point<double, 3> Point;
typedef potasse::cloud::Normal<double, 3> Normal;

typedef potasse::cloud::Cloud<Point> PointCloud;
typedef potasse::cloud::Cloud<Normal> NormalCloud;

template class potasse::detector::ShapeDetector<PointCloud, NormalCloud>;
