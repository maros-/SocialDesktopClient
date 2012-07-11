#include "pubsub_parser.h"
#include "payloads/pubsub.h"
#include "pubsub_parser_factories.h"

/*
 * ITEMS PARSER
 */

template<class T>
ItemParser<T>::ItemParser() : level_(TopLevel), curr_parser_(0) {}

template<class T>
ItemParser<T>::~ItemParser() {
  std::vector<Swift::PayloadParserFactory*>::iterator it;
  for(it = factories_.begin(); it != factories_.end(); ++it) {
    delete *it;
  }
  delete curr_parser_;
}

template<class T>
void ItemParser<T>::addPaylodParserFactory(Swift::PayloadParserFactory* parser) {
  parsers_.addFactory(parser);
  factories_.push_back(parser);
}

template<class T>
void ItemParser<T>::handleStartElement(const std::string& element, const std::string& ns, const Swift::AttributeMap& attributes) {
  if (level_ == TopLevel) { // "item" level
  } else {
    if (!curr_parser_) {
      Swift::PayloadParserFactory* factory = parsers_.getPayloadParserFactory(element, ns, attributes);
      assert(factory);
      curr_parser_ = factory->createPayloadParser(); // FIX: each item, new parser is created, is that necessary for ItemParser?
    }
    assert(curr_parser_);
    curr_parser_->handleStartElement(element, ns, attributes);
  }
  ++level_;
}

template<class T>
void ItemParser<T>::handleEndElement(const std::string& element, const std::string& ns) {
  --level_;
  if (level_ == TopLevel && element == "item") {
    // push back handled payload in item element
    assert(curr_parser_);
    getPayloadInternal()->appendPayload(boost::dynamic_pointer_cast<T>(curr_parser_->getPayload())); // FIX: not efficient
    // reset variables
    delete curr_parser_;
    curr_parser_ = 0;
  } else {
    assert(curr_parser_);
    curr_parser_->handleEndElement(element, ns);
  }
}

template<class T>
void ItemParser<T>::handleCharacterData(const std::string& data) {
  if (level_ > TopLevel) {
    assert(curr_parser_);
    curr_parser_->handleCharacterData(data);
  }
  else { // everything else goes to sink
    character_data_ += data;
  }
}

/*
 * ATOM PARSER
 */

AtomParser::AtomParser() : level_(EntryLevel), meta_(NIL) {}

void AtomParser::handleStartElement(const std::string&  element, const std::string&  ns, const Swift::AttributeMap&  attributes) {
  if (level_ == EntryLevel) { // TopLevel element
  } else if (level_ == MetaLevel) {
    if (element == "author") {
      meta_ = Author;
    } else if (element == "content") {
      meta_ = Content;
      inner_text_.clear();
    } else if (element == "object" && ns == "http://activitystrea.ms/spec/1.0/") {
      meta_ = Object;
    } else if (element == "id") {
      meta_ = Id;
      inner_text_.clear();
    } else if (element == "in-reply-to" && ns == "http://purl.org/syndication/thread/1.0") {
      getPayloadInternal()->setInReplyTo(attributes.getAttributeValue("ref").get_value_or("__ID__"));
    }
  } else if (level_ > MetaLevel) {
    if (meta_ == Author) {
      if (element == "name" || element == "jid" || element == "uri")
        inner_text_.clear();
    } else if (meta_ == Object) {
      if (element == "object-type") {
        inner_text_.clear();
      }
    }
  }
  ++level_;
}

void AtomParser::handleEndElement(const std::string&  element, const std::string&  /*ns*/) {
  --level_;
  if (level_ == MetaLevel) {
    if (meta_ == Content) {
      getPayloadInternal()->setContent(inner_text_);
      meta_ = NIL;
    } else if (meta_ == Id) {
      getPayloadInternal()->setID(inner_text_);
      meta_ = NIL;
    }
  } else if (level_ > MetaLevel) {
    if (meta_ == Author) {
      if (element == "name") {
        getPayloadInternal()->setAuthor(inner_text_);
        meta_ = NIL;
      }
      else if (element == "jid") {
      } // nothing so far
      else if (element == "uri") {
      } // nothing so far
    } else if (meta_ == Object) {
      if (element == "object-type") {
        if (inner_text_ == "note") {
          getPayloadInternal()->setObjectType(Atom::NOTE);
        } else if (inner_text_ == "comment") {
          getPayloadInternal()->setObjectType(Atom::COMMENT);
        }
      }
    }
  }
}

void AtomParser::handleCharacterData(const std::string&  data) {
  if (level_ > MetaLevel) {
    inner_text_ += data;
  }
}

/*
 * PUBSUB ITEMS REQUEST PARSER
 */
PubsubItemsRequestParser::PubsubItemsRequestParser() : level_(TopLevel), is_parsing_items_(false) {
  item_parser_.addPaylodParserFactory(new AtomParserFactory);
}

void PubsubItemsRequestParser::handleStartElement(const std::string& element, const std::string& ns, const Swift::AttributeMap& attributes) {
  if (level_ == TopLevel) {
    if (element == "items") {
      getPayloadInternal()->setNode(attributes.getAttributeValue("node").get_value_or("___NODE___"));
      is_parsing_items_ = true;
    }
  } else {
    if (is_parsing_items_) {
      item_parser_.handleStartElement(element, ns, attributes);
    }
  }
  ++level_;
}

