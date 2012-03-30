/**
 * @file pubsub_payload.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Dec 30, 2011
 */

#ifndef PUBSUB_PAYLOAD_H_
#define PUBSUB_PAYLOAD_H_

#include "Swiften/Swiften.h"
#include "boost/shared_ptr.hpp"
#include <string>
#include <vector>
#include "sdc.h"

template<class P>
class Items : public Swift::Payload {
 public:
  typedef boost::shared_ptr<Items<P> > ref;

  void appendPayload(boost::shared_ptr<P> payload) {
    items_.push_back(payload);
  }

  const std::vector<boost::shared_ptr<P> >& get() const {
    return items_;
  }

 private:
  std::vector<boost::shared_ptr<P> > items_;
};

/**
 * Various payloads
 */

class Atom : public Swift::Payload {
 public:
  friend class AtomSerializer;
  typedef boost::shared_ptr<Atom> ref;
  enum Verb { POST };
  enum ObjectType { NOTE, COMMENT };

  Atom() {}

  void setID(const std::string & id) {
    id_ = id;
  }

  void setVerb(Verb verb) {
    verb_ = verb;
  }

  void setObjectType(ObjectType type) {
    object_type_ = type;
  }

  void setPublished(const std::string & published) {
    published_ = published;
  }

  void setAuthor(const std::string & author) {
    author_ = author;
  }

  void setContent(const std::string & content) {
    content_ = content;
  }

  std::string getID() {
    return id_;
  }

  Verb getVerb() {
    return verb_;
  }

  ObjectType getObjectType() {
    return object_type_;
  }

  std::string getVerbString() {
    switch (verb_) {
      case POST:
        return "post";
      default:
        assert(false);
    }
  }

  std::string getObjectTypeString() {
    switch (object_type_) {
      case NOTE:
        return "note";
      case COMMENT:
        return "comment";
      default:
        assert(false);
    }
  }

  std::string getPublished() {
    return published_;
  }

  std::string getAuthor() {
    return author_;
  }

  std::string getContent() {
    return content_;
  }


 private:
  Verb verb_;
  ObjectType object_type_;
  std::string published_;
  std::string author_;
  std::string content_;
  std::string id_;
};

class Geoloc : Swift::Payload {
  // TODO: As far as this is a desktop client geoloc info are not that important
};

/**
 * Pubsub payloads
 */

class Pubsub : public Swift::Payload {
 public:
  typedef boost::shared_ptr<Pubsub> ref;
  enum Action { SUBSCRIBE, UNSUBSCRIBE, PUBLISH, RETRACT, ITEMS };

  Pubsub() {} // TODO: set default node and type

  Pubsub(Action type, std::string node) :
    type_(type),
    node_(node) {}

  void setPublishItem(Swift::Payload::ref payload) {
    publish_item_ = payload;
  }

  Swift::Payload::ref getPublishItem() {
    return publish_item_;
  }

  Action getAction() {
    return type_;
  }

  std::string getNode() {
    return node_;
  }

 private:
  Action type_;
  std::string node_;
  Swift::Payload::ref publish_item_;
};

class PubsubNode : public Swift::Payload {
 public:
  typedef boost::shared_ptr<PubsubNode> ref;

  void setNode(const std::string &node) {
    node_ = node;
  }

  std::string getNode() {
    return node_;
  }

 private:
  std::string node_;
};

class PubsubNodeItem : public PubsubNode {
 public:
  typedef boost::shared_ptr<PubsubNodeItem> ref;

  void setItemID(const std::string &id) {
    id_ = id;
  }

  std::string getItemID() {
    return id_;
  }

 private:
  std::string id_;
};

class PubsubItemsRequest : public PubsubNode {
 public:
  typedef boost::shared_ptr<PubsubItemsRequest> ref;

  void setItems(Items<Atom>::ref items) {
    items_ = items;
  }

  Items<Atom>::ref getItems() {
    return items_;
  }

 private:
  Items<Atom>::ref items_;
};

class PubsubRetractRequest : public PubsubNodeItem {
 public:
  typedef boost::shared_ptr<PubsubRetractRequest> ref;

  // TODO: this class can also be just a typedef
};

class PubsubPublishRequest : public PubsubNodeItem {
 public:
  typedef boost::shared_ptr<PubsubPublishRequest> ref;

  PubsubPublishRequest() : atom_(new Atom) {}

  void setAtom(Atom::ref atom) {
    atom_ = atom;
  }

  Atom::ref& getAtom() {
    return atom_;
  }

 private:
  Atom::ref atom_;
};

#endif /* PUBSUB_PAYLOAD_H_ */
