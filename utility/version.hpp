#pragma once

#include "myapps/common/utility/version_impl.hpp"

namespace myapps {
namespace utility {

const char* version_vcs_commit();
const char* version_vcs_branch();
const char* version_full();

} // namespace utility
} // namespace myapps
