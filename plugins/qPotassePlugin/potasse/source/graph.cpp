#include <potasse/detector/graph.hpp>

#include <numeric>

template <typename Parameter, typename Vertex>
static bool
isVertexBigEnoughToBeSplitted(
    Parameter const &parameter,
    Vertex const &vertex)
{
  bool output(parameter.cardinal() < vertex.indices().size());
  for (size_t i(0); output && i < parameter.dimensions().size(); ++i)
    output &= vertex.boundingBox().dimension(i) > parameter.dimensions()[i];
  return output;
}

namespace potasse
{
  namespace detector
  {
    // Graph
    template <typename VertexCloud>
    Graph<VertexCloud>::Graph(Parameter const &parameter)
        : parameter_(parameter)
    {
    }

    template <typename VertexCloud>
    Graph<VertexCloud>::operator bool() const
    {
      return static_cast<bool>(parameter_);
    }

    template <typename VertexCloud>
    void
    Graph<VertexCloud>::compute(VertexCloud const &vertexCloud)
    {
      vertexList_.clear();

      potasse::cloud::BoundingBox<VertexCloud> boundingBox(vertexCloud.begin(), vertexCloud.end());
      std::list<size_t> indices(vertexCloud.size());
      std::iota(indices.begin(), indices.end(), 0);

      vertexList_.push_back(Vertex(boundingBox, indices));

      for (typename VertexList::iterator it(vertexList_.begin()); it != vertexList_.end();)
      {
        Vertex &vertex(*it);
        if (!isVertexBigEnoughToBeSplitted(parameter_, vertex))
        {
          ++it;
          continue;
        }

        typedef typename VertexCloud::value_type Vertex;
        typedef typename Vertex::Indices Indices;

        static const size_t dimension(std::tuple_size<Vertex>::value);
        static const size_t size(1 << dimension);

        Vertex const center(vertex.boundingBox().center());
        Indices &indices(vertex.indices());

        std::array<Indices, size> indicesArray;
        while (!indices.empty())
        {
          Vertex const &vertex(vertexCloud[indices.front()]);

          size_t id(0);
          for (size_t i(0); i < dimension; ++i)
            if (vertex.vertex()[i] > center.vertex()[i])
              id += (size >> (1 + i));

          indicesArray[id].splice(indicesArray[id].end(), indices, indices.begin());
        }

        VertexList vertexList;
        for (size_t i(0); i < size; ++i)
        {
          if (indicesArray[i].empty())
            continue;

          Vertex const &vertex(vertexCloud[indicesArray[i].front()]);
          Vertex min(vertex.boundingBox().min()), max(vertex.boundingBox().max());

          for (size_t j(0); j < std::tuple_size<Vertex>::value; ++j)
            if (vertex[j] < center[j])
              max[j] = center[j];
            else
              min[j] = center[j];

          vertexList.push_back(Vertex(typename Vertex::BoundingBox(min, max), indicesArray[i]));
        }

        VertexPtrList &neighborsPtr(vertex.vertexPtrList());
        for (typename VertexPtrList::iterator jt(neighborsPtr.begin()); jt != neighborsPtr.end();)
        {
          Vertex *neighborPtr(*jt);

          VertexPtrList &neighborsOfNeighbors(neighborPtr->vertexPtrList());
          for (typename VertexPtrList::iterator kt(neighborsOfNeighbors.begin()); kt != neighborsOfNeighbors.end();)
          {
            Vertex *neighborOfNeighborPtr(*kt);
            if (neighborOfNeighborPtr == &vertex)
            {
              kt = neighborsOfNeighbors.erase(kt);
              continue;
            }
            ++kt;
          }

          for (Vertex &newVertex : vertexList)
          {
            if (neighborPtr->boundingBox()(newVertex.boundingBox()))
            {
              neighborPtr->vertexPtrList().push_back(&newVertex);
              newVertex.vertexPtrList().push_back(neighborPtr);
            }
          }

          ++jt;
        }

        for (typename VertexList::iterator jt(vertexList.begin()); jt != std::prev(vertexList.end()); ++jt)
          for (typename VertexList::iterator kt(std::next(jt)); kt != vertexList.end(); ++kt)
          {
            jt->vertexPtrList().push_back(&*kt);
            kt->vertexPtrList().push_back(&*jt);
          }

        vertexList_.splice(vertexList_.end(), vertexList);
        it = vertexList_.erase(it);
      }
    }

    template <typename VertexCloud>
    typename Graph<VertexCloud>::Parameter &
    Graph<VertexCloud>::parameter()
    {
      return parameter_;
    }

    template <typename VertexCloud>
    typename Graph<VertexCloud>::Parameter const &
    Graph<VertexCloud>::parameter() const
    {
      return parameter_;
    }

