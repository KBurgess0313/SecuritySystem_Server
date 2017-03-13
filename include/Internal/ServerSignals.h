#ifndef INTERNAL_SERVERSIGNALS_H
#define INTERNAL_SERVERSIGNALS_H

#include <boost/signals2.hpp>
#include <boost/shared_ptr.hpp>

namespace Common {
namespace Types {

  class Account;
  typedef std::shared_ptr<Account> AccountPtr;

  class CameraFeed;
  typedef std::shared_ptr<CameraFeed> CameraFeedPtr;

} //end namespace Types
} //end namespace Common

namespace Internal
{
  class ServerSignals;

  typedef std::shared_ptr<ServerSignals> ServerSignalsPtr;

  typedef boost::function<void(const Common::Types::AccountPtr)> AccountCallback;
  typedef boost::function<void(const Common::Types::CameraFeedPtr)> CameraFeedCallback;

  typedef boost::signals2::signal<void(const Common::Types::AccountPtr)> AccountSignal;
  typedef boost::signals2::signal<void(const Common::Types::CameraFeedPtr)> CameraFeedSignal;

  class ServerSignals
  {
  public:
    static ServerSignalsPtr getInstance();

    boost::signals2::connection subscribeToCreateAccount(AccountCallback callback);
    boost::signals2::connection subscribeToUpdateAccount(AccountCallback callback);
    boost::signals2::connection subscribeToDeleteAccount(AccountCallback callback);

    boost::signals2::connection subscribeToCreateCameraFeed(CameraFeedCallback callback);
    boost::signals2::connection subscribeToUpdateCameraFeed(CameraFeedCallback callback);
    boost::signals2::connection subscribeToDeleteCameraFeed(CameraFeedCallback callback);

    void createAccount(const Common::Types::AccountPtr account);
    void updateAccount(const Common::Types::AccountPtr account);
    void deleteAccount(const Common::Types::AccountPtr account);

    void createCameraFeed(const Common::Types::CameraFeedPtr cameraFeed);
    void updateCameraFeed(const Common::Types::CameraFeedPtr cameraFeed);
    void deleteCameraFeed(const Common::Types::CameraFeedPtr cameraFeed);

  private:
    ServerSignals();

  private:
    AccountSignal mCreateAccount;
    AccountSignal mUpdateAccount;
    AccountSignal mDeleteAccount;

    CameraFeedSignal mCreateCamera;
    CameraFeedSignal mUpdateCamera;
    CameraFeedSignal mDeleteCamera;
  };

} //end namespace InterfaceServices
#endif
