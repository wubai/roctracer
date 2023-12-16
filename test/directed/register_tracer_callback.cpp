#include <hip/hip_runtime.h>
#include "roctracer.h"
#include "roctracer_hip.h"

#include <dlfcn.h>
#include "iostream"

using OperationId = hip_api_id_t;
using ApiData = hip_api_data_t;

struct TraceData {
  ApiData api_data;                // API specific data (for example, function arguments).
  uint64_t phase_enter_timestamp;  // timestamp when phase_enter was executed.
  uint64_t phase_data;             // data that can be shared between phase_enter and phase_exit.

  void (*phase_enter)(OperationId operation_id, TraceData* data);
  void (*phase_exit)(OperationId operation_id, TraceData* data);
};
static void Start(OperationId operation_id, TraceData* trace_data) {
  std::cout << "start: " << "line: " << __LINE__ << " (" << __FUNCTION__ << "): " << std::endl;
}
static void Exit(OperationId operation_id, TraceData* trace_data) {
  std::cout << "exit: " << "line: " << __LINE__ << " (" << __FUNCTION__ << "): " << std::endl;
}

static int Enter(OperationId operation_id, TraceData* trace_data) {
    trace_data->phase_enter = Start;
    trace_data->phase_exit = Exit;
  return 0;
}

int TracerCallback(activity_domain_t domain, uint32_t operation_id, void* data) {
  switch (domain) {
    case ACTIVITY_DOMAIN_HSA_API:
      break;

    case ACTIVITY_DOMAIN_HIP_API:
      std::cout << "ACTIVITY_DOMAIN_HIP_API"  << std::endl;
      std::cout << "operation_id " << operation_id << std::endl;
      Enter(static_cast<OperationId>(operation_id), static_cast<TraceData*>(data));
      return 0;

    case ACTIVITY_DOMAIN_HIP_OPS:
      break;

    case ACTIVITY_DOMAIN_ROCTX:
      break;

    case ACTIVITY_DOMAIN_HSA_OPS:
      break;

    case ACTIVITY_DOMAIN_HSA_EVT:
      break;

    default:
        printf("Invalid choice.\n");
  }
  return -1;
}

int main() {
  void* tracer_library = dlopen("libamdhip64.so", RTLD_LAZY);
  assert(tracer_library != nullptr);

  auto function = reinterpret_cast<
                     void (*)(int (*callback)(activity_domain_t domain,
                                                 uint32_t operation_id,
                                                  void* data))>
                     (dlsym(tracer_library, "hipRegisterTracerCallback"));
  assert(function != nullptr);
  function(TracerCallback);

  hipSetDevice(0);
  dlclose(tracer_library);
  return 0;
}