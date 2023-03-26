#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>
#include <matdash/console.hpp>
#include <gd.h>
#include "gdaddresses.h"
#include "notifications.h"
#include <experimental/coroutine>
#include "CCCoroutine.h"

std::string notification_content_global;

class scheduleFunctions {
public:
	static void pipeMainRunner() {
		scheduleFunctions::pipeMain("\\\\.\\pipe\\GDNotificationsPipe");
	}

	static int processNotificationEventInstant(std::string notification_content)
	{
		notifications::Notification* notification = notifications::getNotification(notification_content);

		gd::AchievementNotifier::sharedState()->notifyAchievement(
			notification->title.c_str(),
			notification->contents.c_str(),
			nullptr,
			false
		);

		return 1;
	}

	void processNotificationEvent(float dt) {
		scheduleFunctions::processNotificationEventInstant(notification_content_global);
	}

	static void InfLoopTest(float dt) {
		std::cout << "Loop test" << std::endl;
	}

	static void pipeMain(std::string const& pipeName) {

		char buffer[1024];
		DWORD dwRead;
		HANDLE hPipe;

		hPipe = CreateNamedPipeA(
			pipeName.c_str(),
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
			8,
			1024 * 16,
			1024 * 16,
			NMPWAIT_USE_DEFAULT_WAIT,
			NULL
		);

		std::cout << "Starting Named Pipe!" << std::endl;

		using namespace cocos2d;

		while (hPipe != INVALID_HANDLE_VALUE) {
			if (ConnectNamedPipe(hPipe, NULL) != FALSE) {
				while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE) {
					buffer[dwRead] = '\0';
					std::string objectString(buffer);
					notification_content_global = objectString;
					CCDirector::sharedDirector()->getRunningScene()->scheduleOnce(schedule_selector(scheduleFunctions::processNotificationEvent), 0);

				}
			}
			DisconnectNamedPipe(hPipe);
		}
	}

};


bool MenuLayer_init(gd::MenuLayer* self) {
    if (!matdash::orig<&MenuLayer_init>(self)) return false;

	using namespace cocos2d;

	std::thread([]() {
		scheduleFunctions::pipeMainRunner();
	}).detach();

    return true;
}

void mod_main(HMODULE hModule) {
    //matdash::create_console();

    matdash::add_hook<&MenuLayer_init>(gd::base + gdaddresses::MenuLayer_init);
}