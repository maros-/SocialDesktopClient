/**
 * @file buddycloud_bot.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Nov 28, 2011
 */

#ifndef BUDDYCLOUD_BOT_H_
#define BUDDYCLOUD_BOT_H_

#include "buddycloud_connection.h"

// sdc
#include "qt_service.h"

#include "Swiften/Client/Client.h"
#include "Swiften/Client/ClientXMLTracer.h"
#include "Swiften/Elements/VCard.h"
#include "Swiften/EventLoop/SimpleEventLoop.h"
#include "Swiften/JID/JID.h"
#include "Swiften/Network/NetworkFactories.h"
#include "boost/signals2.hpp"
#include <vector>
#include <set>

class BcContact;
class ChannelController;

class BcModel : public sdc::QtServiceModel {
  public:
    friend class BuddycloudConnection;
    friend class ChannelController;

    BcModel(sdc::AccountData* account);
    ~BcModel();
    sdc::Connection* CreateConnection() {
      connection_ = new BuddycloudConnection(this);
      return connection_;
    }

    //GetSoftwareVersionRequest::ref gsvr = GetSoftwareVersionRequest::create("localhost", client_->getIQRouter());
    //GetVCardRequest::ref gvcr = GetVCardRequest::create(to, client_->getIQRouter());
    void ToggleChannelPrivacy();
    void Unsubscribe();
    void test();

    /*
     * Content interface
     */
    BcContact* GetContact(const Swift::JID &jid);
    ChannelController* GetChannel(const Swift::JID &jid);
    void AddNewContact(const Swift::JID &jid);
    void RemoveContact(const Swift::JID &jid);

    const std::string GetOwnAvatarPath() {
      return avatar_file_path_;
    }

    const std::string GetDefaultAvatarPath() {
      return service()->dir() + "/default_avatar.png";
    }

    /*
     * Errors
     */
    enum Error {
      ServiceUnavailable
    };
    /*
     * Signals
     */
    boost::signals2::signal<void (Error)> onError;
    boost::signals2::signal<void ()> onConnected;
    boost::signals2::signal<void (const Swift::JID)> onContactAdded;
    boost::signals2::signal<void (const Swift::JID)> onContactRemoved;
    boost::signals2::signal<void ()> onSelfChannelRegistered;
    boost::signals2::signal<void (Swift::VCard::ref)> onOwnVCardUpdate;
    boost::signals2::signal<void (const std::string file_path)> onOwnAvatarChange;

  private:
    /*
     * On request
     */
    void GetServerInfo();
    /*
     * Connected/disconnected handlers
     */
    void handleConnected();
    void handleDisconnected(boost::optional<Swift::ClientError> error);
    /*
     * Permanent stanza handling
     */
    void handleMessageReceived(Swift::Message::ref message);
    void handlePresenceReceived(Swift::Presence::ref presence);
    /*
     * Roster change handlers
     */
    void handleJidAdded(const Swift::JID &jid);
    void handleJidRemove(const Swift::JID &jid);
    void handleJidUpdated(const Swift::JID &jid, const std::string &name, const std::vector<std::string> &groups);
    /*
     * Parsers and serializers
     */
    void AddParserFactories();
    void AddSerializers();
    void AddParserFactory(Swift::PayloadParserFactory* factory);
    void AddSerializer(Swift::PayloadSerializer* serializer);
    void AddContact(const Swift::XMPPRosterItem &item);
    /*
     * Miscellaneous
     */
    const std::string SavePhoto(const Swift::JID &jid, Swift::VCard::ref vcard);
    /**
     * SDC data
     */
    sdc::AccountData* account_;
    BuddycloudConnection* connection_; // TODO: Rename to ChannelConnection
    std::vector<BcContact*> contacts_;
    std::map<Swift::JID, BcContact*> contacts_map_;
    /**
     * Domain server stuff
     */
    Swift::DiscoInfo::ref server_info_;
    /**
     * Channels
     */
    ChannelController* own_channel_;
    std::vector<ChannelController*> channels_;
    std::map<Swift::JID, ChannelController*> channels_map_;
    /**
     * Channel client data
     */
    Swift::JID jid_;
    Swift::JID service_jid_;
    bool is_service_registration_available_;
    std::string posts_node_;
    std::string avatar_file_path_;
    /**
     * Switen engine
     */
    Swift::SimpleEventLoop* loop_;
    Swift::NetworkFactories* network_;
    Swift::Client* client_;
    Swift::ClientXMLTracer* tracer_;

    bool connected_;
    /**
     * Serializers & Parsers
     */
    std::vector<Swift::PayloadParserFactory*> parsers_;
    std::vector<Swift::PayloadSerializer*> serializers_;
};

#endif /* BUDDYCLOUD_BOT_H_ */
