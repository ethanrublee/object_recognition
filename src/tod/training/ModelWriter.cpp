#include <ecto/ecto.hpp>
#include <opencv2/core/core.hpp>

#include <object_recognition/db/ModelWriter.h>

namespace object_recognition
{
  namespace tod
  {
    /** Implementation class inserting the TOD models in the DB
     */
    struct ModelWriterImpl: public db::bases::ModelWriterImpl
    {
    public:
      static void
      declare_io(const ecto::tendrils& params, ecto::tendrils& inputs, ecto::tendrils& outputs)
      {
        inputs.declare(&ModelWriterImpl::points_, "points", "The 3d position of the points.");
        inputs.declare(&ModelWriterImpl::descriptors_, "descriptors", "The descriptors.");
      }

      virtual int
      process(const ecto::tendrils& inputs, const ecto::tendrils& outputs, db::Document& doc)
      {
        doc.set_attachment<cv::Mat>("descriptors", *descriptors_);
        doc.set_attachment<cv::Mat>("points", *points_);
        return ecto::OK;
      }

      virtual std::string
      model_type() const
      {
        return "TOD";
      }

    private:
      ecto::spore<cv::Mat> points_;
      ecto::spore<cv::Mat> descriptors_;
    };

    /** Class inserting the TOD models in the DB
     */
    typedef db::bases::ModelWriterBase<ModelWriterImpl> ModelWriter;
  }
}

ECTO_CELL(tod_training, object_recognition::tod::ModelWriter, "ModelWriter", "Insert a TOD model into the db")