    template <typename VertexCloud>
    void
    Graph<VertexCloud>::parameter(Parameter const &parameter)
    {
      parameter_ = parameter;
    }

    template <typename VertexCloud>
    std::list<typename Graph<VertexCloud>::Vertex> &
    Graph<VertexCloud>::vertexList()
    {
      return vertexList_;
    }

    template <typename VertexCloud>
    std::list<typename Graph<VertexCloud>::Vertex> const &
    Graph<VertexCloud>::vertexList() const
    {
      return vertexList_;
    }

    template <typename VertexCloud>
    void
    Graph<VertexCloud>::vertexList(VertexList const &vertexList)
    {
      vertexList_ = vertexList;
    }

    // Graph Parameter
    template <typename VertexCloud>
    Graph<VertexCloud>::Parameter::Parameter(Dimensions const &dimensions, Cardinal const &cardinal)
        : dimensions_(dimensions), cardinal_(cardinal)
    {
    }

    template <typename VertexCloud>
    Graph<VertexCloud>::Parameter::operator bool() const
    {
      bool output(cardinal_ > 0);
      for (typename VertexCloud::value_type::value_type const &d : dimensions_)
        output &= d > 0;
      return output;
    }

    template <typename VertexCloud>
    typename Graph<VertexCloud>::Parameter::Dimensions &
    Graph<VertexCloud>::Parameter::dimensions()
    {
      return dimensions_;
    }

    template <typename VertexCloud>
    typename Graph<VertexCloud>::Parameter::Dimensions const &
    Graph<VertexCloud>::Parameter::dimensions() const
    {
      return dimensions_;
    }

    template <typename VertexCloud>
    void
    Graph<VertexCloud>::Parameter::dimensions(Dimensions const &dimensions)
    {
      dimensions_ = dimensions;
    }

    template <typename VertexCloud>
    typename Graph<VertexCloud>::Parameter::Cardinal &
    Graph<VertexCloud>::Parameter::cardinal()
    {
      return cardinal_;
    }

    template <typename VertexCloud>
    typename Graph<VertexCloud>::Parameter::Cardinal const &
    Graph<VertexCloud>::Parameter::cardinal() const
    {
      return cardinal_;
    }

    template <typename VertexCloud>
    void
    Graph<VertexCloud>::Parameter::cardinal(Cardinal const &cardinal)
    {
      cardinal_ = cardinal;
    }

    // Graph Vertex
    template <typename VertexCloud>
    Graph<VertexCloud>::Vertex::Vertex(BoundingBox const &boundingBox, Indices const &indices)
        : boundingBox_(boundingBox), indices_(indices)
    {
    }

    template <typename VertexCloud>
    typename Graph<VertexCloud>::Vertex::BoundingBox &
    Graph<VertexCloud>::Vertex::boundingBox()
    {
      return boundingBox_;
    }

    template <typename VertexCloud>
    typename Graph<VertexCloud>::Vertex::BoundingBox const &
    Graph<VertexCloud>::Vertex::boundingBox() const
    {
      return boundingBox_;
    }

    template <typename VertexCloud>
    void
    Graph<VertexCloud>::Vertex::boundingBox(BoundingBox const &boundingBox)
    {
      boundingBox_ = boundingBox;
    }

    template <typename VertexCloud>
    typename Graph<VertexCloud>::Vertex::Indices &
    Graph<VertexCloud>::Vertex::indices()
    {
      return indices_;
    }

    template <typename VertexCloud>
    typename Graph<VertexCloud>::Vertex::Indices const &
    Graph<VertexCloud>::Vertex::indices() const
    {
      return indices_;
    }

    template <typename VertexCloud>
    void
    Graph<VertexCloud>::Vertex::indices(Indices const &indices)
    {
      indices_ = indices;
    }

    template <typename VertexCloud>
    std::list<typename Graph<VertexCloud>::Vertex *> &
    Graph<VertexCloud>::Vertex::vertexPtrList()
    {
      return vertexPtrList_;
    }

    template <typename VertexCloud>
    std::list<typename Graph<VertexCloud>::Vertex *> const &
    Graph<VertexCloud>::Vertex::vertexPtrList() const
    {
      return vertexPtrList_;
    }

    template <typename VertexCloud>
    void
    Graph<VertexCloud>::Vertex::vertexPtrList(VertexPtrList const &vertexPtrList)
    {
      vertexPtrList_ = vertexPtrList;
    }
  }
}

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/vertex.hpp>

typedef potasse::cloud::Vertex<3> Vertex;
typedef potasse::cloud::Cloud<Vertex> VertexCloud;

template class potasse::detector::Graph<VertexCloud>;
