namespace potasse
{
  namespace cloud
  {
    template <typename PC>
      BoundingBox<PC>::BoundingBox(PointCloud const &pointCloud)
      : min_{pointCloud.front()},
      max_{pointCloud.front()}
    {
      for (auto const &point : pointCloud)
        for (typename PointCloud::size_type coordinate{0}; coordinate < Point().size(); ++coordinate)
        {
          if (min_[coordinate] > point[coordinate]) min_[coordinate] = point[coordinate];
          if (max_[coordinate] < point[coordinate]) max_[coordinate] = point[coordinate];
        }
    }

    template <typename PC>
      BoundingBox<PC>::BoundingBox(PointCloud const &pointCloud, Indices const &indices)
      : min_{pointCloud[indices.front()]},
      max_{pointCloud[indices.front()]}
    {
      for (auto const &index : indices)
      {
        Point const &point{pointCloud[index]};
        for (typename PointCloud::size_type coordinate{0}; coordinate < Point().size(); ++coordinate)
        {
          if (min_[coordinate] > point[coordinate]) min_[coordinate] = point[coordinate];
          if (max_[coordinate] < point[coordinate]) max_[coordinate] = point[coordinate];
        }
      }
    }

    template <typename PC>
      typename BoundingBox<PC>::Point
      BoundingBox<PC>::center() const
      {
        Point output;
        for (typename PointCloud::size_type coordinate{0}; coordinate < Point().size(); ++coordinate)
          output[coordinate] = (max_[coordinate] + min_[coordinate]) / 2.;
        return output;
      }

    template <typename PC>
      typename BoundingBox<PC>::Point
      BoundingBox<PC>::dimensions() const
      {
        Point output;
        for (typename PointCloud::size_type coordinate{0}; coordinate < Point().size(); ++coordinate)
          output[coordinate] = max_[coordinate] - min_[coordinate];
        return output;
      }
  }
}
