ectomodule(conversion
  module.cpp
  MatToPointCloud.cpp
  PointCloudToDepthMat.cpp
  PointCloudToMat.cpp
  )

link_ecto(conversion
  ${OpenCV_LIBS}
  ${pcl_LIBRARIES}
  ${Boost_LIBRARIES}
  )

install_ecto_module(conversion)
