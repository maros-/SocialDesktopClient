/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <Swiften/Base/API.h>

namespace Swift {
	class XMLParser;
	class XMLParserClient;

	class SWIFTEN_API XMLParserFactory {
		public:
			virtual ~XMLParserFactory();

			virtual XMLParser* createXMLParser(XMLParserClient*) = 0;
	};
}