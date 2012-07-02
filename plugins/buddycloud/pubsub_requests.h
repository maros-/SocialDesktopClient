/**
 * @file pubsub_requests.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 19, 2012
 */

#ifndef PUBSUB_REQUESTS_H_
#define PUBSUB_REQUESTS_H_

#include "payloads/pubsub.h"

/**
 * Pubsub <items> request
 */
class GetPubsubItemsRequest : public Swift::GenericRequest<PubsubItemsRequest> {
 public:
  typedef boost::shared_ptr<GetPubsubItemsRequest> ref;

  static ref create(const std::string &reciever, const std::string &node, Swift::IQRouter* router) {
    boost::shared_ptr<PubsubItemsRequest> payload(new PubsubItemsRequest);
    payload->setNode(node);
    return ref(new GetPubsubItemsRequest(Swift::IQ::Get, payload, reciever, router));
  }

  /*virtual void handleResponse(boost::shared_ptr<Swift::Payload> payload, Swift::ErrorPayload::ref error) {
    LOG(DEBUG) << "GetPubsubItemsRequest response object: " << typeid(*payload.get()).name();
    Swift::GenericRequest<PubsubItemsRequest>::handleResponse(payload, error);
  }*/

 private:
  GetPubsubItemsRequest(Swift::IQ::Type type, Swift::Payload::ref payload, const std::string &reciever, Swift::IQRouter* router)
      : Swift::GenericRequest<PubsubItemsRequest>(type, reciever, payload, router) {}
};

/**
 * Pubsub node item <retract> request
 */
class SetPubsubRetractRequest : public Swift::GenericRequest<PubsubRetractRequest> {
 public:
  typedef boost::shared_ptr<SetPubsubRetractRequest> ref;

  static ref create(const std::string &id, const std::string &node, const std::string &reciever, Swift::IQRouter* router) {
    PubsubRetractRequest::ref payload(new PubsubRetractRequest);
    payload->setNode(node);
    payload->setItemID(id);
    return ref(new SetPubsubRetractRequest(payload, reciever, router));
  }

 private:
  SetPubsubRetractRequest(Swift::Payload::ref payload, const std::string &reciever, Swift::IQRouter* router)
        : Swift::GenericRequest<PubsubRetractRequest>(Swift::IQ::Set, reciever, payload, router) {}
};

/**
 * Pubsub node item <publish> request
 */
class SetPubsubPublishRequest : public Swift::GenericRequest<PubsubPublishRequest> {
 public:
  typedef boost::shared_ptr<SetPubsubPublishRequest> ref;

  static ref create(Swift::Payload::ref payload, const Swift::JID &reciever, Swift::IQRouter* router) {
    return ref(new SetPubsubPublishRequest(payload, reciever, router));
  }

 private:
  SetPubsubPublishRequest(Swift::Payload::ref payload, const Swift::JID &reciever, Swift::IQRouter* router)
        : Swift::GenericRequest<PubsubPublishRequest>(Swift::IQ::Set, reciever, payload, router) {}
};

/**
 * Pubsub node <subscribe> request
 */
class SetPubsubSubscribeRequest : public Swift::GenericRequest<PubsubSubscribeRequest> {
 public:
  typedef boost::shared_ptr<SetPubsubSubscribeRequest> ref;

  static ref create(Swift::Payload::ref payload, const Swift::JID &reciever, Swift::IQRouter* router) {
    return ref(new SetPubsubSubscribeRequest(payload, reciever, router));
  }

 private:
  SetPubsubSubscribeRequest(Swift::Payload::ref payload, const Swift::JID &reciever, Swift::IQRouter* router)
      : Swift::GenericRequest<PubsubSubscribeRequest>(Swift::IQ::Set, reciever, payload, router) {}
};


#endif /* PUBSUB_REQUESTS_H_ */
