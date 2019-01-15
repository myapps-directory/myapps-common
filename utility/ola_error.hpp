#pragma once

#include "solid/system/error.hpp"

namespace ola {
namespace utility {

solid::ErrorConditionT make_error(uint32_t _err);

extern const solid::ErrorConditionT error_generic;
extern const solid::ErrorConditionT error_exist;
extern const solid::ErrorConditionT error_backend;
extern const solid::ErrorConditionT error_account_invalid;
extern const solid::ErrorConditionT error_account_application_quota;
extern const solid::ErrorConditionT error_account_storage_quota;
extern const solid::ErrorConditionT error_account_no_reservation;

} //namespace utility
} //namespace ola
