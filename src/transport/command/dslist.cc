/**
 * Copyright (C) 2013 Tadas Vilkeliskis <vilkeliskis.t@gmail.com>
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <iostream>
#include "service/dataset_manager.h"
#include "transport/response.h"
#include "transport/command/dslist.h"

namespace chimp {
namespace transport {
namespace command {

DatasetList::DatasetList(chimp::transport::Client *client)
{
    client_ = client;
}


int DatasetList::Execute()
{
    auto dsmanager = chimp::service::DatasetManager::GetInstance();
    auto datasets = dsmanager->GetDatasets();
    auto response = new Response();

    for (auto iter = datasets.begin(); iter != datasets.end(); ++iter) {
        response->AddItem(iter->first, iter->second->GetDimensions());
    }

    response_.reset(response);
    client_->Write(response_);
    return 0;
}


int DatasetList::FromMessagePack(const msgpack_unpacked *msg)
{
    return 0;
}


msgpack_sbuffer *DatasetList::ToMessagePack()
{
    msgpack_sbuffer* buffer = msgpack_sbuffer_new();
    msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

    msgpack_pack_array(pk, 1);
    msgpack_pack_raw(pk, 6);
    msgpack_pack_raw_body(pk, "DSLIST", 6);

    msgpack_packer_free(pk);
    return buffer;
}


DatasetList::Response::Response()
{
}


void DatasetList::Response::AddItem(const std::string &dataset_name,
                                    const chimp::db::dataset::Dimensions &dims)
{
    std::pair<std::string, chimp::db::dataset::Dimensions> element(dataset_name, dims);
    elements_.push_back(element);
}


msgpack_sbuffer *DatasetList::Response::ToMessagePack()
{
    msgpack_sbuffer* buffer = msgpack_sbuffer_new();
    msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

    msgpack_pack_array(pk, 3);
    msgpack_pack_unsigned_int(pk, chimp::transport::response::ResponseCode::RESPONSE_CODE_OK);
    msgpack_pack_array(pk, elements_.size());

    for (auto iter = elements_.begin(); iter != elements_.end(); ++iter) {
        msgpack_pack_map(pk, 2);
        msgpack_pack_raw(pk, 4);
        msgpack_pack_raw_body(pk, "name", 4);
        msgpack_pack_raw(pk, iter->first.size());
        msgpack_pack_raw_body(pk, iter->first.c_str(), iter->first.size());
        msgpack_pack_raw(pk, 10);
        msgpack_pack_raw_body(pk, "dimensions", 10);

        // dimensions object
        msgpack_pack_map(pk, 2);
        msgpack_pack_raw(pk, 4);
        msgpack_pack_raw_body(pk, "rows", 4);
        msgpack_pack_unsigned_int(pk, iter->second.rows);
        msgpack_pack_raw(pk, 7);
        msgpack_pack_raw_body(pk, "columns", 7);
        msgpack_pack_unsigned_int(pk, iter->second.cols);
    }

    // error
    msgpack_pack_nil(pk);

    msgpack_packer_free(pk);
    return buffer;
}


}
}
}
