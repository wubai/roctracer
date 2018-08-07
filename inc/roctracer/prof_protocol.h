////////////////////////////////////////////////////////////////////////////////
//
// The University of Illinois/NCSA
// Open Source License (NCSA)
//
// Copyright (c) 2014-2015, Advanced Micro Devices, Inc. All rights reserved.
//
// Developed by:
//
//                 AMD Research and AMD HSA Software Development
//
//                 Advanced Micro Devices, Inc.
//
//                 www.amd.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal with the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
//  - Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimers.
//  - Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimers in
//    the documentation and/or other materials provided with the distribution.
//  - Neither the names of Advanced Micro Devices, Inc,
//    nor the names of its contributors may be used to endorse or promote
//    products derived from this Software without specific prior written
//    permission.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS WITH THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef INC_ROCTRACER_PROF_PROTOCOL_H_
#define INC_ROCTRACER_PROF_PROTOCOL_H_

// Traced API domains
typedef enum {
  ACTIVITY_DOMAIN_ANY = 0,                        // Any domain
  ACTIVITY_DOMAIN_HIP_API = 1,                    // HIP domain
  ACTIVITY_DOMAIN_HCC_OPS = 2,                    // HCC domain
  ACTIVITY_DOMAIN_NUMBER = 3
} activity_domain_t;

// API calback type
typedef void (*activity_rtapi_callback_t)(uint32_t domain, uint32_t cid, const void* data, void* arg);

// API callback phase
typedef enum {
  ACTIVITY_API_PHASE_ENTER = 0,
  ACTIVITY_API_PHASE_EXIT = 1
} r_feature_kind_t;

// Trace record types
// Correlation id
typedef uint64_t activity_correlation_id_t;

// Activity record type
struct activity_record_t {
    uint32_t domain;                               // activity domain id
    uint32_t op_id;                                // operation id, dispatch/copy/barrier
    uint32_t activity_kind;                        // activity kind
    activity_correlation_id_t correlation_id;     // activity correlation ID
    uint64_t begin_ns;                             // host begin timestamp
    uint64_t end_ns;                               // host end timestamp
    int device_id;                                 // device id
    uint64_t stream_id;                            // stream id
    size_t bytes;                                  // data size bytes
};

// Activity sync calback type
typedef activity_record_t* (*activity_sync_callback_t)(uint32_t cid, activity_record_t* record, const void* data, void* arg);
// Activity async calback type
typedef void (*activity_async_callback_t)(uint32_t op, void* record, void* arg);

#endif  // INC_ROCTRACER_PROF_PROTOCOL_H_
