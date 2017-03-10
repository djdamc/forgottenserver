/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FS_HTTP_API_SERVER_H_EFFE32E9805E557424770A73C1FD86BB
#define FS_HTTP_API_SERVER_H_EFFE32E9805E557424770A73C1FD86BB

#include "common.h"
#include "tools.h"
#include <memory>
#include <unordered_set>

namespace HttpApi
{

class Peer;
class Router;
using PeerSharedPtr = std::shared_ptr<Peer>; ///Shared pointer to Peer object

class GenericStreamAcceptor;

/** \brief represents the HTTP API subsystem
 *
 * This class handles preparation of the API subsystem and handling of incoming connections.
 * \remark Thread-safe - access to this object is synchronized with a local \ref Server::strand
 */
class Server : NonCopyable, NonMovable
{
	using Acceptor = GenericStreamAcceptor;
	using Peers = std::unordered_set<PeerSharedPtr>;

	std::unique_ptr<Acceptor> acceptor; ///The acceptor used to listen for incomming TCP/IP connections
	Peers peers; ///Set of connections, tracked so that we can perform a clean shutdown
	Strand strand; ///Internal strand for the server to synchronize access to the server
	PeerID peerCounter{}; ///Counter used to generate PeerIDs
	std::unique_ptr<Router> router; ///Router which dispatches handlers

	void accept();
	void start(const std::string& address, uint16_t port);
public:

    explicit Server(IoService& service);
	~Server();

	/** \brief Stops the API server
	 *
	 * Stops the API server by closing the acceptor and terminating connections to
	 * all connected \ref Peer objects.
	 * \remark Thread-safe - access to this object is synchronized with a local \ref Server::strand
	 */
	void stop();

	IoService& getIoService();

	/** \brief Handles closing of a remote connection
	 *
	 * Handles removing memory related to the \ref Peer that has just disconnected.
	 * \remark Thread-safe - access to this object is synchronized with a local \ref Server::strand
	 */
	void onPeerClose(Peer& peer);

	/** \brief loads routing lua subsystem
	 *
	 * Loads the routing lua subsystem. If failure occurs the server will continue to run but the API
	 * will be disabled.
	 * \remark NOT Thread-safe - access to this object is NOT synchronized
	 */
	void loadRoutes();
};

} //namespace HttpApi

#endif // FS_HTTP_API_SERVER_H_EFFE32E9805E557424770A73C1FD86BB