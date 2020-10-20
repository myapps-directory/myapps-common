#pragma once

#include "ola/common/utility/version_impl.hpp"

namespace ola {
namespace utility {

const char* version_vcs_commit();
const char* version_vcs_branch();
const char* version_full();

} //namespace utility
} //namespace ola
