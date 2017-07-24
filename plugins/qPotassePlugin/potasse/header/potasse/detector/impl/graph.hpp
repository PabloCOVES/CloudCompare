//#include <potasse/detector/graph.hpp>

//#include <numeric>

//namespace potasse
//{
  //namespace detector
  //{
    //namespace util
    //{
      //template <typename PointCloud>
      //Graph<PointCloud>::Graph(Parameter const &parameter) : parameter_(parameter)
      //{
      //}

      //template <typename PointCloud>
      //typename Graph<PointCloud>::Parameter const &
      //Graph<PointCloud>::parameter() const
      //{
        //if (static_cast<bool>(parameter_))
          //return parameter_;
        //throw std::domain_error("Invalid parameters.");
      //}

      //template <typename PointCloud>
      //void
      //Graph<PointCloud>::parameter(Parameter const &parameter)
      //{
        //parameter_ = parameter;
      //}

      //template <typename PointCloud>
      //void
      //Graph<PointCloud>::compute(PointCloud const &pointCloud)
      //{
        //edgePtrList_.clear();
        //vertexPtrList_.clear();

        //Box box(pointCloud.begin(), pointCloud.end());
        //Indices indices(pointCloud.size());
        //std::iota(indices.begin(), indices.end(), 0);

        //VertexPtr vertexPtr(std::make_shared<Vertex>(box, indices));
        //vertexPtrList_.push_front(vertexPtr);

        //split(pointCloud);
      //}

      //template <typename PointCloud>
      //void
      //Graph<PointCloud>::split(PointCloud const &pointCloud)
      //{
        //Parameter const &parameter(this->parameter());

        //// For every Vertex of the Graph that is big enough:
        //// Split it in smaller cells.
        //typename VertexPtrList::iterator it(vertexPtrList_.begin());
        //while (it != vertexPtrList_.end())
        //{
          //if (!parameter((*it)->box()))
          //{
            //++it;
            //continue;
          //}

          //split(pointCloud, it);
          //it = vertexPtrList_.erase(it);
        //}

        //// Once splitted:
        //// Link every Vertex to its neighbors
        //for (typename EdgePtrList::const_iterator it(edgePtrList_.begin()); it != edgePtrList_.end(); ++it)
        //{
          //(*it)->first.first.lock()->edgeWPtrList().push_back(*it);
          //(*it)->second.second.lock()->edgeWPtrList().push_back(*it);
        //}
      //}

      //template <typename PointCloud>
      //void
      //Graph<PointCloud>::split(PointCloud const &pointCloud, typename VertexPtrList::const_iterator it)
      //{
        //VertexPtr const vertexPtr(*it);

        //VertexPtrList vertexPtrList;
        //Point const center(vertexPtr->box().center());

        //static const size_t size(1 << std::tuple_size<Point>::value);

        //// Split indices
        //Indices indices[size];
        //while (!vertexPtr->indices().empty())
        //{
          //Point const &point(pointCloud[vertexPtr->indices().front()]);

          //size_t id(0);
          //for (size_t i(0); i < std::tuple_size<Point>::value; ++i)
            //if (point.point()[i] > center.point()[i])
              //id += (size >> (1 + i));

          //indices[id].splice(indices[id].end(), vertexPtr->indices(), vertexPtr->indices().begin());
        //}

        //// Find the correct bounding box
        //Point const &min(vertexPtr->box().min());
        //Point const &max(vertexPtr->box().max());
        //for (size_t i(0); i < size; ++i)
        //{
          //if (indices[i].empty())
            //continue;

          //Point const &point(pointCloud[indices[i].front()]);
          //Point bbMin(min), bbMax(max);

          //for (size_t j(0); j < std::tuple_size<Point>::value; ++j)
            //if (point[j] < center[j])
              //bbMax[j] = center[j];
            //else
              //bbMin[j] = center[j];

          //vertexPtrList.push_back(std::make_shared<Vertex>(Box(bbMin, bbMax), indices[i]));
        //}

        //// Delete now useless Edges because of the deleted Vertex.
        //// And create the newly needed ones from old to new Vertices.
        //typename EdgePtrList::const_iterator jt(edgePtrList_.begin());
        //while (jt != edgePtrList_.end())
        //{
          //VertexPtr vertexTmpPtr((**jt)(vertexPtr));
          //if (static_cast<bool>(vertexTmpPtr))
          //{
            //for (typename VertexPtrList::const_iterator kt(vertexPtrList.begin()); kt != vertexPtrList.end(); ++kt)
              //if (vertexTmpPtr->box()((*kt)->box()))
                //edgePtrList_.push_back(std::make_shared<Edge>(vertexTmpPtr, *kt));

            //jt = edgePtrList_.erase(jt);
          //}
          //else
            //++jt;
        //}

        //// Create Edges betwen every new Vertices.
        //for (typename VertexPtrList::const_iterator jt(vertexPtrList.begin()); jt != std::prev(vertexPtrList.end()); ++jt)
          //for (typename VertexPtrList::const_iterator kt(std::next(jt)); kt != vertexPtrList.end(); ++kt)
            //edgePtrList_.push_back(std::make_shared<Edge>(*jt, *kt));

        //vertexPtrList_.splice(vertexPtrList_.end(), vertexPtrList);
      //}

      //template <typename PointCloud>
      //Graph<PointCloud>::Parameter::Parameter(Dimensions const &dimensions,
                                              //Cardinal const &cardinal) : dimensions_(dimensions),
                                                                          //cardinal_(cardinal)
      //{
      //}

      //template <typename PointCloud>
      //typename Graph<PointCloud>::Parameter::Dimensions const &
      //Graph<PointCloud>::Parameter::dimensions() const
      //{
        //return dimensions_;
      //}

      //template <typename PointCloud>
      //void
      //Graph<PointCloud>::Parameter::dimensions(Dimensions const &dimensions)
      //{
        //dimensions_ = dimensions;
      //}

      //template <typename PointCloud>
      //typename Graph<PointCloud>::Scalar
      //Graph<PointCloud>::Parameter::dimension(size_t dimension) const
      //{
        //return dimensions_[dimension];
      //}

      //template <typename PointCloud>
      //template <size_t S>
      //typename Graph<PointCloud>::Scalar
      //Graph<PointCloud>::Parameter::dimension() const
      //{
        //return dimensions_[S];
      //}

      //template <typename PointCloud>
      //typename Graph<PointCloud>::Parameter::Cardinal const &
      //Graph<PointCloud>::Parameter::cardinal() const
      //{
        //return cardinal_;
      //}

      //template <typename PointCloud>
      //void
      //Graph<PointCloud>::Parameter::cardinal(Cardinal const &cardinal)
      //{
        //cardinal_ = cardinal;
      //}

      //template <typename PointCloud>
      //Graph<PointCloud>::Parameter::operator bool() const
      //{
        //bool output(cardinal_ > 0);
        //for (size_t const &d : dimensions_)
          //output &= d > 0;
        //return output;
      //}

      //template <typename PointCloud>
      //bool
      //Graph<PointCloud>::Parameter::operator()(Box const &box) const
      //{
        //for (size_t i(0); i < std::tuple_size<Point>::value; ++i)
          //if (box.dimension(i) < dimension(i))
            //return false;

        //return true;
      //}

      ////Graph Vertex
      //template <typename PointCloud>
      //Graph<PointCloud>::Vertex::Vertex(Box const &box, Indices const &indices) : box_(box), indices_(indices)
      //{
      //}

      //template <typename PointCloud>
      //typename Graph<PointCloud>::Indices const &
      //Graph<PointCloud>::Vertex::indices() const
      //{
        //return indices_;
      //}

      //template <typename PointCloud>
      //typename Graph<PointCloud>::Box const &
      //Graph<PointCloud>::Vertex::box() const
      //{
        //return box_;
      //}

      //template <typename PointCloud>
      //typename Graph<PointCloud>::Vertex::EdgeWPtrList const &
      //Graph<PointCloud>::Vertex::edgeWPtrList() const
      //{
        //return edgeWPtrList_;
      //}

      //template <typename PointCloud>
      //typename Graph<PointCloud>::Indices &
      //Graph<PointCloud>::Vertex::indices()
      //{
        //return indices_;
      //}

      //template <typename PointCloud>
      //typename Graph<PointCloud>::Box &
      //Graph<PointCloud>::Vertex::box()
      //{
        //return box_;
      //}

      //template <typename PointCloud>
      //typename Graph<PointCloud>::Vertex::EdgeWPtrList &
      //Graph<PointCloud>::Vertex::edgeWPtrList()
      //{
        //return edgeWPtrList_;
      //}
    //}
  //}
//}
