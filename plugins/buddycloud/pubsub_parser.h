/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef PUBSUB_PAYLOAD_PARSER_H_
#define PUBSUB_PAYLOAD_PARSER_H_

#include "payloads/items.h"
#include "sdc/core/log.h"
#include "Swiften/Parser/GenericPayloadParser.h"
#include "Swiften/Parser/PayloadParser.h"
#include "Swiften/Parser/PayloadParserFactoryCollection.h"
#include <string>

class Atom;
class EventPayload;
class PubsubItemsRequest;
class PubsubPublishRequest;
class PubsubRetractRequest;
class PubsubSubscribeRequest;
class Rsm;

class RsmParser;

/**
 * Parser for <items/> with various payload.
 */
class ItemParser : public Swift::GenericPayloadParser<Items> {
 public:
  ItemParser();
  ~ItemParser();

  /**
   * Adds specific payload parser to parse required payload within <item/> element.
   * @param parser
   */
  void addPaylodParserFactory(Swift::PayloadParserFactory* parser);

  virtual void handleStartElement(const std::string& element, const std::string& ns, const Swift::AttributeMap& attributes);
  virtual void handleEndElement(const std::string& element, const std::string& ns);
  virtual void handleCharacterData(const std::string& data);

 private:
  enum Level {
    TopLevel = 0
  };
  int level_;
  Swift::PayloadParserFactoryCollection parsers_;
  std::vector<Swift::PayloadParserFactory*> factories_;
  Swift::PayloadParser* curr_parser_;
  std::string character_data_;
  bool is_item_;
};

/**
 * Parser for debugging purposes.
 * All elements are dumped to LOG(DEBUG).
 */
class LogParser : public Swift::PayloadParser {
 public:
  void handleStartElement(const std::string&  element, const std::string&  ns, const Swift::AttributeMap&  /*attributes*/) {
    LOG(DEBUG) << "<" << element << " xmlns=" << ns << ">";
  }
  void handleEndElement(const std::string&  element, const std::string&  ns) {
    LOG(DEBUG) << "</" << element << " xmlns=" << ns << ">";
  }
  void handleCharacterData(const std::string&  data) {
    LOG(DEBUG) << data;
  }

 private:
  virtual boost::shared_ptr<Swift::Payload> getPayload() const {
    /* NO SINGLE PAYLOAD */
    return boost::shared_ptr<Swift::Payload>();
  }
};

/**
 * Parser for buddycloud Atom payload.
 */
class AtomParser : public Swift::GenericPayloadParser<Atom> {
 public:
  AtomParser();

  void handleStartElement(const std::string&  element, const std::string&  ns, const Swift::AttributeMap&  /*attributes*/);
  void handleEndElement(const std::string&  element, const std::string&  /*ns*/);
  void handleCharacterData(const std::string&  data);

 private:
  enum Level {
    TopLevel = 0,
    MetaLevel = 1
  };
  enum Meta {
    NIL,
    Author,
    Content,
    Object,
    Id
  };
  int level_;
  int meta_;
  std::string text_;
};

/**
 * Parser for Pubsub <items/> request.
 */
class PubsubItemsRequestParser : public Swift::GenericPayloadParser<PubsubItemsRequest> {
 public:
  PubsubItemsRequestParser();
  ~PubsubItemsRequestParser();

  virtual void handleStartElement(const std::string& element, const std::string& ns, const Swift::AttributeMap& attributes);
  virtual void handleEndElement(const std::string& element, const std::string& ns);
  virtual void handleCharacterData(const std::string& data);

 private:
  enum Level {
    TopLevel = 0
  };
  int level_;
  bool is_parsing_items_;
  ItemParser item_parser_;
  RsmParser* rsm_parser_;
  bool is_parsing_rsm_;
};

/**
 * Parser for Pubsub <publish/> request.
 */
class PubsubPublishRequestParser : public Swift::GenericPayloadParser<PubsubPublishRequest> {
 public:
  virtual void handleStartElement(const std::string& element, const std::string& /*ns*/, const Swift::AttributeMap& attributes);
  virtual void handleEndElement(const std::string& /*element*/, const std::string& /*ns*/) {}
  virtual void handleCharacterData(const std::string& /*data*/) {}
};

//class PubsubRetractRequestParser : public Swift::GenericPayloadParser<PubsubRetractRequest> {
// public:
//  virtual void handleStartElement(const std::string& element, const std::string& ns, const Swift::AttributeMap& attributes);
//  virtual void handleEndElement(const std::string& /*element*/, const std::string& /*ns*/) {}
//  virtual void handleCharacterData(const std::string& /*data*/) {}
//};

/**
 * Parser for Pubsub <subscribe/> request.
 */
class PubsubSubscribeRequestParser : public Swift::GenericPayloadParser<PubsubSubscribeRequest> {
 public:
  virtual void handleStartElement(const std::string& element, const std::string& /*ns*/, const Swift::AttributeMap& attributes);
  virtual void handleEndElement(const std::string& /*element*/, const std::string& /*ns*/) {}
  virtual void handleCharacterData(const std::string& /*data*/) {}
};

/**
 * Top-level Pubsub parser.
 */
class PubsubParser : public Swift::PayloadParser {
 public:
  PubsubParser();
  ~PubsubParser();

  void handleStartElement(const std::string&  element, const std::string&  ns, const Swift::AttributeMap&  attributes);
  void handleEndElement(const std::string&  element, const std::string&  ns);
  void handleCharacterData(const std::string& data);

  virtual boost::shared_ptr<Swift::Payload> getPayload() const;

 private:
  enum Level {
    TopLevel = 0,
    PayloadLevel = 1
  };
  int level_;
  enum Pubsub {
    Items,
    Publish
  } type_;
  PayloadParser* parser_;
};

/**
 * Top-level Pubsub <event/> payload.
 */
class EventPayloadParser : public Swift::GenericPayloadParser<EventPayload> {
 public:
  EventPayloadParser();

  ~EventPayloadParser();

  void handleStartElement(const std::string&  element, const std::string&  ns, const Swift::AttributeMap&  attributes);
  void handleEndElement(const std::string&  element, const std::string&  ns);
  void handleCharacterData(const std::string& data);

  virtual boost::shared_ptr<Swift::Payload> getPayload() const;

 private:
  enum Level {
    TopLevel = 0,
    PayloadLevel = 1
  };
  enum Type {
    Unknown,
    Items,
    Subscription,
    Retract
  } type_;
  int level_;
  PubsubItemsRequestParser* items_parser_;
  PayloadParser* parser_;
};

/**
 * Parser for <rsm/> items management payload.
 */
class RsmParser : public Swift::GenericPayloadParser<Rsm> {
 public:
  ~RsmParser();

  void handleStartElement(const std::string&  element, const std::string&  ns, const Swift::AttributeMap&  attributes);
  void handleEndElement(const std::string&  element, const std::string&  ns);
  void handleCharacterData(const std::string& data);

 private:
  std::string data_;
};

#endif // PUBSUB_PAYLOAD_PARSER_H_
