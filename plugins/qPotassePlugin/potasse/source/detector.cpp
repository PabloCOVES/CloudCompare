#include <potasse/detector/detector.hpp>

#include <potasse/detector/exception.hpp>

#include <potasse/shape/plane.hpp>

#include <fstream>

namespace potasse
{
  namespace detector
  {
    template <typename PointCloud, typename NormalCloud>
    void
    Detector<PointCloud, NormalCloud>::compute(PointCloud const &pointCloud, NormalCloud const &normalCloud)
    {
      if (pointCloud.size() != normalCloud.size()) throw Exception("Clouds must be of the same size.", __FILE__, __LINE__);
      if (!static_cast<bool>(graph_)) throw Exception("Invalid graph parameters", __FILE__, __LINE__);

      for (ShapeDetectorPtr shapeDetetectorPtr : shapeDetectorPtrVector_)
      {
        if (!static_cast<bool>(shapeDetetectorPtr)) throw Exception("Invalid shape detector pointer.", __FILE__, __LINE__);
        if (!static_cast<bool>(*shapeDetetectorPtr)) throw Exception("Invalid shape detector parameters.", __FILE__, __LINE__);
      }

      std::cout << "Graph computation:\t" << std::flush;
      graph_.compute(pointCloud);
      std::cout << "Done.\t" << graph_.vertexList().size() << std::endl;

      std::cout << "Shape detection:\t" << std::flush;
      std::size_t detected(0);
      for (typename Graph<PointCloud>::Vertex const &vertex : graph_.vertexList())
      {
        DetectedShapeList detectedShapeList;
        for (ShapeDetectorPtr const shapeDetectorPtr : shapeDetectorPtrVector_)
        {
          try
          {
            detectedShapeList.push_back(shapeDetectorPtr->compute(pointCloud, normalCloud, vertex));
          }
          catch (Exception &exception)
          {
//#ifndef NDEBUG
            std::cerr << "Detector error in file " << exception.file() << " at line " << exception.line() << ":\t" << exception.what() << std::endl;
//#endif
          }
        }
        detected += detectedShapeList.size();

        if (!detectedShapeList.empty())
        {
          detectedShapeList.sort([](DetectedShape const &lhs, DetectedShape const &rhs) { return lhs.error() < rhs.error(); });
          detectedShapeListMap_.insert(std::make_pair(&vertex, detectedShapeList));
        }
      }
      std::cout << "Done.\t" << detected << std::endl;

      std::ofstream planeStream("result/plane.xyz");
      std::ofstream sphereStream("result/sphere.xyz");
      std::ofstream cylinderStream("result/cylinder.xyz");
      for (auto const &foo : detectedShapeListMap_)
      {
        DetectedShapeList const &detectedShapeList(foo.second);
        if (!detectedShapeList.empty())
        {
          DetectedShape const &detectedShape(detectedShapeList.front());
          if (detectedShape.shapePtr()->type() == potasse::shape::Shape<double, 3>::Type::Plane)
          {
            for (auto const &i : detectedShape.inliers())
              planeStream << pointCloud[i] << std::endl;
            for (auto const &i : detectedShape.outliers())
              planeStream << pointCloud[i] << std::endl;
          }
          else if (detectedShape.shapePtr()->type() == potasse::shape::Shape<double, 3>::Type::Sphere)
          {
            for (auto const &i : detectedShape.inliers())
              sphereStream << pointCloud[i] << std::endl;
            for (auto const &i : detectedShape.outliers())
              sphereStream << pointCloud[i] << std::endl;
          }
          else if (detectedShape.shapePtr()->type() == potasse::shape::Shape<double, 3>::Type::Cylinder)
          {
            for (auto const &i : detectedShape.inliers())
              cylinderStream << pointCloud[i] << std::endl;
            for (auto const &i : detectedShape.outliers())
              cylinderStream << pointCloud[i] << std::endl;
          }
        }
      }
    }

    template <typename PointCloud, typename NormalCloud>
    Detector<PointCloud, NormalCloud>::Detector(Graph<PointCloud> const &graph,
                                                ShapeDetectorPtrVector const &shapeDetectorPtrVector)
        : graph_(graph), shapeDetectorPtrVector_(shapeDetectorPtrVector)
    {
    }

    template <typename PointCloud, typename NormalCloud>
    Graph<PointCloud> &
    Detector<PointCloud, NormalCloud>::graph()
    {
      return graph_;
    }

    template <typename PointCloud, typename NormalCloud>
    Graph<PointCloud> const &
    Detector<PointCloud, NormalCloud>::graph() const
    {
      return graph_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    Detector<PointCloud, NormalCloud>::graph(Graph<PointCloud> const &graph)
    {
      graph_ = graph;
    }

    template <typename PointCloud, typename NormalCloud>
    typename Detector<PointCloud, NormalCloud>::ShapeDetectorPtrVector &
    Detector<PointCloud, NormalCloud>::shapeDetectorPtrVector()
    {
      return shapeDetectorPtrVector_;
    }

    template <typename PointCloud, typename NormalCloud>
    typename Detector<PointCloud, NormalCloud>::ShapeDetectorPtrVector const &
    Detector<PointCloud, NormalCloud>::shapeDetectorPtrVector() const
    {
      return shapeDetectorPtrVector_;
    }

    template <typename PointCloud, typename NormalCloud>
    void
    Detector<PointCloud, NormalCloud>::shapeDetectorPtrVector(ShapeDetectorPtrVector const &shapeDetectorPtrVector)
    {
      shapeDetectorPtrVector_ = shapeDetectorPtrVector;
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

template class potasse::detector::Detector<PointCloud, NormalCloud>;
