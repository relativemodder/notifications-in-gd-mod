#include <matdash.hpp>
#include <gd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include <thread>
#include "parsers.h"

namespace notifications {

	class Notification {
	public:
		std::string title;
		std::string contents;

		inline static Notification* create (

			std::string title,
			std::string contents ) {

			Notification* notification = new Notification();
			notification->title = title;
			notification->contents = contents;

			return notification;
		}
	};

	Notification* getNotification(std::string notification_content);

}