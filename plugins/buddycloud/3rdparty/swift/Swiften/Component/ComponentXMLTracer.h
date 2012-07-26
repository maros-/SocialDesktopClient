/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <Swiften/Base/API.h>
#include <Swiften/Component/Component.h>

namespace Swift {
	class SWIFTEN_API ComponentXMLTracer {
		public:
			ComponentXMLTracer(CoreComponent* component);

		private:
			static void printData(char direction, const SafeByteArray& data);
			static void printLine(char c);
	};
}
