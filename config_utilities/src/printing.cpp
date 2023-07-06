

#include "config_utilities/printing.h"

namespace config::internal {

void checkDefaultValues(MetaData& data, const MetaData& default_data) {
  for (internal::FieldInfo& info : data.field_infos) {
    if (info.subconfig_id >= 0) {
      // This means the field is a subconfig, check the entire subconfig.
      checkDefaultValues(data.sub_configs[info.subconfig_id], default_data.sub_configs[info.subconfig_id]);
      info.is_default = true;
      for (const internal::FieldInfo& sub_info : data.sub_configs[info.subconfig_id].field_infos) {
        if (!sub_info.is_default) {
          info.is_default = false;
          break;
        }
      }
    } else {
      // Check the field itself. NOTE(lschmid): Operator YAML::Node== checks for identity, not equality. Comparing the
      // formatted strings should be identical for default constructed configs.
      if (internal::dataToString(data.data[info.name]) == internal::dataToString(default_data.data[info.name])) {
        info.is_default = true;
      }
    }
  }
}

}  // namespace config::internal
