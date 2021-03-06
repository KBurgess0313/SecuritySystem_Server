#include "Common/Types/Account.h"
#include "Common/Types/CameraFeed.h"

#include "Interfaces/CentralService.h"
#include "Config/ConfigFile.h"
#include "Internal/ServerSignals.h"

#include <iostream>
#include <thread>

using namespace std;

void loadCameras();
void loadAccounts();
void loadSettings();

int main(void)
{
  std::thread::id tid = std::this_thread::get_id();

  std::cout << "Main Thread: " << tid << "\n";
  Interfaces::CentralService service = Interfaces::CentralService();

  loadSettings();

  while(1)
  {
    sleep(1);
  }

  return 0;
}

void loadSettings()
{
  loadCameras();
  loadAccounts();
}

void loadAccounts()
{
  Config::ConfigFilePtr config = Config::ConfigFile::instance();

  Internal::ServerSignalsPtr serverSignals = Internal::ServerSignals::getInstance();

  Common::Types::AccountPtrList accounts = config->getAccounts();

  for(const Common::Types::AccountPtr acct : accounts)
  {
    serverSignals->createAccount(acct);
  }
}

void loadCameras()
{
  Config::ConfigFilePtr config = Config::ConfigFile::instance();
  Internal::ServerSignalsPtr serverSignals = Internal::ServerSignals::getInstance();

  Common::Types::CameraFeedPtrList cameras = config->getCameraFeeds();

  for(const Common::Types::CameraFeedPtr camera : cameras)
  {
    camera->init();
    serverSignals->createCameraFeed(camera);
  }
}
