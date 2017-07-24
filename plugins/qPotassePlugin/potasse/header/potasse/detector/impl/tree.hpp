#include<potasse/detector/tree.hpp>

#include<stdexcept>

namespace potasse
{
  namespace detector
  {
    namespace util
    {
      //////////////////////////////////////////////////////////////////////////
      // Tree
      //////////////////////////////////////////////////////////////////////////
      template<typename PointCloud>
        Tree<PointCloud>::Tree
        ( Parameter const &parameter ): 
          parameter_(parameter) {}

      template<typename PointCloud>
        typename Tree<PointCloud>::Parameter &
        Tree<PointCloud>::parameter()
        { 
          if (static_cast<bool>(parameter_))
            return parameter_;
          throw std::domain_error("Invalid parameters.");
        }

      template<typename PointCloud>
        typename Tree<PointCloud>::Parameter const &
        Tree<PointCloud>::parameter() const
        { 
          if (static_cast<bool>(parameter_))
            return parameter_;
          throw std::domain_error("Invalid parameters.");
        }

      template<typename PointCloud>
        void
        Tree<PointCloud>::parameter(Parameter const &parameter)
        { parameter_ = parameter; }

      template<typename PointCloud>
        void
        Tree<PointCloud>::compute
        ( PointCloud const &pointCloud,
          Indices indices )
        {
          Indices::const_iterator it(indices.begin());

          Box box(pointCloud[*it], pointCloud[*it]);
          while (++it != indices.end())
          {
            for (size_t i(0); i < std::tuple_size<Point>::value; ++i)
              if (box.min()[i] < pointCloud[*it][i])
                box.min()[i] = pointCloud[*it][i];
              else if (box.max()[i] > pointCloud[*it][i])
                box.max()[i] = pointCloud[*it][i];
          }

          Parameter const &parameter(this->parameter());
          if (parameter(box))
            root_ = std::make_shared<Branch>(parameter, pointCloud, nullptr, box, indices);
          else
            root_ = std::make_shared<Leaf>(nullptr, box, indices);
        }

      template<typename PointCloud>
        typename Tree<PointCloud>::Scalar
        Tree<PointCloud>::ratio() const
        { return root_->volume() / root_->box().volume(); }

      //////////////////////////////////////////////////////////////////////////
      // Parameter
      //////////////////////////////////////////////////////////////////////////
      template<typename PointCloud>
        Tree<PointCloud>::Parameter::Parameter
        ( Dimensions const &dimensions ):
          dimensions_(dimensions) {}

      template<typename PointCloud>
        typename Tree<PointCloud>::Parameter::Dimensions const &
        Tree<PointCloud>::Parameter::dimensions() const
        { return dimensions_; }

      template<typename PointCloud>
        void
        Tree<PointCloud>::Parameter::dimensions
        ( Dimensions const &dimensions )
        { dimensions_ = dimensions; }

      template<typename PointCloud>
        typename Tree<PointCloud>::Scalar
        Tree<PointCloud>::Parameter::dimension(size_t dimension) const
        { return dimensions_[dimension]; }

      template<typename PointCloud>
        template<size_t S>
        typename Tree<PointCloud>::Scalar
        Tree<PointCloud>::Parameter::dimension() const
        { return dimensions_[S]; }

      template<typename PointCloud>
        Tree<PointCloud>::Parameter::operator bool() const
        {
          for (size_t const &d : dimensions_)
            if (d <= 0)
              return false;
          return true;
        }

      template<typename PointCloud>
        bool 
        Tree<PointCloud>::Parameter::operator()(Box const &box) const
        {
          for (size_t i(0); i < std::tuple_size<Point>::value; ++i)
            if (box.dimension(i) < dimension(i))
              return false;

          return true;
        }

      //////////////////////////////////////////////////////////////////////////
      // Node
      //////////////////////////////////////////////////////////////////////////
      template<typename PointCloud>
        Tree<PointCloud>::Node::Node
        ( Node const *const parent,
          Box const &box ):
          parent_(parent),
          box_(box) {}

      template<typename PointCloud>
        typename Tree<PointCloud>::Node const *
        Tree<PointCloud>::Node::parent() const
        { return parent_; }

      template<typename PointCloud>
        typename Tree<PointCloud>::Box &
        Tree<PointCloud>::Node::box()
        { return box_; }

      template<typename PointCloud>
        typename Tree<PointCloud>::Box const &
        Tree<PointCloud>::Node::box() const
        { return box_; }

      template<typename PointCloud>
        void
        Tree<PointCloud>::Node::box(Box const &box)
        { box_ = box; }

