/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2009, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef DB_COUCH_H_
#define DB_COUCH_H_

#include "curl_interface.h"
#include "db_base.h"
#include "object_recognition/common/types.h"

using object_recognition::db::View;
using object_recognition::db::AttachmentName;
using object_recognition::db::CollectionName;
using object_recognition::db::DbType;
using object_recognition::db::DocumentId;
using object_recognition::db::ObjectId;
using object_recognition::db::MimeType;
using object_recognition::db::RevisionId;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ObjectDbCouch: public object_recognition::db::ObjectDbBase
{
public:
  ObjectDbCouch();

  ObjectDbCouch(const std::string &url, const std::string & collection);

  virtual void
  insert_object(const or_json::mObject &fields, DocumentId & document_id, RevisionId & revision_id);

  virtual void
  persist_fields(const DocumentId & document_id, const or_json::mObject &fields, RevisionId & revision_id);

  virtual void
  load_fields(const DocumentId & document_id, or_json::mObject &fields);

  virtual void
  get_attachment_stream(const DocumentId & document_id, const std::string& attachment_name,
                        const std::string& content_type, std::ostream& stream, RevisionId & revision_id);

  virtual void
  set_attachment_stream(const DocumentId & document_id, const AttachmentName& attachment_name,
                        const MimeType& mime_type, const std::istream& stream, RevisionId & revision_id);

  virtual
  void
  Delete(const ObjectId & id);

  virtual
  void
  Query(const View & view, int limit_rows, int start_offset, int& total_rows, int& offset,
        std::vector<DocumentId> & document_ids);

  virtual void
  Query(const std::vector<std::string> & queries, int limit_rows, int start_offset, int& total_rows, int& offset,
        std::vector<DocumentId> & document_ids);

  virtual std::string
  Status();

  virtual std::string
  Status(const CollectionName& collection);

  virtual void
  CreateCollection(const CollectionName &collection);

  virtual void
  DeleteCollection(const CollectionName &collection);

  virtual DbType
  type()
  {
    return "CouchDB";
  }
private:

  inline void
  precondition_id(const DocumentId & id) const
  {
    if (id.empty())
      throw std::runtime_error("The document's id must be initialized.");
  }

  inline void
  precondition_rev(const RevisionId & rev) const
  {
    if (rev.empty())
      throw std::runtime_error("The document must have a valid revision.");
  }

  void
  upload_json(const or_json::mObject &ptree, const std::string& url, const std::string& request);

  template<typename T>
  void
  read_json(T &reader, or_json::mObject& object)
  {
    or_json::mValue value;
    or_json::read(reader, value);
    object = value.get_obj();
  }

  template<typename T>
  void
  write_json(const or_json::mObject& object, T &writer)
  {
    or_json::mValue value(object);
    or_json::write(value, writer);
  }

  inline std::string
  url_id(const DocumentId & id) const
  {
    return root_ + "/" + collection_ + (id.empty() ? "" : "/" + id);
  }

  inline std::string
  url_id_rev(const DocumentId & id, const RevisionId & rev) const
  {
    return url_id(id) + "?rev=" + rev;
  }

  void
  GetObjectRevisionId(DocumentId& document_id, RevisionId & revision_id);

  void
  GetRevisionId(RevisionId & revision_id);

  /** Once json_reader_stream_ has been filled, call that function to get the results of the view
   *
   */
  void
  QueryView(const CollectionName & collection_name, int limit_rows, int start_offset, const std::string &options,
            int& total_rows, int& offset, std::vector<DocumentId> & document_ids, bool do_throw);

  // These mutable are they are internals/temporary variables
  mutable object_recognition::curl::cURL curl_;
  mutable std::stringstream json_writer_stream_, json_reader_stream_;

  object_recognition::curl::writer json_writer_;
  object_recognition::curl::reader json_reader_;
};

#endif /* DB_COUCH_H_ */
