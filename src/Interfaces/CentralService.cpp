#include "Interfaces/CentralService.h"

#include "Common/Interfaces/Messages/LoginMessage.h"
#include "Common/Interfaces/Messages/CameraMessage.h"
#include "Common/Interfaces/Utils/XmlHelpers.h"

#include "Common/Types/Account.h"
#include "Common/Types/CameraFeed.h"

#include "Config/ConfigFile.h"
#include "Internal/ServerSignals.h"

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <cpprest/http_msg.h>\

namespace {
  using namespace boost::property_tree;
} //end namespace

namespace Interfaces {

  namespace {
    namespace CI = Common::Interfaces;
    namespace CT = Common::Types;
    namespace CIM = Common::Interfaces::Messages;

    namespace bp = boost::property_tree;
  }

  class Session
  {
  public:
    Session(CT::Privelages::Type aPrivelages) :
      mSessionId(boost::uuids::random_generator()()),
      mPrivelages(aPrivelages)
    {
    }

    bool operator==(const boost::uuids::uuid& aId)
    {
      return (mSessionId == aId);
    }

    std::string getSessionIdStr()
    {
      return boost::lexical_cast<std::string>(mSessionId);
    }

    boost::uuids::uuid mSessionId;
    CT::Privelages::Type mPrivelages;
  };

  CentralService::CentralService() :
    TcpRestServer(constructUri()),
    mServerSignals(Internal::ServerSignals::getInstance())
  {
    mServerSignals->subscribeToCreateAccount(boost::bind(&CentralService::createUserAccount, this, boost::placeholders::_1));
    mServerSignals->subscribeToCreateCameraFeed(boost::bind(&CentralService::createCameraFeed, this, boost::placeholders::_1));

    setGetHandler(boost::bind(&CentralService::handleGet, this, boost::placeholders::_1));
    setPostHandler(boost::bind(&CentralService::handlePost, this, boost::placeholders::_1));
    setDeleteHandler(boost::bind(&CentralService::handleDelete, this, boost::placeholders::_1));
  }

  void CentralService::createUserAccount(const CT::AccountPtr aAccount)
  {
    mUserAccounts.push_back(aAccount);
  }

  void CentralService::createCameraFeed(const CT::CameraFeedPtr aCamera)
  {
    mLiveCameras.push_back(aCamera);
  }

  void CentralService::handleGet(web::http::http_request aRequest)
  {
    bp::ptree message = CI::Utils::requestToXml(aRequest);
    CIM::Types::Value messageType = CI::Utils::getMessageType(message);

    switch(messageType)
    {
      case CIM::Types::CAMERA:
        std::cout << "Get Cameras received\n";
        break;
      case CIM::Types::ACCOUNT:
        std::cout << "Get Accounts received\n";
        break;
      case CIM::Types::INVALID:
        std::cout << "Invalid GET received\n";
        break;
    }
  }

  void CentralService::handlePost(web::http::http_request aRequest)
  {
    bp::ptree message = CI::Utils::requestToXml(aRequest);
    CIM::Types::Value messageType = CI::Utils::getMessageType(message);

    std::thread::id tid = std::this_thread::get_id();
    std::cout << "Post in Thread: " << tid << "\n";

    web::http::http_response response;
    switch(messageType)
    {
      case CIM::Types::LOGIN:
        response = processLoginMessage(message);
        break;
      case CIM::Types::CAMERA:
        response = processPostCameraMessage(message);
        break;
      case CIM::Types::ACCOUNT:
        std::cout << "Post Accounts received\n";
        break;
      case CIM::Types::INVALID:
        std::cout << "Invalid POST received\n";
        break;
    }

    aRequest.reply(response);
  }

  void CentralService::handleDelete(web::http::http_request aRequest)
  {
    bp::ptree message = CI::Utils::requestToXml(aRequest);
    CIM::Types::Value messageType = CI::Utils::getMessageType(message);

    web::http::http_response http_response;
    switch(messageType)
    {
      case CIM::Types::LOGIN:
        std::cout << "Delete Login received\n";
        break;
      case CIM::Types::CAMERA:
        std::cout << "Delete Cameras received\n";
        break;
      case CIM::Types::ACCOUNT:
        std::cout << "Delete Accounts received\n";
        break;
      case CIM::Types::INVALID:
        break;
    }
  }

  web::http::http_response CentralService::processLoginMessage(const bp::ptree& aLoginXml)
  {
    web::http::http_response response;

    response.set_status_code(web::http::status_codes::Unauthorized);

    CIM::LoginMessage login = CIM::LoginMessage(aLoginXml);

    for(const CT::AccountPtr& acct : mUserAccounts)
    {
      if(acct->isLogin(login))
      {
        SessionPtr session = std::make_shared<Session>(acct->getPrivelages());
        mActiveSessions.push_back(session);

        bp::ptree xmlResponse;
        xmlResponse.put("Message.Session.uuid", session->getSessionIdStr());

        response.set_status_code(web::http::status_codes::OK);
        response.set_body(CI::Utils::xmlToString(xmlResponse));
        break;
      }
    }

    return response;
  }


  web::http::http_response CentralService::processPostCameraMessage(const boost::property_tree::ptree& aMessage)
  {
    web::http::http_response response;

    boost::uuids::uuid sessionId = boost::lexical_cast<boost::uuids::uuid>(aMessage.get<std::string>("Message.Cameras.SessionId"));

    if(!isSessionIdValid(sessionId))
    {
      response.set_status_code(web::http::status_codes::Unauthorized);
      return response;
    }

    CIM::CameraMessage message = CIM::CameraMessage(aMessage);

    CT::CameraFeedPtr cam = message.getCamera();

    mLiveCameras.push_back(cam);

    cam->init();

    mServerSignals->createCameraFeed(cam);

    response.set_status_code(web::http::status_codes::OK);

    return response;
  }


  bool CentralService::isSessionIdValid(const boost::uuids::uuid& aSessionId)
  {
    bool isValid = false;

    for(const SessionPtr& session : mActiveSessions)
    {
      if(session->mSessionId == aSessionId)
      {
        isValid = true;
        break;
      }
    }

    return isValid;
  }

  web::http::uri CentralService::constructUri()
  {
    Config::ConfigFilePtr config = Config::ConfigFile::instance();

    std::string uriStr = config->getServerUri();

    web::http::uri uri = web::http::uri(utility::conversions::to_string_t(uriStr));

    return uri;
  }
}
