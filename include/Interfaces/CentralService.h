#include "Common/Interfaces/TcpRestServer.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid.hpp>

#include <string>

namespace Common {

  namespace Interfaces {
  namespace Messages {
    class LoginMessage;
    class AccountMessage;
  } //end namespace Messages
  } //end namespace Interfaces

  namespace Types {
    class Account;
    typedef std::shared_ptr<Account> AccountPtr;

    class CameraFeed;
    typedef std::shared_ptr<CameraFeed> CameraFeedPtr;
  } //end namespace Types
} //end namespace Common

namespace Internal {
  class ServerSignals;
  typedef std::shared_ptr<ServerSignals> ServerSignalsPtr;
} //end namespace ServerSignals

namespace Interfaces {

  class Session;
  typedef std::shared_ptr<Session> SessionPtr;

  class CentralService : public Common::Interfaces::TcpRestServer
  {
  public:
    CentralService();

    void createUserAccount(const Common::Types::AccountPtr aAccount);
    void createCameraFeed(const Common::Types::CameraFeedPtr aCamera);

  private:
    void handleGet(web::http::http_request aRequest);
    void handlePost(web::http::http_request aRequest);
    void handleDelete(web::http::http_request aRequest);

    web::http::http_response processLoginMessage(const boost::property_tree::ptree& aLoginXml);
    web::http::http_response processPostCameraMessage(const boost::property_tree::ptree& aLoginXml);

    bool isSessionIdValid(const boost::uuids::uuid& aSessionId);
    web::http::uri constructUri();

  private:
    Internal::ServerSignalsPtr mServerSignals;

    std::vector<SessionPtr> mActiveSessions;

    std::vector<Common::Types::AccountPtr> mUserAccounts;
    std::vector<Common::Types::CameraFeedPtr> mLiveCameras;
  };

} // end namespace Interfaces
