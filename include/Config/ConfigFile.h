#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include "Common/Types/Account.h"
#include "Common/Types/CameraFeed.h"

#include <boost/property_tree/ptree.hpp>

namespace Config {
class ConfigFile;
typedef std::shared_ptr<ConfigFile> ConfigFilePtr;

class ConfigFile
{
public:
  static ConfigFilePtr instance();

  std::string getServerUri();
  bool isDebugging();

  Common::Types::AccountPtrList getAccounts();
  Common::Types::CameraFeedPtrList getCameraFeeds();

private:
  ConfigFile();

private:
  std::string mFileName;

  boost::property_tree::ptree mFile;
};
} //end namespace Config
#endif // CONFIGFILE_H
