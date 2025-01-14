#include <ecto/ecto.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/format.hpp>
#include <string>

#include <object_recognition/db/db.h>
#include <object_recognition/db/opencv.h>
#include <object_recognition/db/prototypes/observations.hpp>

using ecto::tendrils;

namespace object_recognition
{
  namespace prototypes
  {
    using db::Document;
    using db::ObjectDb;
    struct ObservationInserter
    {
      static void
      declare_params(tendrils& params)
      {
        params.declare<std::string>("object_id", "The object id, to associate this frame with.").required(true);
        params.declare<std::string>("session_id", "The session id, to associate this frame with.").required(true);

        params.declare(&ObservationInserter::db_params_, "db_params", "The database parameters");
      }
      static void
      declare_io(const tendrils& params, tendrils& inputs, tendrils& outputs)
      {
        Observation::declare(inputs, true); //required
      }

      ObservationInserter()
          :
            frame_number(0)
      {
      }
      void
      on_object_id_change(const std::string& id)
      {
        object_id = id;
      }
      void
      on_session_id_change(const std::string& id)
      {
        session_id = id;
        frame_number = 0;
      }
      void
      configure(const tendrils& params, const tendrils& inputs, const tendrils& outputs)
      {
        db = object_recognition::db::ObjectDb(*db_params_);
        ecto::spore<std::string> object_id = params["object_id"];
        object_id.set_callback(boost::bind(&ObservationInserter::on_object_id_change, this, _1));
        ecto::spore<std::string> session_id = params["session_id"];
        session_id.set_callback(boost::bind(&ObservationInserter::on_session_id_change, this, _1));
      }
      int
      process(const tendrils& inputs, const tendrils& outputs)
      {
        std::cout << "Inserting frame: " << frame_number << std::endl;
        Observation obs;
        obs << inputs;
        obs.frame_number = frame_number++;
        obs.object_id = object_id;
        obs.session_id = session_id;
        Document doc(db);
        obs >> doc;
        doc.Persist();
        return ecto::OK;
      }
      int frame_number;
      std::string object_id, session_id;
      ecto::spore<db::ObjectDbParameters> db_params_;
      db::ObjectDb db;
    };
  }
}
ECTO_CELL(object_recognition_db, object_recognition::prototypes::ObservationInserter, "ObservationInserter",
          "Inserts observations into the database.");