void PubsubItemsRequestParser::handleEndElement(const std::string& element, const std::string& ns) {
  --level_;
  if (level_ == TopLevel) {
    if (element == "items") {
      getPayloadInternal()->setItems(item_parser_.getPayloadInternal());
      is_parsing_items_ = false;
    }
  } else if (level_ > TopLevel && is_parsing_items_) {
    item_parser_.handleEndElement(element, ns);
  }
}

void PubsubItemsRequestParser::handleCharacterData(const std::string& data) {
  if (level_ > TopLevel && is_parsing_items_) {
    item_parser_.handleCharacterData(data);
  }
}

/*
 * PUBSUB PUBLISH REQUEST PARSER
 */
void PubsubPublishRequestParser::handleStartElement(const std::string& element, const std::string& /*ns*/, const Swift::AttributeMap& attributes) {
  if (element == "publish") {
    getPayloadInternal()->setNode(attributes.getAttributeValue("node").get_value_or("__NODE__"));
  } else if (element == "item") {
    getPayloadInternal()->setItemID(attributes.getAttributeValue("id").get_value_or("__ID__"));
  }
}

/*
 * PUBSUB RETRACT REQUEST PARSER
 */
//void PubsubRetractRequestParser::handleStartElement(const std::string& element, const std::string& /*ns*/, const Swift::AttributeMap& attributes) {
//  if (element == "retract") {
//    getPayloadInternal()->setNode(attributes.getAttributeValue("node").get_value_or("__NODE__"));
//  } else if (element == "item") {
//    getPayloadInternal()->setItemID(attributes.getAttributeValue("id").get_value_or("__ID__"));
//  }
//}

/*
 * PUBSUB SUBSCRIBE REQUEST PARSER
 */
void PubsubSubscribeRequestParser::handleStartElement(const std::string& element, const std::string& /*ns*/, const Swift::AttributeMap& attributes) {
  if (element == "subscription") {
    getPayloadInternal()->setNode(attributes.getAttributeValue("node").get_value_or(""));
    getPayloadInternal()->setSubscribersJID(attributes.getAttributeValue("jid").get_value_or(""));
    std::string subscr = attributes.getAttributeValue("subscription").get_value_or("");
    if (subscr == "subscribed") {
      getPayloadInternal()->setSubscription(PubsubSubscribeRequest::Subscribed);
    }
  }
}

/*
 * TOP LEVEL PUBSUB PARSER (element == "pubsub", ns == "http://jabber.org/protocol/pubsub")
 */
PubsubParser::PubsubParser() : level_(TopLevel), parser_(0) {}

PubsubParser::~PubsubParser() {
  delete parser_;
}

void PubsubParser::handleStartElement(const std::string&  element, const std::string&  ns, const Swift::AttributeMap&  attributes) {
  if (level_ == TopLevel) {
  } else {
    if(!parser_) {
      if (element == "items") {
        parser_ = new PubsubItemsRequestParser;
      } else if (element == "publish") {
        parser_ = new PubsubPublishRequestParser;
      } else if (element == "retract") { // TODO: on retract request server just replies with pure iq result, but retraction notification should come with event payload
        //parser_ = new PubsubRetractRequestParser;
      } else if (element == "subscription") {
        parser_ = new PubsubSubscribeRequestParser;
      } else { // TODO: parse unsubscribe response
        parser_ = new LogParser;
      }
    }
    assert(parser_);
    parser_->handleStartElement(element, ns, attributes);
  }
  ++level_;
}

void PubsubParser::handleEndElement(const std::string&  element, const std::string&  ns) {
  --level_;
  if (parser_) {
    parser_->handleEndElement(element, ns);
  }
}

void PubsubParser::handleCharacterData(const std::string& data) {
  if (parser_) {
    parser_->handleCharacterData(data);
  }
}

boost::shared_ptr<Swift::Payload> PubsubParser::getPayload() const {
  LOG(DEBUG) << "PubsubParser getPayload() object: " << typeid(*parser_->getPayload().get()).name();
  return parser_->getPayload();
}

/*
 * TOP LEVEL PUBSUB EVENT PARSER (element == "event", ns == "http://jabber.org/protocol/pubsub#event")
 */
EventPayloadParser::EventPayloadParser() : type_(Unknown), level_(TopLevel), parser_(0) {}

EventPayloadParser::~EventPayloadParser() {
  delete parser_;
}

void EventPayloadParser::handleStartElement(const std::string&  element, const std::string&  ns, const Swift::AttributeMap&  attributes) {
  if (level_ == TopLevel) {
  } else {
    if(!parser_) {
      if (element == "items") {
        type_ = Items;
        items_parser_ = new PubsubItemsRequestParser;
        parser_ = items_parser_;
      } else {
        parser_ = new LogParser;
      }
    }
    assert(parser_);
    parser_->handleStartElement(element, ns, attributes);

  }
  ++level_;
}

void EventPayloadParser::handleEndElement(const std::string&  element, const std::string&  ns) {
  --level_;
  if (parser_) {
    parser_->handleEndElement(element, ns);
  }
  if (level_ == PayloadLevel) {
    if (type_ == Items && element == "items") {
      getPayloadInternal()->setNode(items_parser_->getPayloadInternal()->getNode());
      getPayloadInternal()->setItems(items_parser_->getPayloadInternal()->getItems());
    } else {

    }
  }
}

void EventPayloadParser::handleCharacterData(const std::string& data) {
  if (parser_) {
    parser_->handleCharacterData(data);
  }
}

boost::shared_ptr<Swift::Payload> EventPayloadParser::getPayload() const {
  return getPayloadInternal();
}