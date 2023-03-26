#include "notifications.h";

namespace notifications {

	Notification* getNotification(std::string notification_content) {
		//std::string notification_content = parsers::getFileContents("notifications");

		return Notification::create(
			parsers::getSplittedStringItem(notification_content, ">-<", 0),
			parsers::getSplittedStringItem(notification_content, ">-<", 1)
		);
	}

}