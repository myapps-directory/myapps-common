#pragma once
#include "solid/system/error.hpp"
#include <cstdint>

namespace myapps {
namespace utility {

solid::ErrorConditionT make_error(uint32_t _err);

extern const solid::ErrorConditionT error_version;
extern const solid::ErrorConditionT error_state;
extern const solid::ErrorConditionT error_generic;
extern const solid::ErrorConditionT error_exist;
extern const solid::ErrorConditionT error_retry;
extern const solid::ErrorConditionT error_backend;
extern const solid::ErrorConditionT error_pending;
extern const solid::ErrorConditionT error_argument_invalid;
extern const solid::ErrorConditionT error_request_invalid;
extern const solid::ErrorConditionT error_authentication_invalid;
extern const solid::ErrorConditionT error_authentication_locked;
extern const solid::ErrorConditionT error_authentication_validate;
extern const solid::ErrorConditionT error_authentication_wait;
extern const solid::ErrorConditionT error_authentication_relogin;
extern const solid::ErrorConditionT error_authentication_demo;
extern const solid::ErrorConditionT error_authentication_demo_invalid;
extern const solid::ErrorConditionT error_authentication_connection_count;
extern const solid::ErrorConditionT error_account_invalid;
extern const solid::ErrorConditionT error_account_application_quota;
extern const solid::ErrorConditionT error_account_storage_quota;
extern const solid::ErrorConditionT error_account_no_reservation;
extern const solid::ErrorConditionT error_storage_limit;
extern const solid::ErrorConditionT error_storage_sum;
extern const solid::ErrorConditionT error_storage_zip;
extern const solid::ErrorConditionT error_application_invalid;
extern const solid::ErrorConditionT error_application_reservation;
extern const solid::ErrorConditionT error_application_system;
extern const solid::ErrorConditionT error_storage_invalid;
extern const solid::ErrorConditionT error_storage;
extern const solid::ErrorConditionT error_storage_size;
} // namespace utility
} // namespace myapps
