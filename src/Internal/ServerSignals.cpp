#include "Internal/ServerSignals.h"

namespace Internal
{
  ServerSignals::ServerSignals()
  {

  }

  ServerSignalsPtr ServerSignals::getInstance()
  {
    static ServerSignalsPtr out = std::shared_ptr<ServerSignals>(new ServerSignals());
    return out;
  }

  boost::signals2::connection ServerSignals::subscribeToCreateAccount(AccountCallback callback)
  {
    return mCreateAccount.connect(callback);
  }
  boost::signals2::connection ServerSignals::subscribeToUpdateAccount(AccountCallback callback)
  {
    return mUpdateAccount.connect(callback);
  }
  boost::signals2::connection ServerSignals::subscribeToDeleteAccount(AccountCallback callback)
  {
    return mDeleteAccount.connect(callback);
  }

  boost::signals2::connection ServerSignals::subscribeToCreateCameraFeed(CameraFeedCallback callback)
  {
    return mCreateCamera.connect(callback);
  }
  boost::signals2::connection ServerSignals::subscribeToUpdateCameraFeed(CameraFeedCallback callback)
  {
    return mUpdateCamera.connect(callback);
  }
  boost::signals2::connection ServerSignals::subscribeToDeleteCameraFeed(CameraFeedCallback callback)
  {
    return mDeleteCamera.connect(callback);
  }

  void ServerSignals::createAccount(const Common::Types::AccountPtr account)
  {
    mCreateAccount(account);
  }

  void ServerSignals::updateAccount(const Common::Types::AccountPtr account)
  {
    mUpdateAccount(account);
  }

  void ServerSignals::deleteAccount(const Common::Types::AccountPtr account)
  {
    mDeleteAccount(account);
  }

  void ServerSignals::createCameraFeed(const Common::Types::CameraFeedPtr cameraFeed)
  {
    mCreateCamera(cameraFeed);
  }
  void ServerSignals::updateCameraFeed(const Common::Types::CameraFeedPtr cameraFeed)
  {
    mUpdateCamera(cameraFeed);
  }
  void ServerSignals::deleteCameraFeed(const Common::Types::CameraFeedPtr cameraFeed)
  {
    mDeleteCamera(cameraFeed);
  }
}
