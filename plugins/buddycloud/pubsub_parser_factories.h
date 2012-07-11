#ifndef PUBSUB_PARSER_FACTORIES_H
#define PUBSUB_PARSER_FACTORIES_H

#include "Swiften/Parser/PayloadParserFactory.h"
#include "Swiften/Parser/GenericPayloadParserFactory.h"

class AtomParser;

class AnyParserFactory : public Swift::PayloadParserFactory {
 public:
  AnyParserFactory(Swift::PayloadParser* parser) : parser_(parser) {}
  ~AnyParserFactory() { delete parser_; }

  virtual bool canParse(const std::string& /*element*/, const std::string& /*ns*/, const Swift::AttributeMap& /*attributes*/) const {
      return true;
  }

  virtual Swift::PayloadParser* createPayloadParser() {
    return parser_;
  }

 private:
  Swift::PayloadParser* parser_;
};

class AtomParserFactory : public Swift::GenericPayloadParserFactory<AtomParser> {
 public:
  AtomParserFactory() : Swift::GenericPayloadParserFactory<AtomParser>("entry", "http://www.w3.org/2005/Atom") {}
};

#endif // PUBSUB_PARSER_FACTORIES_H
