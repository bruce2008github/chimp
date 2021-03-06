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
#include "transport/response.h"

namespace chimp {
namespace transport {
namespace response {

ErrorResponse::ErrorResponse(ResponseCode code, const char *message)
{
    code_ = code;
    message_ = message;
}


msgpack_sbuffer *ErrorResponse::ToMessagePack()
{
    msgpack_sbuffer* buffer = msgpack_sbuffer_new();
    msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

    msgpack_pack_array(pk, 3);
    msgpack_pack_unsigned_int(pk, code_);
    msgpack_pack_nil(pk);

    int msg_len = strlen(message_);
    msgpack_pack_raw(pk, msg_len);
    msgpack_pack_raw_body(pk, message_, msg_len);

    msgpack_packer_free(pk);
    return buffer;
}


SuccessResponse::SuccessResponse()
{
}


msgpack_sbuffer *SuccessResponse::ToMessagePack()
{
    msgpack_sbuffer* buffer = msgpack_sbuffer_new();
    msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

    msgpack_pack_array(pk, 3);
    msgpack_pack_unsigned_int(pk, ResponseCode::RESPONSE_CODE_OK);
    msgpack_pack_nil(pk);
    msgpack_pack_nil(pk);

    msgpack_packer_free(pk);
    return buffer;
}

}
}
}
