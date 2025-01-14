#include <ecto/ecto.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

#include "object_recognition/common/types.h"
#include <object_recognition/db/db.h>
#include <object_recognition/db/opencv.h>
#include <object_recognition/db/prototypes/observations.hpp>

#include "object_recognition/db/db.h"

#define DEFAULT_COUCHDB_URL "http://localhost:5984"
using ecto::tendrils;
namespace object_recognition
{
  namespace prototypes
  {

    using db::Document;
    using db::ObjectDb;

    struct ObservationReader
    {
      static void
      declare_params(tendrils& params)
      {
        params.declare(&ObservationReader::db_params_, "db_params", "The DB parameters");
      }
      static void
      declare_io(const tendrils& params, tendrils& inputs, tendrils& outputs)
      {
        inputs.declare<std::string>("observation", "The observation id to load.");
        Observation::declare(outputs, false); //not required
        outputs.declare<int>("frame_number", "The frame id number.", 0);
      }
      ObservationReader()
          :
            current_frame(0)
      {
      }
      void
      configure(const tendrils& params, const tendrils& inputs, const tendrils& outputs)
      {
        observation = inputs["observation"];
        db = object_recognition::db::ObjectDb(*db_params_);
      }
      int
      process(const tendrils& inputs, const tendrils& outputs)
      {
        Document doc(db, *observation);
        Observation obs;
        obs << doc;
        obs >> outputs;
        return 0;
      }
      int total_rows, offset;
      ecto::spore<std::string> observation;
      ecto::spore<db::ObjectDbParameters> db_params_;
      ObjectDb db;
      int current_frame;
    };
  }
}
ECTO_CELL(object_recognition_db, object_recognition::prototypes::ObservationReader, "ObservationReader",
          "Reads observations from the database.");
