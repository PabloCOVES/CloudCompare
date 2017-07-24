#include <potasse/detector/tree.hpp>

template <typename Parameter, typename Box, typename Cardinal>
bool
toSplitOrNotToSplit(Parameter const &parameter, Box const &box, Cardinal const &cardinal)
{
  bool output(cardinal > 0 && parameter.cardinal() < cardinal);
  for (std::size_t i(0); i < parameter.dimensions().size(); ++i)
    output &= parameter.dimensions()[i] < box.dimension(i);
  return output;
}

namespace potasse
{
  namespace detector
  {
    // Tree
    template <typename PointCloud>
    Tree<PointCloud>::Tree(Parameter const &parameter) : parameter_(parameter)
    {
    }

    template <typename PointCloud>
    Tree<PointCloud>::operator bool() const
    {
      return static_cast<bool>(parameter_);
    }

    template <typename PointCloud>
    void
    Tree<PointCloud>::compute(
        PointCloud const &pointCloud,
        Indices const &indices,
        Box const &box)
    {
      root_ = Node::create(pointCloud, parameter_, indices, box);
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Scalar
    Tree<PointCloud>::volume() const
    {
      if (static_cast<bool>(root_))
        return root_->volume();
      else
        return 0;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Parameter &
    Tree<PointCloud>::parameter()
    {
      return parameter_;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Parameter const &
    Tree<PointCloud>::parameter() const
    {
      return parameter_;
    }

    template <typename PointCloud>
    void
    Tree<PointCloud>::parameter(Parameter const &parameter)
    {
      parameter_ = parameter;
    }

    // Tree Parameter
    template <typename PointCloud>
    Tree<PointCloud>::Parameter::Parameter(Dimensions const &dimensions, Cardinal const &cardinal)
        : dimensions_(dimensions), cardinal_(cardinal)
    {
    }

    template <typename PointCloud>
    Tree<PointCloud>::Parameter::operator bool() const
    {
      bool output(cardinal_ > 0);
      for (typename PointCloud::value_type::value_type const &d : dimensions_)
        output &= d > 0;
      return output;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Parameter::Dimensions &
    Tree<PointCloud>::Parameter::dimensions()
    {
      return dimensions_;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Parameter::Dimensions const &
    Tree<PointCloud>::Parameter::dimensions() const
    {
      return dimensions_;
    }

    template <typename PointCloud>
    void
    Tree<PointCloud>::Parameter::dimensions(Dimensions const &dimensions)
    {
      dimensions_ = dimensions;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Parameter::Cardinal &
    Tree<PointCloud>::Parameter::cardinal()
    {
      return cardinal_;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Parameter::Cardinal const &
    Tree<PointCloud>::Parameter::cardinal() const
    {
      return cardinal_;
    }

    template <typename PointCloud>
    void
    Tree<PointCloud>::Parameter::cardinal(Cardinal const &cardinal)
    {
      cardinal_ = cardinal;
    }

    // Tree Node
    template <typename PointCloud>
    Tree<PointCloud>::Node::Node(Node *parent, Box const &box)
        : parent_(parent), box_(box)
    {
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::NodePtr
    Tree<PointCloud>::Node::create(
        PointCloud const &pointCloud,
        Parameter const &parameter,
        Indices const &indices,
        Box const &box)
    {
      NodePtr output(nullptr);

      if (toSplitOrNotToSplit(parameter, box, indices.size()))
        output = std::make_shared<Branch>(pointCloud, parameter, indices, box);
      else
        output = std::make_shared<Leaf>(indices, box);

      return output;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Node *
    Tree<PointCloud>::Node::parent()
    {
      return parent_;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Node const *
    Tree<PointCloud>::Node::parent() const
    {
      return parent_;
    }

    template <typename PointCloud>
    void
    Tree<PointCloud>::Node::parent(Node *parent)
    {
      parent_ = parent;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Box &
    Tree<PointCloud>::Node::box()
    {
      return box_;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Box const &
    Tree<PointCloud>::Node::box() const
    {
      return box_;
    }

    template <typename PointCloud>
    void
    Tree<PointCloud>::Node::box(Box const &box)
    {
      box_ = box;
    }

    // Tree Branch
    template <typename PointCloud>
    Tree<PointCloud>::Branch::Branch(
        PointCloud const &pointCloud,
        Parameter const &parameter,
        Indices const &indices,
        Box const &box)
        : Branch(nullptr, pointCloud, parameter, indices, box)
    {
    }

    template <typename PointCloud>
    Tree<PointCloud>::Branch::Branch(
        Node *parent,
        PointCloud const &pointCloud,
        Parameter const &parameter,
        Indices indices,
        Box const &box)
        : Node(parent, box)
    {
      typedef typename PointCloud::value_type Point;

      static const size_t dimension(std::tuple_size<Point>::value);
      static const size_t size(1 << dimension);

      Point const center(box.center());

      std::array<Indices, size> indicesArray;
      while (!indices.empty())
      {
        Point const &point(pointCloud[indices.front()]);

        size_t id(0);
        for (size_t i(0); i < dimension; ++i)
          if (point.point()[i] > center.point()[i])
            id += (size >> (1 + i));

        indicesArray[id].splice(indicesArray[id].end(), indices, indices.begin());
      }

      for (size_t i(0); i < size; ++i)
      {
        if (indicesArray[i].empty())
        {
          nodePrtArray_[i].reset();
          continue;
        }

        Point const &point(pointCloud[indicesArray[i].front()]);
        Point min(box.min()), max(box.max());

        for (size_t j(0); j < dimension; ++j)
          if (point[j] < center[j])
            max[j] = center[j];
          else
            min[j] = center[j];

        Box newBox(min, max);

        if (toSplitOrNotToSplit(parameter, newBox, indicesArray[i].size()))
          nodePrtArray_[i] = std::make_shared<Branch>(this, pointCloud, parameter, indicesArray[i], newBox);
        else
          nodePrtArray_[i] = std::make_shared<Leaf>(this, indicesArray[i], newBox);
      }
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Scalar
    Tree<PointCloud>::Branch::volume() const
    {
      Scalar output(0);
      for (NodePtr const &nodePtr : nodePrtArray_)
        if (static_cast<bool>(nodePtr))
          output += nodePtr->volume();
      return output;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Branch::NodePtrArray &
    Tree<PointCloud>::Branch::nodePtrArray()
    {
      return nodePrtArray_;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Branch::NodePtrArray const &
    Tree<PointCloud>::Branch::nodePtrArray() const
    {
      return nodePrtArray_;
    }

    template <typename PointCloud>
    void
    Tree<PointCloud>::Branch::nodePtrArray(NodePtrArray const &nodePtrArray)
    {
      nodePrtArray_ = nodePtrArray;
    }

    // Tree Leaf
    template <typename PointCloud>
    Tree<PointCloud>::Leaf::Leaf(
        Node *parent,
        Indices const &indices,
        Box const &box)
        : Node(parent, box), indices_(indices)
    {
    }

    template <typename PointCloud>
    Tree<PointCloud>::Leaf::Leaf(
        Indices const &indices,
        Box const &box)
        : Node(nullptr, box), indices_(indices)
    {
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Scalar
    Tree<PointCloud>::Leaf::volume() const
    {
      return this->box().volume();
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Indices &
    Tree<PointCloud>::Leaf::indices()
    {
      return indices_;
    }

    template <typename PointCloud>
    typename Tree<PointCloud>::Indices const &
    Tree<PointCloud>::Leaf::indices() const
    {
      return indices_;
    }

    template <typename PointCloud>
    void
    Tree<PointCloud>::Leaf::indices(Indices const &indices)
    {
      indices_ = indices;
    }
  }
}

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/point.hpp>

typedef potasse::cloud::Point<double, 3> Point;
typedef potasse::cloud::Cloud<Point> PointCloud;

template class potasse::detector::Tree<PointCloud>;
