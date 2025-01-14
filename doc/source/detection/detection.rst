Detection
=========
.. highlight:: ectosh

.. contents::

Using the different trained objects, we can now detect them.

use
^^^

.. toggle1:: click here for ROS instructions

    Just run the detection.py script in /apps. This will run continuously on the input image/point cloud.

.. toggle2:: click here for non-ROS instructions

    Just rosrun::

       % rosrun object_recognition_server server.py -c config_detection.sample

    This will start an actionlib server that you can then query. If you want to test the server, just execute the client once::

       % rosrun object_recognition_server client.py

The server requires a configuration file through the ``-c`` option. Some of the
options in there can be overriden by the command line for convenience.
For now, use the default config_detection.sample and change the following parameters to your needs:
  - the db section can point to a different root, another machine for example
  - the object_ids should be the list of object ids you want to train on. If you want, you can also use object_names,
that are more human readable
  - the source type can be ``ros_kinect`` or ``ros_bag``, in which case you must supply the bag field

A typical command line session might look like::

   % apps/detection -c config_detection.sample
   [ INFO] [1317692023.717854617]: Initialized ros. node_name: /ecto_node_1317692023710501315
   Threadpool executing [unlimited] ticks in 5 threads.
   [ INFO] [1317692024.254588151]: Subscribed to topic:/camera/rgb/camera_info with queue size of 0
   [ INFO] [1317692024.255467268]: Subscribed to topic:/camera/depth_registered/camera_info with queue size of 0
   [ INFO] [1317692024.256186358]: Subscribed to topic:/camera/depth_registered/image with queue size of 0
   [ INFO] [1317692024.256863212]: Subscribed to topic:/camera/rgb/image_color with queue size of 0
   model_id: e2449bdc43fd6d9dd646fcbcd012daaa
   span: 0.433393 meters
   1
   ***Starting object: 0
   * starting RANSAC
    added : 1
    added : 0
   * n inliers: 1824
   [-0.056509789, 0.99800211, 0.028263446;
     0.94346958, 0.062639669, -0.32548648;
     -0.32660651, 0.0082725696, -0.94512439]
   [-0.32655218; 0.03684178; 0.85040951]
   ********************* found 1poses
   [ INFO] [1317692117.187226953]: publishing to topic:/object_ids
   [ INFO] [1317692117.188155476]: publishing to topic:/poses


command line interface
^^^^^^^^^^^^^^^^^^^^^^
.. program-output:: apps/detection --help
   :in_srcdir:
