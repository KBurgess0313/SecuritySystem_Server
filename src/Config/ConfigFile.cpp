#include "Config/ConfigFile.h"

#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include <vector>
#include <unistd.h>

namespace Config {
ConfigFilePtr ConfigFile::instance()
{
  static ConfigFilePtr config = std::shared_ptr<ConfigFile>(new ConfigFile());

  return config;
}

bool ConfigFile::isDebugging()
{
  bool out = false;
  if(!mFile.empty())
  {
    out = mFile.get<bool>("SecuritySystem.Debugging");
  }

  return out;
}

Common::Types::AccountPtrList ConfigFile::getAccounts()
{
  using namespace boost::property_tree;
  using namespace Common::Types;

  AccountPtrList out;

  if(!mFile.empty())
  {
    for(const ptree::value_type& value : mFile.get_child("SecuritySystem.UserAccounts"))
    {
      AccountPtr acct = std::make_shared<Account>(value);
      out.push_back(acct);
    }
  }

  return out;
}

Common::Types::CameraFeedPtrList ConfigFile::getCameraFeeds()
{
  using namespace boost::property_tree;
  using namespace Common::Types;

  Common::Types::CameraFeedPtrList out;

  if(!mFile.empty())
  {
    for(const ptree::value_type& value : mFile.get_child("SecuritySystem.Cameras"))
    {
      CameraFeedPtr camera = CameraFeedPtr(new CameraFeed(value));
      out.push_back(camera);
    }
  }

  return out;
}

std::string ConfigFile::getServerUri()
{
  using namespace boost::property_tree;
  using namespace Common::Types;

  std::string uri = "";

  try
  {
    std::string host = mFile.get<std::string>("SecuritySystem.RestService.Host");
    std::string port = mFile.get<std::string>("SecuritySystem.RestService.Port");

    //If the string doesn't contain http://, prepend it to host.
    if(host.find("http://") == std::string::npos)
    {
      uri = std::string("http://");
    }

    uri += host;
    uri += std::string(":");
    uri += port;
  }
  catch(std::exception e)
  {
    std::cout << e.what() << "\n";
  }

  std::cout << "URI: " << uri << "\n";

  return uri;
}

ConfigFile::ConfigFile() :
  mFile()
{
  char* cwd = new char[100];
  getcwd(cwd, 100);

  mFileName = std::string(cwd) + "/config/config.xml";

  try
  {
    boost::property_tree::read_xml(mFileName, mFile);
  }
  catch(std::exception e)
  {
    std::cout << e.what() << "\n";

    std::cout << "error\n";
  }
}

} //end namespace Common
