#ifndef _POTASSE_DETECTOR_DETECTOR_HPP_
#define _POTASSE_DETECTOR_DETECTOR_HPP_

#include <potasse/detector/graph.hpp>
#include <potasse/detector/shapeDetector.hpp>

#include <map>
#include <memory>
#include <vector>

namespace potasse
{
  namespace detector
  {
    template <typename PointCloud, typename NormalCloud>
    class Detector
    {
     public:
      typedef std::shared_ptr<ShapeDetector<PointCloud, NormalCloud>> ShapeDetectorPtr;
      typedef std::vector<ShapeDetectorPtr> ShapeDetectorPtrVector;

      explicit Detector(Graph<PointCloud> const &graph = Graph<PointCloud>(),
                        ShapeDetectorPtrVector const &shapeDetectorPtrVector = ShapeDetectorPtrVector());

      void compute(PointCloud const &pointCloud, NormalCloud const &normalCloud);

      Graph<PointCloud> &graph();
      Graph<PointCloud> const &graph() const;
      void graph(Graph<PointCloud> const &graph);

      ShapeDetectorPtrVector &shapeDetectorPtrVector();
      ShapeDetectorPtrVector const &shapeDetectorPtrVector() const;
      void shapeDetectorPtrVector(ShapeDetectorPtrVector const &shapeDetectorPtrVector);

     protected:
     private:
      typedef typename Graph<PointCloud>::Vertex Vertex;
      typedef typename ShapeDetector<PointCloud, NormalCloud>::Detected DetectedShape;
      typedef std::list<DetectedShape> DetectedShapeList;
      typedef std::map<Vertex const *const, DetectedShapeList> DetectedShapeListMap;

      Graph<PointCloud> graph_;
      ShapeDetectorPtrVector shapeDetectorPtrVector_;

      DetectedShapeListMap detectedShapeListMap_;
      
    };
  }
}

#endif  // _POTASSE_DETECTOR_DETECTOR_HPP_
