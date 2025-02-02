// myapps/common/utility/src/error.cpp

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

#include "myapps/common/utility/error.hpp"
#include <sstream>

namespace myapps {
namespace utility {

namespace {

enum struct ErrorE : uint32_t {
    // NOTE: Always add at the end
    Generic = 1,
    Exist,
    Backend,
    Pending,
    Version,
    State,
    RequestInvalid,
    AuthenticationInvalid,
    AuthenticationValidate,
    AuthenticationLocked,
    AuthenticationWait,
    AuthenticationRelogin,
    AuthenticationDemo,
    AuthenticationDemoInvalid,
    AuthenticationConnectionCount,
    AccountInvalid,
    AccountApplicationQuota,
    AccountStorageQuota,
    AccountNoReservation,
    Storage,
    StorageLimit,
    StorageSum,
    StorageZip,
    StorageSize,
    StorageInvalid,
    ApplicationInvalid,
    ApplicationReservation,
    ApplicationSystem,
    Retry,
    ArgumentInvalid,
    RequestCount,
};

constexpr uint32_t cast(const ErrorE _e) { return static_cast<uint32_t>(_e); }

class ErrorCategory : public solid::ErrorCategoryT {
public:
    ErrorCategory() {}
    const char* name() const noexcept override { return "myapps::common"; }
    std::string message(int _ev) const override;
};

const ErrorCategory category;

std::string ErrorCategory::message(int _ev) const
{
    std::ostringstream oss;

    oss << "(" << name() << ":" << _ev << "): ";

    switch (_ev) {
    case 0:
        oss << "Success";
        break;
    case cast(ErrorE::Generic):
        oss << "Generic";
        break;
    case cast(ErrorE::Exist):
        oss << "Exist";
        break;
    case cast(ErrorE::Backend):
        oss << "Backend unavailable";
        break;
    case cast(ErrorE::Pending):
        oss << "Operation pending";
        break;
    case cast(ErrorE::Version):
        oss << "Version mismatch";
        break;
    case cast(ErrorE::State):
        oss << "Invalid state";
        break;
    case cast(ErrorE::RequestInvalid):
        oss << "Request Invalid";
        break;
    case cast(ErrorE::AuthenticationInvalid):
        oss << "Authentication: Invalid";
        break;
    case cast(ErrorE::AuthenticationValidate):
        oss << "Authentication: Validate Required";
        break;
    case cast(ErrorE::AuthenticationLocked):
        oss << "Authentication: Locked";
        break;
    case cast(ErrorE::AuthenticationWait):
        oss << "Authentication: Wait";
        break;
    case cast(ErrorE::AuthenticationRelogin):
        oss << "Authentication: Relogin";
        break;
    case cast(ErrorE::AuthenticationDemo):
        oss << "Authentication: No demo slot available";
        break;
    case cast(ErrorE::AuthenticationDemoInvalid):
        oss << "Authentication: Demo not supported";
        break;
    case cast(ErrorE::AuthenticationConnectionCount):
        oss << " Authentication: Connection Count";
        break;
    case cast(ErrorE::AccountInvalid):
        oss << "Account: Invalid";
        break;
    case cast(ErrorE::AccountApplicationQuota):
        oss << "Account: Application count quota exceeded";
        break;
    case cast(ErrorE::AccountStorageQuota):
        oss << "Account: Storage quota exceeded";
        break;
    case cast(ErrorE::AccountNoReservation):
        oss << "Account: No reservation";
        break;
    case cast(ErrorE::Storage):
        oss << "Storage";
        break;
    case cast(ErrorE::StorageLimit):
        oss << "Storage: Limit";
        break;
    case cast(ErrorE::StorageSum):
        oss << "Storage: Sum";
        break;
    case cast(ErrorE::StorageInvalid):
        oss << "Storage: Invalid";
        break;
    case cast(ErrorE::StorageZip):
        oss << "Storage: Zip";
        break;
    case cast(ErrorE::StorageSize):
        oss << "Storage: Size";
        break;
    case cast(ErrorE::ApplicationInvalid):
        oss << "Application: Invalid";
        break;
    case cast(ErrorE::ApplicationReservation):
        oss << "Application: Reservation";
        break;
    case cast(ErrorE::ApplicationSystem):
        oss << "Application: System";
        break;
    case cast(ErrorE::Retry):
        oss << "Retry";
        break;
    case cast(ErrorE::ArgumentInvalid):
        oss << "Invalid Argument";
        break;
    case cast(ErrorE::RequestCount):
        oss << "Request Count";
        break;
    default:
        oss << "Unknown";
        break;
    }
    return oss.str();
}

} // namespace

solid::ErrorConditionT make_error(const uint32_t _err)
{
    return solid::ErrorConditionT(_err, category);
}

/*extern*/ const solid::ErrorConditionT error_generic(cast(ErrorE::Generic),
    category);
/*extern*/ const solid::ErrorConditionT error_exist(cast(ErrorE::Exist),
    category);
/*extern*/ const solid::ErrorConditionT error_retry(cast(ErrorE::Retry),
    category);
/*extern*/ const solid::ErrorConditionT error_backend(cast(ErrorE::Backend),
    category);
/*extern*/ const solid::ErrorConditionT error_pending(cast(ErrorE::Pending),
    category);
/*extern*/ const solid::ErrorConditionT error_version(cast(ErrorE::Version),
    category);
/*extern*/ const solid::ErrorConditionT error_state(cast(ErrorE::State),
    category);
/*extern*/ const solid::ErrorConditionT error_request_count(cast(ErrorE::RequestCount),
    category);

/*extern*/ const solid::ErrorConditionT
    error_request_invalid(cast(ErrorE::RequestInvalid), category);
/*extern*/ const solid::ErrorConditionT
    error_authentication_invalid(cast(ErrorE::AuthenticationInvalid), category);
/*extern*/ const solid::ErrorConditionT
    error_authentication_validate(cast(ErrorE::AuthenticationValidate),
        category);
/*extern*/ const solid::ErrorConditionT
    error_authentication_locked(cast(ErrorE::AuthenticationLocked), category);
/*extern*/ const solid::ErrorConditionT
    error_authentication_wait(cast(ErrorE::AuthenticationWait), category);
/*extern*/ const solid::ErrorConditionT
    error_authentication_relogin(cast(ErrorE::AuthenticationRelogin), category);
/*extern*/ const solid::ErrorConditionT
    error_authentication_demo(cast(ErrorE::AuthenticationDemo), category);
/*extern*/ const solid::ErrorConditionT
                                        error_authentication_demo_invalid(cast(ErrorE::AuthenticationDemoInvalid), category);
/*extern*/ const solid::ErrorConditionT error_authentication_connection_count(cast(ErrorE::AuthenticationConnectionCount), category);
/*extern*/ const solid::ErrorConditionT
    error_argument_invalid(cast(ErrorE::ArgumentInvalid), category);
/*extern*/ const solid::ErrorConditionT
    error_account_invalid(cast(ErrorE::AccountInvalid), category);
/*extern*/ const solid::ErrorConditionT
    error_account_application_quota(cast(ErrorE::AccountApplicationQuota),
        category);
/*extern*/ const solid::ErrorConditionT
    error_account_storage_quota(cast(ErrorE::AccountStorageQuota), category);
/*extern*/ const solid::ErrorConditionT
                                        error_account_no_reservation(cast(ErrorE::AccountNoReservation), category);
/*extern*/ const solid::ErrorConditionT error_storage(cast(ErrorE::Storage),
    category);
/*extern*/ const solid::ErrorConditionT
    error_storage_limit(cast(ErrorE::StorageLimit), category);
/*extern*/ const solid::ErrorConditionT
    error_storage_sum(cast(ErrorE::StorageSum), category);
/*extern*/ const solid::ErrorConditionT
    error_storage_zip(cast(ErrorE::StorageZip), category);
/*extern*/ const solid::ErrorConditionT
    error_storage_size(cast(ErrorE::StorageSize), category);
/*extern*/ const solid::ErrorConditionT
    error_storage_invalid(cast(ErrorE::StorageInvalid), category);
/*extern*/ const solid::ErrorConditionT
    error_application_invalid(cast(ErrorE::ApplicationInvalid), category);
/*extern*/ const solid::ErrorConditionT
    error_application_reservation(cast(ErrorE::ApplicationReservation),
        category);
/*extern*/ const solid::ErrorConditionT
    error_application_system(cast(ErrorE::ApplicationSystem), category);

} // namespace utility
} // namespace myapps