      //////////////////////////////////////////////////////////////////////////
      // Branch
      //////////////////////////////////////////////////////////////////////////
      template<typename PointCloud>
        Tree<PointCloud>::Branch::Branch
        ( Parameter const &parameter,
          PointCloud const &pointCloud,
          Node const *const parent,
          Box const &box,
          Indices &indices ):
          Node(parent, box)
      {
        Point const center(this->box().center());

        static const size_t size(1 << std::tuple_size<Point>::value);

        // Split indices
        Indices indicesArray[size];
        while (!indices.empty())
        {
          Point const &point(pointCloud[indices.front()]);

          size_t id(0);
          for (size_t i(0); i < std::tuple_size<Point>::value; ++i)
            if (point.point()[i] > center.point()[i])
              id += (size >> (1+i));

          indicesArray[id].splice(indicesArray[id].end(), indices, indices.begin());
        }

        // Find the correct bounding box
        Point const &min(this->box().min());
        Point const &max(this->box().max());
        for (size_t i(0); i < size; ++i)
        {
          if (indicesArray[i].empty())
          {
            nodePtrArray_[i].reset();
            continue;
          }

          Point const &point(pointCloud[indicesArray[i].front()]);
          Point bbMin(min), bbMax(max);
          for (size_t j(0); j < std::tuple_size<Point>::value; ++j)
            if (point[j] < center[j])
              bbMax[j] = center[j];
            else
              bbMin[j] = center[j];

          Box bbBox(bbMin, bbMax);

          if (parameter(bbBox))
            nodePtrArray_[i] = std::make_shared<Branch>(parameter, pointCloud, this, bbBox, indices);
          else
            nodePtrArray_[i] = std::make_shared<Leaf>(this, bbBox, indices);
        }
      }

      template<typename PointCloud>
        typename Tree<PointCloud>::Scalar
        Tree<PointCloud>::Branch::volume() const
        {
          Scalar output(0);
          for (NodeConstPtr nodeConstPtr : nodePtrArray_)
            if (nodeConstPtr)
              output += nodeConstPtr->volume();
          return output;
        }

      //////////////////////////////////////////////////////////////////////////
      // Leaf
      //////////////////////////////////////////////////////////////////////////
      template<typename PointCloud>
        Tree<PointCloud>::Leaf::Leaf
        ( Node const *const parent,
          Box const &box,
          Indices const &indices ):
          Node(parent, box),
          indices_(indices) {}

      template<typename PointCloud>
        typename Tree<PointCloud>::Indices &
        Tree<PointCloud>::Leaf::indices()
        { return indices_; }

      template<typename PointCloud>
        typename Tree<PointCloud>::Indices const &
        Tree<PointCloud>::Leaf::indices() const
        { return indices_; }

      template<typename PointCloud>
        void
        Tree<PointCloud>::Leaf::indices(Indices const &indices)
        { indices_ = indices; }

      template<typename PointCloud>
        typename Tree<PointCloud>::Scalar
        Tree<PointCloud>::Leaf::volume() const
        {
          return this->box().volume();
        }
    }

    template<typename PointCloud>
      Tree<PointCloud>::Tree
      ( PointCloudConstPtr pointCloudConstPtr,
        Parameter const &parameter ):
        util::Tree<PointCloud>(parameter),
        pointCloudConstWPtr_(pointCloudConstPtr) {}

    template<typename PointCloud>
      typename Tree<PointCloud>::PointCloudConstPtr const
      Tree<PointCloud>::pointCloud() const
      {
        PointCloudConstPtr output(pointCloudConstWPtr_.lock());
        if (static_cast<bool>(output))
          return output;
        throw std::bad_weak_ptr();
      }

    template<typename PointCloud>
      void
      Tree<PointCloud>::pointCloud(PointCloudConstPtr pointCloudConstPtr)
      { pointCloudConstWPtr_ = pointCloudConstPtr; }

    template<typename PointCloud>
      void
      Tree<PointCloud>::compute(Indices const &indices)
      {
        PointCloudConstPtr pointCloudConstPtr(this->pointCloud());
        util::Tree<PointCloud>::compute(*pointCloudConstPtr, indices);
      }

    template<typename PointCloud>
      void
      Tree<PointCloud>::compute
      ( PointCloudConstPtr pointCloudConstPtr,
        Indices const &indices,
        Parameter const &parameter )
      {
        this->pointCloud(pointCloudConstPtr);
        this->parameter(parameter);
        this->compute(indices);
      }
  }
}
