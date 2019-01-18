#include "ola/common/utility/ola_error.hpp"
#include <sstream>

namespace ola {
namespace utility {

namespace {

enum struct ErrorE : uint32_t {
    Generic = 1,
    Exist,
    Backend,
    Pending,
    AccountInvalid,
    AccountApplicationQuota,
    AccountStorageQuota,
    AccountNoReservation,
};

constexpr uint32_t cast(const ErrorE _e)
{
    return static_cast<uint32_t>(_e);
}

class ErrorCategory : public solid::ErrorCategoryT {
public:
    ErrorCategory() {}
    const char* name() const noexcept override
    {
        return "solid::frame::mprpc";
    }
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
    default:
        oss << "Unknown";
        break;
    }
    return oss.str();
}

} //namespace

solid::ErrorConditionT make_error(const uint32_t _err)
{
    return solid::ErrorConditionT(_err, category);
}

/*extern*/ const solid::ErrorConditionT error_generic(cast(ErrorE::Generic), category);
/*extern*/ const solid::ErrorConditionT error_exist(cast(ErrorE::Exist), category);
/*extern*/ const solid::ErrorConditionT error_backend(cast(ErrorE::Backend), category);
/*extern*/ const solid::ErrorConditionT error_pending(cast(ErrorE::Pending), category);
/*extern*/ const solid::ErrorConditionT error_account_invalid(cast(ErrorE::AccountInvalid), category);
/*extern*/ const solid::ErrorConditionT error_account_application_quota(cast(ErrorE::AccountApplicationQuota), category);
/*extern*/ const solid::ErrorConditionT error_account_storage_quota(cast(ErrorE::AccountStorageQuota), category);
/*extern*/ const solid::ErrorConditionT error_account_no_reservation(cast(ErrorE::AccountNoReservation), category);

} //namespace utility
} //namespace ola
