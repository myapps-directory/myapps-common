// myapps/common/utility/error.hpp

// This file is part of MyApps.directory project
// Copyright (C) 2020, 2021, 2022, 2023, 2024, 2025 Valentin Palade (vipalade @ gmail . com)

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
extern const solid::ErrorConditionT error_request_count;
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
